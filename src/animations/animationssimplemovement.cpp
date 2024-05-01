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
#include "opticksupport.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "db/save.h"

#include "label.h"
#include "sprite2d.h"
#include "db/custom/customobject.h"
#include <3rdparty/picojson/valuetotype.h>

DECLARE_SOBJ_INHERITANCE(sad::animations::SimpleMovement, sad::animations::Animation)

// =============================== PUBLIC METHODS ==========================


sad::animations::SimpleMovement::SimpleMovement() : m_clamp_floating_points(false)
{
    PROFILER_EVENT;
    m_creators.pushCreator<sad::animations::SavedObjectPosition>("sad::animations::SavedObjectPosition");
}

sad::animations::SimpleMovement::~SimpleMovement() = default;

void sad::animations::SimpleMovement::setStartingPoint(const sad::Point2D& p)
{
    PROFILER_EVENT;
    m_start_point = p;
}

void sad::animations::SimpleMovement::setEndingPoint(const sad::Point2D& p)
{
    PROFILER_EVENT;
    m_end_point = p;
}

const sad::Point2D& sad::animations::SimpleMovement::startingPoint() const
{
    PROFILER_EVENT;
    return m_start_point;
}

const sad::Point2D& sad::animations::SimpleMovement::endingPoint() const
{
    PROFILER_EVENT;
    return m_end_point;
}

static sad::db::schema::Schema* AnimationSimpleMovementSchema = nullptr;

static sad::Mutex AnimationSimpleMovementInit;  // NOLINT(clang-diagnostic-exit-time-destructors)

sad::db::schema::Schema* sad::animations::SimpleMovement::basicSchema()
{
    PROFILER_EVENT;
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

            auto* clamp_field = sad::db::define_field(&sad::animations::SimpleMovement::m_clamp_floating_points);
            clamp_field->makeNonRequiredWithDefaultValue(new sad::db::Variant(false));
            AnimationSimpleMovementSchema->add(
                "clamp_floating_points",
                clamp_field
            );

            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationSimpleMovementSchema);
        }
        AnimationSimpleMovementInit.unlock();
    }
    return AnimationSimpleMovementSchema;
}

sad::db::schema::Schema* sad::animations::SimpleMovement::schema() const
{
    PROFILER_EVENT;
    return sad::animations::SimpleMovement::basicSchema();
}

bool sad::animations::SimpleMovement::loadFromValue(const picojson::value& v)
{
    PROFILER_EVENT;
    bool flag = this->sad::animations::Animation::loadFromValue(v);
    if (flag)
    {
        const sad::Maybe<sad::Point2D> maybe_start_point = picojson::to_type<sad::Point2D>(
                                                       picojson::get_property(v, "start_point")
                                                   );
        const sad::Maybe<sad::Point2D> maybe_end_point   = picojson::to_type<sad::Point2D>(
                                                       picojson::get_property(v, "end_point")
                                                   );
        const sad::Maybe<bool> maybe_clamp     = picojson::to_type<bool>(
                                                        picojson::get_property(v, "clamp_floating_points")
                                                   );
        const bool result = maybe_start_point.exists() && maybe_end_point.exists();
        if (maybe_start_point.exists())
        {
            m_start_point = maybe_start_point.value();
        }

        if (maybe_end_point.exists())
        {
            m_end_point = maybe_end_point.value();
        }
        if (maybe_clamp.exists())
        {
            m_clamp_floating_points = maybe_clamp.value();
        }


        flag = flag && result;
    }
    return flag;	
}

void sad::animations::SimpleMovement::setState(sad::animations::Instance* i, double time)
{
    PROFILER_EVENT;
    const double time_position = m_easing->eval(time, m_time);
    sad::Point2D pos = m_start_point + ((m_end_point - m_start_point) * time_position);
    if (m_clamp_floating_points)
    {
        pos.setX(std::round(pos.x()));
        pos.setY(std::round(pos.y()));
    }
    i->stateCommandAs<sad::Point2D>()->call(pos);
    if (i->body())
    {
        i->body()->shedulePosition(pos);
    }
}

sad::animations::setstate::AbstractSetStateCommand* sad::animations::SimpleMovement::stateCommand(sad::db::Object* o)
{
    PROFILER_EVENT;
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
    PROFILER_EVENT;
    bool result = false;
    if (o)
    {
        const bool area_property_exists = o->getProperty<sad::Rect2D>("area").exists();
        result = area_property_exists;
    }
    return result;
}

void sad::animations::SimpleMovement::setClampFloatingPoints(bool clamp)
{
    PROFILER_EVENT;
    m_clamp_floating_points = clamp;
}

bool sad::animations::SimpleMovement::clampFloatingPoints() const
{
    PROFILER_EVENT;
    return m_clamp_floating_points;
}
