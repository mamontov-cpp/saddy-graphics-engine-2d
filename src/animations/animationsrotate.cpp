#include "animations/animationsrotate.h"
#include "animations/animationsinstance.h"

#include "animations/easing/easingfunction.h"

#include "animations/setstate/methodcall.h"
#include "animations/setstate/setproperty.h"
#include "animations/setstate/dummycommand.h"

#include "label.h"
#include "sprite2d.h"
#include "sadmutex.h"
#include "db/custom/customobject.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/dbtable.h"

#include <util/fs.h>

#include <resource/resourcefile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>



DECLARE_SOBJ_INHERITANCE(sad::animations::Rotate, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================

sad::animations::Rotate::Rotate() : m_min_angle(0), m_max_angle(0)
{
    m_creators.pushProperty<double>("angle", "angle");
}

sad::animations::Rotate::~Rotate()
{
    
}

static sad::db::schema::Schema* AnimationRotateSchema = nullptr;

static sad::Mutex AnimationRotateSchemaLock;

sad::db::schema::Schema* sad::animations::Rotate::basicSchema()
{
    if (AnimationRotateSchema == nullptr)
    {
        AnimationRotateSchemaLock.lock();
        if (AnimationRotateSchema == nullptr)
        {
            AnimationRotateSchema = new sad::db::schema::Schema();
            AnimationRotateSchema->addParent(sad::animations::Animation::basicSchema());

            AnimationRotateSchema->add(
                "min_angle",
                new sad::db::MethodPair<sad::animations::Rotate, double>(
                    &sad::animations::Rotate::minAngle,
                    &sad::animations::Rotate::setMinAngle
                )
            );
            AnimationRotateSchema->add(
                "max_angle",
                new sad::db::MethodPair<sad::animations::Rotate, double>(
                    &sad::animations::Rotate::maxAngle,
                    &sad::animations::Rotate::setMaxAngle
                )
            );				        
            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationRotateSchema);
        }
        AnimationRotateSchemaLock.unlock();
    }
    return AnimationRotateSchema;
}

sad::db::schema::Schema* sad::animations::Rotate::schema() const
{
    return sad::animations::Rotate::basicSchema();
}

bool sad::animations::Rotate::loadFromValue(const picojson::value& v)
{
    bool flag = this->sad::animations::Animation::loadFromValue(v);
    if (flag)
    {
         sad::Maybe<double> minvalue = picojson::to_type<double>(
                                                picojson::get_property(v, "min_angle")
                                            );
         sad::Maybe<double> maxvalue = picojson::to_type<double>(
                                                picojson::get_property(v, "max_angle")
                                            );
        bool result = minvalue.exists() && maxvalue.exists();
        if (result)
        {
            m_min_angle = minvalue.value();
            m_max_angle = maxvalue.value();
        }

        flag = flag && result;
    }
    return flag;
}


void sad::animations::Rotate::setMinAngle(double angle)
{
    m_min_angle = angle;
}

double sad::animations::Rotate::minAngle() const
{
    return m_min_angle;
}

void sad::animations::Rotate::setMaxAngle(double angle)
{
    m_max_angle = angle;
}

double sad::animations::Rotate::maxAngle() const
{
    return m_max_angle;
}

void sad::animations::Rotate::setState(sad::animations::Instance* i, double time)
{	
    double time_position = m_easing->eval(time, m_time);
    double value = m_min_angle + (m_max_angle - m_min_angle) * time_position;
    i->stateCommandAs<double>()->call(value);
}



sad::animations::setstate::AbstractSetStateCommand* sad::animations::Rotate::stateCommand(sad::db::Object* o)
{
    if (this->applicableTo(o))
    {
        sad::animations::setstate::AbstractSetStateCommand* c;
        if (o->isInstanceOf("sad::Label"))
        {
            c = sad::animations::setstate::make(
                    o,
                    &sad::Label::setAngle
                );
        }
        else
        {
            if (o->isInstanceOf("sad::db::custom::Object"))
            {
                c = sad::animations::setstate::make(
                        o,
                        &sad::db::custom::Object::setAngle
                    );
            }
            else
            {
                if (o->isInstanceOf("sad::Sprite2D"))
                {
                     c = sad::animations::setstate::make(
                            o,
                            &sad::Sprite2D::setAngle
                         );
                }
                else
                {
                    c = new sad::animations::setstate::SetProperty<double>(o, "angle");					
                }
            }
        }
        return c;
    }
    return new sad::animations::setstate::DummyCommand<double>();
}

bool sad::animations::Rotate::applicableTo(sad::db::Object* o)
{
    bool result = false;
    if (o && m_valid)
    {
        result = o->getProperty<double>("angle").exists();
    }
    return result;
}
