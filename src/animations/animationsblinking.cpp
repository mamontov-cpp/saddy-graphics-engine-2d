#include "animations/animationsblinking.h"
#include "animations/animationsinstance.h"

#include "animations/setstate/methodcall.h"
#include "animations/setstate/setproperty.h"
#include "animations/setstate/dummycommand.h"

#include "animations/easing/easingfunction.h"

#include "label.h"
#include "sprite2d.h"
#include "db/custom/customobject.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbproperty.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/dbtable.h"

#include <util/fs.h>

#include <resource/resourcefile.h>

#include <3rdparty/picojson/valuetotype.h>

#include "sadmutex.h"

#include <fstream>


DECLARE_SOBJ_INHERITANCE(sad::animations::Blinking, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================

sad::animations::Blinking::Blinking() : m_frequency(1)
{
    m_creators.pushProperty<bool>("visible", "visible");
}

sad::animations::Blinking::~Blinking()
{
    
}


static sad::db::schema::Schema* AnimationBlinkingSchema = nullptr;

static sad::Mutex AnimationsBlinkingSchemaInit;

sad::db::schema::Schema* sad::animations::Blinking::basicSchema()
{
    if (AnimationBlinkingSchema == nullptr)
    {
        AnimationsBlinkingSchemaInit.lock();
        if (AnimationBlinkingSchema == nullptr)
        {
            AnimationBlinkingSchema = new sad::db::schema::Schema();
            AnimationBlinkingSchema->addParent(sad::animations::Animation::basicSchema());

            AnimationBlinkingSchema->add(
                "frequency",
                new sad::db::MethodPair<sad::animations::Blinking, unsigned int>(
                    &sad::animations::Blinking::frequency,
                    &sad::animations::Blinking::setFrequency
                )
            );		
            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationBlinkingSchema);
        }
        AnimationsBlinkingSchemaInit.unlock();
    }
    return AnimationBlinkingSchema;
}

sad::db::schema::Schema* sad::animations::Blinking::schema() const
{
    return sad::animations::Blinking::basicSchema();
}


bool sad::animations::Blinking::loadFromValue(const picojson::value& v)
{
    bool flag = this->sad::animations::Animation::loadFromValue(v);
    if (flag)
    {
        sad::Maybe<unsigned int> frequency = picojson::to_type<unsigned int>(
                                                picojson::get_property(v, "frequency")
                                            );
        bool result = frequency.exists();
        if (result)
        {
            setFrequency(frequency.value());
        }

        flag = flag && result;
    }
    return flag;
}


void sad::animations::Blinking::setFrequency(unsigned int freq)
{
    m_frequency = freq;
    m_inner_valid = m_frequency != 0;
    this->updateValidFlag();
}

unsigned int sad::animations::Blinking::frequency() const
{
    return m_frequency;
}


void sad::animations::Blinking::setState(sad::animations::Instance* i, double time)
{	
    double timeposition = m_easing->eval(time, m_time);
    unsigned int pos = static_cast<unsigned int>(timeposition * m_frequency);
    i->stateCommandAs<bool>()->call((pos % 2) != 0);
}



sad::animations::setstate::AbstractSetStateCommand* sad::animations::Blinking::stateCommand(sad::db::Object* o)
{
    if (this->applicableTo(o))
    {
        sad::animations::setstate::AbstractSetStateCommand* c;
        if (o->isInstanceOf("sad::SceneNode"))
        {
            c = sad::animations::setstate::make(
                    o,
                    &sad::SceneNode::setVisible
                );
        }
        else
        {
            c = new sad::animations::setstate::SetProperty<bool>(o, "visible");
        }
        return c;
    }
    return new sad::animations::setstate::DummyCommand<bool>();
}

bool sad::animations::Blinking::applicableTo(sad::db::Object* o)
{
    bool result = false;
    if (o && m_valid)
    {
        result = o->getProperty<bool>("visible").exists();
    }
    return result;
}
