#include "animations/animationscamerarotation.h"
#include "animations/animationsinstance.h"
#include "animations/animationsanimationfastcall.h"

#include "fuzzyequal.h"
#include "scene.h"
#include "camera.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>



DECLARE_SOBJ_INHERITANCE(sad::animations::CameraRotation, sad::animations::Animation);

// =============================== PUBLIC METHODS OF sad::animations::CameraRotation::AbstractFastCall ==========================

sad::animations::CameraRotation::AbstractFastCall::~AbstractFastCall()
{

}

// =============================== PUBLIC METHODS OF sad::animations::CameraRotation::DummyFastCall ==========================

void sad::animations::CameraRotation::DummyFastCall::call(double angle)
{

}

sad::animations::CameraRotation::DummyFastCall::~DummyFastCall()
{

}

// =============================== PUBLIC METHODS OF sad::animations::CameraRotation::FastCall ==========================

sad::animations::CameraRotation::FastCall::FastCall(sad::Scene* s, const sad::Point3D& p) : m_scene(s), m_pivot(p)
{

}

void sad::animations::CameraRotation::FastCall::call(double angle)
{
	sad::Camera& c = m_scene->camera();
	c.Angle = angle;
	c.RotationVectorDirection = sad::Vector3D(0, 0, 1);
	c.TemporaryRotationOffset = m_pivot;
}

sad::animations::CameraRotation::FastCall::~FastCall()
{

}

// =============================== PUBLIC METHODS OF sad::animations::CameraRotation ==========================

sad::animations::CameraRotation::CameraRotation() : m_min_angle(0), m_max_angle(0)
{
	
}

sad::animations::CameraRotation::~CameraRotation()
{
	
}

bool sad::animations::CameraRotation::loadFromValue(const picojson::value& v)
{
	bool flag = this->sad::animations::Animation::loadFromValue(v);
	if (flag)
	{
		sad::Maybe<double> minangle     = picojson::to_type<double>(
										      picojson::get_property(v, "min_angle")
										  );
		sad::Maybe<double> maxangle     = picojson::to_type<double>(
										      picojson::get_property(v, "max_angle")
										  );
		sad::Maybe<sad::Point3D> pivot  = picojson::to_type<sad::Point3D>(
										          picojson::get_property(v, "pivot")
										      );
		bool result = minangle.exists() && maxangle.exists() && pivot.exists();
		if (result)
		{
			m_min_angle = minangle.value();
			m_max_angle = maxangle.value();
			m_pivot = pivot.value();
		}

		flag = flag && result;
	}
	return flag;
}

void sad::animations::CameraRotation::setMinAngle(double angle)
{
	m_min_angle = angle;
}

double sad::animations::CameraRotation::minAngle() const
{
	return m_min_angle;
}

void sad::animations::CameraRotation::setMaxAngle(double angle)
{
	m_max_angle = angle;
}

double sad::animations::CameraRotation::maxAngle() const
{
	return m_max_angle;
}

void sad::animations::CameraRotation::setPivot(const sad::Point3D& p)
{
	m_pivot = p;	
}

const sad::Point3D& sad::animations::CameraRotation::pivot() const
{
	return m_pivot;
}

void sad::animations::CameraRotation::setState(sad::animations::Instance* i, double time)
{
	double angle = m_min_angle + (m_max_angle - m_min_angle) * time / m_time;;
	static_cast<sad::animations::CameraRotation::AbstractFastCall*>(i->fastCall())->call(angle);
}

bool sad::animations::CameraRotation::saveState(sad::animations::Instance* i)
{
	sad::db::Object* o = i->object();
	bool result = false;
	if (o && m_valid)
	{
		if (o->isInstanceOf("sad::Scene"))
		{
			result = true;
			sad::Scene* s = static_cast<sad::Scene*>(o);
			sad::Point3D v = s->camera().RotationVectorDirection;
			double a = s->camera().Angle;
			sad::Point3D offset = s->camera().TemporaryRotationOffset;
			i->oldState().clear();
			i->oldState() << sad::db::Variant(v);
			i->oldState() << sad::db::Variant(a);
			i->oldState() << sad::db::Variant(offset);

			i->setFastCall(new sad::animations::CameraRotation::FastCall(s, m_pivot));
		}
	}
	if (!result)
	{
		i->setFastCall(new sad::animations::CameraRotation::DummyFastCall());
	}
	return result;
}


void sad::animations::CameraRotation::resetState(sad::animations::Instance* i)
{
	sad::db::Object* o = i->object();
	if (o && i->oldState().size() == 3)
	{
		sad::Maybe<sad::Point3D> rotvector = i->oldState()[0].get<sad::Point3D>();
		sad::Maybe<double> angle = i->oldState()[1].get<double>();
		sad::Maybe<sad::Point3D> offset = i->oldState()[2].get<sad::Point3D>();
		
		if (rotvector.exists() && angle.exists() && offset.exists())
		{
			sad::Scene* s = static_cast<sad::Scene*>(o);
			s->camera().RotationVectorDirection = rotvector.value();
			s->camera().Angle = angle.value();
			s->camera().TemporaryRotationOffset = offset.value();
		}
	}
}
