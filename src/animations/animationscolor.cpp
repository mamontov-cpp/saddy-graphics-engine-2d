#include "animations/animationscolor.h"
#include "animations/animationsinstance.h"

#include "animations/easing/easingfunction.h"

#include "animations/setstate/methodcall.h"
#include "animations/setstate/setproperty.h"
#include "animations/setstate/dummycommand.h"

#include "label.h"
#include "sprite2d.h"
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
#include "opticksupport.h"



DECLARE_SOBJ_INHERITANCE(sad::animations::Color, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================

sad::animations::Color::Color()
{
    PROFILER_EVENT;
    m_creators.pushProperty<sad::AColor>("color", "color");
}

sad::animations::Color::~Color()
{
    PROFILER_EVENT;
    
}


static sad::db::schema::Schema* AnimationColorSchema = nullptr;

static sad::Mutex AnimationColorSchemaInit;

sad::db::schema::Schema* sad::animations::Color::basicSchema()
{
    PROFILER_EVENT;
    if (AnimationColorSchema == nullptr)
    {
        AnimationColorSchemaInit.lock();
        if (AnimationColorSchema == nullptr)
        {
            AnimationColorSchema = new sad::db::schema::Schema();
            AnimationColorSchema->addParent(sad::animations::Animation::basicSchema());

            AnimationColorSchema->add(
                "min_color",
                new sad::db::MethodPair<sad::animations::Color, sad::AColor>(
                    &sad::animations::Color::minColor,
                    &sad::animations::Color::setMinColor
                )
            );
            AnimationColorSchema->add(
                "max_color",
                new sad::db::MethodPair<sad::animations::Color, sad::AColor>(
                    &sad::animations::Color::maxColor,
                    &sad::animations::Color::setMaxColor
                )
            );		        
            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationColorSchema);
        }
        AnimationColorSchemaInit.unlock();
    }
    return AnimationColorSchema;
}

sad::db::schema::Schema* sad::animations::Color::schema() const
{
    PROFILER_EVENT;
    return sad::animations::Color::basicSchema();
}



bool sad::animations::Color::loadFromValue(const picojson::value& v)
{
    PROFILER_EVENT;
    bool flag = this->sad::animations::Animation::loadFromValue(v);
    if (flag)
    {
        sad::Maybe<sad::AColor> mincolor = picojson::to_type<sad::AColor>(
                                                picojson::get_property(v, "min_color")
                                            );
        sad::Maybe<sad::AColor> maxcolor = picojson::to_type<sad::AColor>(
                                                picojson::get_property(v, "max_color")
                                            );
        bool result = mincolor.exists() && maxcolor.exists();
        if (result)
        {
            m_min_color = mincolor.value();
            m_max_color = maxcolor.value();
        }

        flag = flag && result;
    }
    return flag;
}


void sad::animations::Color::setMinColor(const sad::AColor& color)
{
    PROFILER_EVENT;
    m_min_color = color;
}

const sad::AColor& sad::animations::Color::minColor() const
{
    PROFILER_EVENT;
    return m_min_color;
}

void sad::animations::Color::setMaxColor(const sad::AColor& color)
{
    PROFILER_EVENT;
    m_max_color = color;
}

const sad::AColor& sad::animations::Color::maxColor() const
{
    PROFILER_EVENT;
    return m_max_color;
}

void sad::animations::Color::setState(sad::animations::Instance* i, double time)
{	
    double time_position = m_easing->eval(time, m_time);
    sad::AColor value = m_min_color + (m_max_color - m_min_color) * time_position;
    i->stateCommandAs<sad::AColor>()->call(value);
}



sad::animations::setstate::AbstractSetStateCommand* sad::animations::Color::stateCommand(sad::db::Object* o)
{
    PROFILER_EVENT;
    if (this->applicableTo(o))
    {
        sad::animations::setstate::AbstractSetStateCommand* c;
        if (o->isInstanceOf("sad::Label"))
        {
            c = sad::animations::setstate::make(
                    o,
                    &sad::Label::setColor
                );
        }
        else
        {
            if (o->isInstanceOf("sad::db::custom::Object"))
            {
                c = sad::animations::setstate::make(
                        o,
                        &sad::db::custom::Object::setColor
                    );
            }
            else
            {
                if (o->isInstanceOf("sad::Sprite2D"))
                {
                     c = sad::animations::setstate::make(
                            o,
                            &sad::Sprite2D::setColor
                         );
                }
                else
                {
                    c = new sad::animations::setstate::SetProperty<sad::AColor>(o, "color");					
                }
            }
        }
        return c;
    }
    return new sad::animations::setstate::DummyCommand<sad::AColor>();
}

bool sad::animations::Color::applicableTo(sad::db::Object* o)
{
    PROFILER_EVENT;
    bool result = false;
    if (o && m_valid)
    {
        result = o->getProperty<sad::AColor>("color").exists();
    }
    return result;
}
