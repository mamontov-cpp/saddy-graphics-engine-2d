#include "animations/animationscamerashaking.h"
#include "animations/animationsinstance.h"

#include "../fuzzyequal.h"
#include "../scene.h"
#include "../camera.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>



DECLARE_SOBJ_INHERITANCE(sad::animations::CameraShaking, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================


sad::animations::CameraShaking::CameraShaking() : m_frequency(0)
{
	
}

sad::animations::CameraShaking::~CameraShaking()
{
	
}

bool sad::animations::CameraShaking::loadFromValue(const picojson::value& v)
{
	bool flag = this->sad::animations::Animation::loadFromValue(v);
	if (flag)
	{
		sad::Maybe<sad::Point2D> offset = picojson::to_type<sad::Point2D>(
										      picojson::get_property(v, "offset")
										  );
		sad::Maybe<int> frequency       = picojson::to_type<int>(
										      picojson::get_property(v, "frequency")
										  );

		bool result = offset.exists() && frequency.exists();
		if (result)
		{
			m_offset = offset.value();
			m_frequency = frequency.value();
		}

		flag = flag && result;
	}
	return flag;
}

void sad::animations::CameraShaking::setOffset(const sad::Point2D& offset)
{
	m_offset = offset;
}

const sad::Point2D & sad::animations::CameraShaking::offset() const
{
	return m_offset;
}

void sad::animations::CameraShaking::setFrequency(int freq)
{
	m_frequency = freq;
}

int sad::animations::CameraShaking::frequency() const
{
	return m_frequency;
}

void sad::animations::CameraShaking::setState(sad::db::Object* o, double time)
{
	if (o == NULL || m_frequency == NULL || sad::is_fuzzy_zero(m_time))
		return;

	if (o->isInstanceOf("sad::Scene"))
	{
		sad::Scene* scene = static_cast<sad::Scene*>(o);
		sad::Point2D offset = m_offset * cos(time / m_time * static_cast<double>(m_frequency));
		scene->camera().TranslationOffset = offset;
	}
}

bool sad::animations::CameraShaking::saveState(sad::animations::Instance* i)
{
	sad::db::Object* o = i->object();
	bool result = false;
	if (o)
	{
		if (o->isInstanceOf("sad::Scene"))
		{
			result = true;
			sad::Scene* s = static_cast<sad::Scene*>(o);
			sad::Point3D v = s->camera().TranslationOffset;
			i->oldState().clear();
			i->oldState() << sad::db::Variant(v);
		}
	}
	return result;
}

void sad::animations::CameraShaking::resetState(sad::animations::Instance* i)
{
	sad::db::Object* o = i->object();
	if (o && i->oldState().size() == 1)
	{
		sad::Maybe<sad::Point3D> value = i->oldState()[0].get<sad::Point3D>();
		if (value.exists())
		{
			sad::Scene* s = static_cast<sad::Scene*>(o);
			s->camera().TranslationOffset = value.value();
		}
	}	
}


