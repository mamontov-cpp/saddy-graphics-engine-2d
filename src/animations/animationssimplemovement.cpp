#include "animations/animationssimplemovement.h"
#include "animations/animationsinstance.h"
#include "animations/animationssavedobjectposition.h"

#include "animations/easing/easingfunction.h"

#include "animations/setstate/setpositionproperty.h"
#include "animations/setstate/setpositionviaareacall.h"
#include "animations/setstate/dummycommand.h"

#include "sadmutex.h"

#include "db/schema/schema.h"
#include "db/dbfield.h"
#include "db/save.h"

#include "label.h"
#include "sprite2d.h"
#include "db/custom/customobject.h"
#include <3rdparty/picojson/valuetotype.h>

DECLARE_SOBJ_INHERITANCE(sad::animations::SimpleMovement, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================


sad::animations::SimpleMovement::SimpleMovement()
{
    m_creators.pushCreator<sad::animations::SavedObjectPosition>("sad::animations::SavedObjectPosition");
}

sad::animations::SimpleMovement::~SimpleMovement()
{
    
}

void sad::animations::SimpleMovement::setStartingPoint(const sad::Point2D& p)
{
    m_start_point = p;
}

void sad::animations::SimpleMovement::setEndingPoint(const sad::Point2D& p)
{
    m_end_point = p;
}

const sad::Point2D& sad::animations::SimpleMovement::startingPoint() const
{
    return m_start_point;
}

const sad::Point2D& sad::animations::SimpleMovement::endingPoint() const
{
    return m_end_point;    
}

static sad::db::schema::Schema* AnimationSimpleMovementSchema = nullptr;

static sad::Mutex AnimationSimpleMovementInit;

sad::db::schema::Schema* sad::animations::SimpleMovement::basicSchema()
{
    if (AnimationSimpleMovementSchema == nullptr)
    {
        AnimationSimpleMovementInit.lock();
        if (AnimationSimpleMovementSchema == nullptr)
        {
            AnimationSimpleMovementSchema = new sad::db::schema::Schema();
            AnimationSimpleMovementSchema->addParent(sad::animations::Animation::basicSchema());

            AnimationSimpleMovementSchema->add(
                "start_point",
                sad::db::define_field(&sad::animations::SimpleMovement::m_start_point)
            );

            AnimationSimpleMovementSchema->add(
                "end_point",
                sad::db::define_field(&sad::animations::SimpleMovement::m_end_point)
            );
                
            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationSimpleMovementSchema);
        }
        AnimationSimpleMovementInit.unlock();
    }
    return AnimationSimpleMovementSchema;
}

sad::db::schema::Schema* sad::animations::SimpleMovement::schema() const
{
    return sad::animations::SimpleMovement::basicSchema();
}

bool sad::animations::SimpleMovement::loadFromValue(const picojson::value& v)
{
    bool flag = this->sad::animations::Animation::loadFromValue(v);
    if (flag)
    {
        sad::Maybe<sad::Point2D> maybestartpoint = picojson::to_type<sad::Point2D>(
                                                       picojson::get_property(v, "start_point")
                                                   );
        sad::Maybe<sad::Point2D> maybeendpoint   = picojson::to_type<sad::Point2D>(
                                                       picojson::get_property(v, "end_point")
                                                   );
        bool result = maybestartpoint.exists() && maybeendpoint.exists();
        if (maybestartpoint.exists())
        {
            m_start_point = maybestartpoint.value();
        }

        if (maybeendpoint.exists())
        {
            m_end_point = maybeendpoint.value();
        }


        flag = flag && result;
    }
    return flag;	
}

void sad::animations::SimpleMovement::setState(sad::animations::Instance* i, double time)
{
    double time_position = m_easing->eval(time, m_time);
    sad::Point2D pos = m_start_point + ((m_end_point - m_start_point) * time_position);

    i->stateCommandAs<sad::Point2D>()->call(pos);
    if (i->body())
    {
        i->body()->shedulePosition(pos);
    }
}

sad::animations::setstate::AbstractSetStateCommand* sad::animations::SimpleMovement::stateCommand(sad::db::Object* o)
{
    if (o)
    {
        if (o->isInstanceOf("sad::Sprite2D"))
        {
            return new sad::animations::setstate::SetPositionViaMethodCall<sad::Sprite2D>(o);
        }
        
        if (o->isInstanceOf("sad::Label"))
        {
            return new sad::animations::setstate::SetPositionViaMethodCall<sad::Label>(o);
        }
            
        if (o->isInstanceOf("sad::db::custom::Object"))
        {
            return new sad::animations::setstate::SetPositionViaMethodCall<sad::db::custom::Object>(o);
        }
        
        return new sad::animations::setstate::SetPositionProperty(o);
    }
    return new sad::animations::setstate::DummyCommand<sad::Point2D>();
}

bool sad::animations::SimpleMovement::applicableTo(sad::db::Object* o)
{
    bool result = false;
    if (o)
    {
        bool areapropertyexists = o->getProperty<sad::Rect2D>("area").exists();
        result = areapropertyexists;
    }
    return result;
}
