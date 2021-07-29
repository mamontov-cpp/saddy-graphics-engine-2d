#include "animations/animationscamerashaking.h"
#include "animations/animationsinstance.h"

#include "animations/easing/easingfunction.h"

#include "animations/animationssavedcameratranslation.h"
#include "animations/setstate/setcameratranslation.h"
#include "animations/setstate/dummycommand.h"

#include "fuzzyequal.h"
#include "scene.h"
#include "camera.h"
#include "sadmutex.h"

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



DECLARE_SOBJ_INHERITANCE(sad::animations::CameraShaking, sad::animations::Animation);


// =============================== PUBLIC METHODS ==========================


sad::animations::CameraShaking::CameraShaking() : m_frequency(0)
{
    m_creators.pushCreator<sad::animations::SavedCameraTranslation>("sad::animations::SavedCameraTranslation");
}

sad::animations::CameraShaking::~CameraShaking()
{
    
}

static sad::db::schema::Schema* AnimationCameraShakingSchema = nullptr;

static sad::Mutex AnimationCameraShakingSchemaInit;

sad::db::schema::Schema* sad::animations::CameraShaking::basicSchema()
{
    if (AnimationCameraShakingSchema == nullptr)
    {
        AnimationCameraShakingSchemaInit.lock();
        if (AnimationCameraShakingSchema == nullptr)
        {
            AnimationCameraShakingSchema = new sad::db::schema::Schema();
            AnimationCameraShakingSchema->addParent(sad::animations::Animation::basicSchema());

            AnimationCameraShakingSchema->add(
                "offset",
                new sad::db::MethodPair<sad::animations::CameraShaking, sad::Point2D>(
                    &sad::animations::CameraShaking::offset,
                    &sad::animations::CameraShaking::setOffset
                )
            );
            AnimationCameraShakingSchema->add(
                "frequency",
                new sad::db::MethodPair<sad::animations::CameraShaking, int>(
                    &sad::animations::CameraShaking::frequency,
                    &sad::animations::CameraShaking::setFrequency
                )
            );		        
            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationCameraShakingSchema);
        }
        AnimationCameraShakingSchemaInit.unlock();
    }
    return AnimationCameraShakingSchema;
}

sad::db::schema::Schema* sad::animations::CameraShaking::schema() const
{
    return sad::animations::CameraShaking::basicSchema();
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

            m_inner_valid  = m_frequency != 0;
            this->updateValidFlag();
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
    m_inner_valid  = m_frequency != 0;
    this->updateValidFlag();
}

int sad::animations::CameraShaking::frequency() const
{
    return m_frequency;
}

void sad::animations::CameraShaking::setState(sad::animations::Instance* i, double time)
{
    double time_position = m_easing->eval(time, m_time);
    sad::Point2D offset = m_offset * cos(time_position * static_cast<double>(m_frequency));	
    i->stateCommandAs<sad::Point3D>()->call(offset);
}

sad::animations::setstate::AbstractSetStateCommand* sad::animations::CameraShaking::stateCommand(sad::db::Object* o)
{
    if (this->applicableTo(o) && o)
    {
        return new sad::animations::setstate::SetCameraTranslation(static_cast<sad::Scene*>(o));
    }
    return new sad::animations::setstate::DummyCommand<sad::Point3D>();
}

bool sad::animations::CameraShaking::applicableTo(sad::db::Object* o)
{
    bool result = false;
    if (o && m_valid)
    {
        result = o->isInstanceOf("sad::Scene");
    }
    return result;
}
