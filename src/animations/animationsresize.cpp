#include "animations/animationsresize.h"
#include "animations/animationsinstance.h"
#include "animations/animationssavedobjectsize.h"

#include "animations/setstate/methodcall.h"
#include "animations/setstate/setproperty.h"

#include "fuzzyequal.h"
#include "scene.h"
#include "camera.h"
#include "geometry2d.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"

#include "label.h"
#include "sprite2d.h"
#include "db/custom/customobject.h"

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>


DECLARE_SOBJ_INHERITANCE(sad::animations::Resize, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================


sad::animations::Resize::Resize()
{
	m_creators.pushCreator<sad::animations::SavedObjectSize>("sad::animations::SavedObjectSize");
}

sad::animations::Resize::~Resize()
{
	
}

bool sad::animations::Resize::loadFromValue(const picojson::value& v)
{
	bool flag = this->sad::animations::Animation::loadFromValue(v);
	if (flag)
	{
		sad::Maybe<sad::Point2D> vector = picojson::to_type<sad::Point2D>(
										      picojson::get_property(v, "vector")
										  );
		bool result = vector.exists();
		if (result)
		{
			m_vector = vector.value();
		}

		flag = flag && result;
	}
	return flag;
}


void sad::animations::Resize::setVector(const sad::Point2D& v)
{
	m_vector = v;
}

const sad::Point2D& sad::animations::Resize::vector() const
{
	return m_vector;
}

void sad::animations::Resize::start(sad::animations::Instance* i)
{
	i->setBasicArea(i->object()->getProperty<sad::Rect2D>("area").value());
}

void sad::animations::Resize::setState(sad::animations::Instance* i, double time)
{
	sad::Point2D p = m_vector * (time / m_time);
	sad::Rect2D r(i->basicArea());
	sad::Rect2D area = i->object()->getProperty<sad::Rect2D>("area").value();
	
	sad::Point2D pr = area.p0();
	pr += area.p1();
	pr += area.p2();
	pr += area.p3();
	pr /= 4;

	r[0] += sad::Point2D(-p.x(), -p.y());
	r[1] += sad::Point2D(p.x(), -p.y());
	r[2] += sad::Point2D(p.x(), p.y());
	r[3] += sad::Point2D(-p.x(), p.y());
	sad::moveBy(pr - i->basicCenter(), r);

	i->stateCommandAs<sad::Rect2D>()->call(r);
	if (i->body())
	{
		sad::p2d::CollisionShape* s = i->shape()->clone();
		s->resizeBy(p);
		i->body()->setShape(s);
	}
}

sad::animations::setstate::AbstractSetStateCommand* sad::animations::Resize::stateCommand(sad::db::Object* o)
{
	if (this->applicableTo(o))
    {
        sad::animations::setstate::AbstractSetStateCommand* c = NULL;
        if (o->isInstanceOf("sad::Label"))
        {
            c = sad::animations::setstate::make(
                    o,
					&sad::Label::setArea
                );
        }
        else
        {
			if (o->isInstanceOf("sad::Sprite2D"))
			{
				c = sad::animations::setstate::make(
						o,
						&sad::Sprite2D::setArea
					);
			}
			else
			{
				if (o->isInstanceOf("sad::db::custom::Object"))
				{
					c = sad::animations::setstate::make(
							o,
							&sad::db::custom::Object::setArea
						);
				}
				else
				{
					c = new sad::animations::setstate::SetProperty<sad::Rect2D>(o, "area");
				}
			}
        }
        return c;
    }
    return new sad::animations::setstate::DummyCommand<sad::Rect2D>();
}


bool sad::animations::Resize::applicableTo(sad::db::Object* o)
{
	bool result = false;
    if (o && m_valid)
    {
		result = o->getProperty<sad::Rect2D>("area").exists();
    }
    return result;
}

