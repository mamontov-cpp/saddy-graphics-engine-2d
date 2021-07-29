#include "animations/animationsfontsize.h"
#include "animations/animationsinstance.h"
#include "animations/setstate/dummycommand.h"

#include "animations/easing/easingfunction.h"

#include "animations/setstate/methodcall.h"
#include "animations/setstate/setproperty.h"

#include "label.h"
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



DECLARE_SOBJ_INHERITANCE(sad::animations::FontSize, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================

sad::animations::FontSize::FontSize() : m_min_size(0), m_max_size(0)
{
    m_creators.pushProperty<unsigned int>("fontsize", "fontsize");
}

sad::animations::FontSize::~FontSize()
{
    
}

static sad::db::schema::Schema* AnimationFontSizeSchema = nullptr;

static sad::Mutex AnimationFontSizeSchemaInit;

sad::db::schema::Schema* sad::animations::FontSize::basicSchema()
{
    if (AnimationFontSizeSchema == nullptr)
    {
        AnimationFontSizeSchemaInit.lock();
        if (AnimationFontSizeSchema == nullptr)
        {
            AnimationFontSizeSchema = new sad::db::schema::Schema();
            AnimationFontSizeSchema->addParent(sad::animations::Animation::basicSchema());

            AnimationFontSizeSchema->add(
                "min_size",
                new sad::db::MethodPair<sad::animations::FontSize, unsigned int>(
                    &sad::animations::FontSize::minSize,
                    &sad::animations::FontSize::setMinSize
                )
            );
            AnimationFontSizeSchema->add(
                "max_size",
                new sad::db::MethodPair<sad::animations::FontSize, unsigned int>(
                    &sad::animations::FontSize::maxSize,
                    &sad::animations::FontSize::setMaxSize
                )
            );
                
            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationFontSizeSchema);
        }
        AnimationFontSizeSchemaInit.unlock();
    }
    return AnimationFontSizeSchema;
}

sad::db::schema::Schema* sad::animations::FontSize::schema() const
{
    return sad::animations::FontSize::basicSchema();
}


bool sad::animations::FontSize::loadFromValue(const picojson::value& v)
{
    bool flag = this->sad::animations::Animation::loadFromValue(v);
    if (flag)
    {
         sad::Maybe<unsigned int> minsize = picojson::to_type<unsigned int>(
                                                picojson::get_property(v, "min_size")
                                            );
         sad::Maybe<unsigned int> maxsize = picojson::to_type<unsigned int>(
                                                picojson::get_property(v, "max_size")
                                            );
        bool result = minsize.exists() && maxsize.exists();
        if (result)
        {
            m_min_size = minsize.value();
            m_max_size = maxsize.value();
        }

        flag = flag && result;
    }
    return flag;
}


void sad::animations::FontSize::setMinSize(unsigned int size)
{
    m_min_size = size;
}

unsigned int sad::animations::FontSize::minSize() const
{
    return m_min_size;
}

void sad::animations::FontSize::setMaxSize(unsigned int size)
{
    m_max_size = size;
}

unsigned int sad::animations::FontSize::maxSize() const
{
    return m_max_size;
}

void sad::animations::FontSize::setState(sad::animations::Instance* i, double time)
{	
    double time_position = m_easing->eval(time, m_time);
    double min = m_min_size;
    double max = m_max_size;
    double value = min + (max - min) * time_position;
    unsigned int kvalue = static_cast<unsigned int>(value);
    i->stateCommandAs<unsigned int>()->call(kvalue);
}



sad::animations::setstate::AbstractSetStateCommand* sad::animations::FontSize::stateCommand(sad::db::Object* o)
{
    if (this->applicableTo(o))
    {
        sad::animations::setstate::AbstractSetStateCommand* c = nullptr;
        if (o->isInstanceOf("sad::Label"))
        {
            c = sad::animations::setstate::make(
                    o,
                    &sad::Label::setSize
                );
        }
        else
        {
            if (o->isInstanceOf("sad::db::custom::Object"))
            {
                c = sad::animations::setstate::make(
                        o,
                        &sad::db::custom::Object::setFontSize
                    );
            }
            else
            {
                c = new sad::animations::setstate::SetProperty<unsigned int>(o, "fontsize");
            }
        }
        return c;
    }
    return new sad::animations::setstate::DummyCommand<unsigned int>();
}

bool sad::animations::FontSize::applicableTo(sad::db::Object* o)
{
    bool result = false;
    if (o && m_valid)
    {
        result = o->getProperty<unsigned int>("fontsize").exists();
    }
    return result;
}
