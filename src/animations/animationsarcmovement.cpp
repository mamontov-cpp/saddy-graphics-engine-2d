#include "animations/animationsarcmovement.h"
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

DECLARE_SOBJ_INHERITANCE(sad::animations::ArcMovement, sad::animations::Animation)

// =============================== PUBLIC METHODS ==========================


sad::animations::ArcMovement::ArcMovement() : m_clamp_floating_points(false)
{
    PROFILER_EVENT;
    m_creators.pushCreator<sad::animations::SavedObjectPosition>("sad::animations::SavedObjectPosition");
}

sad::animations::ArcMovement::~ArcMovement() = default;

void sad::animations::ArcMovement::setCenterPoint(const sad::Point2D& p)
{
    PROFILER_EVENT;
    m_center_point = p;
    recomputeProperties();
}

void sad::animations::ArcMovement::setStartingPoint(const sad::Point2D& p)
{
    PROFILER_EVENT;
    m_start_point = p;
    recomputeProperties();
}

void sad::animations::ArcMovement::setEndingPoint(const sad::Point2D& p)
{
    PROFILER_EVENT;
    m_end_point = p;
    recomputeProperties();
}

const sad::Point2D& sad::animations::ArcMovement::centerPoint() const
{
    PROFILER_EVENT;
    return m_center_point;
}


const sad::Point2D& sad::animations::ArcMovement::startingPoint() const
{
    PROFILER_EVENT;
    return m_start_point;
}

const sad::Point2D& sad::animations::ArcMovement::endingPoint() const
{
    PROFILER_EVENT;
    return m_end_point;
}


static sad::db::schema::Schema* AnimationArcMovementSchema = nullptr;

static sad::Mutex AnimationArcMovementInit;  // NOLINT(clang-diagnostic-exit-time-destructors)

sad::db::schema::Schema* sad::animations::ArcMovement::basicSchema()
{
    PROFILER_EVENT;
    if (AnimationArcMovementSchema == nullptr)
    {
        AnimationArcMovementInit.lock();
        if (AnimationArcMovementSchema == nullptr)
        {
            AnimationArcMovementSchema = new sad::db::schema::Schema();
            AnimationArcMovementSchema->addParent(sad::animations::Animation::basicSchema());

            AnimationArcMovementSchema->add(
                "start_point",
                sad::db::define_field(&sad::animations::ArcMovement::m_start_point)
            );

            AnimationArcMovementSchema->add(
                "end_point",
                sad::db::define_field(&sad::animations::ArcMovement::m_end_point)
            );

            AnimationArcMovementSchema->add(
                "center_point",
                sad::db::define_field(&sad::animations::ArcMovement::m_center_point)
            );

            AnimationArcMovementSchema->add(
                "clamp_floating_points",
                sad::db::define_field(&sad::animations::ArcMovement::m_clamp_floating_points)
            );

            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationArcMovementSchema);
        }
        AnimationArcMovementInit.unlock();
    }
    return AnimationArcMovementSchema;
}

sad::db::schema::Schema* sad::animations::ArcMovement::schema() const
{
    PROFILER_EVENT;
    return sad::animations::ArcMovement::basicSchema();
}

bool sad::animations::ArcMovement::loadFromValue(const picojson::value& v)
{
    PROFILER_EVENT;
    bool flag = this->sad::animations::Animation::loadFromValue(v);
    if (flag)
    {
        const sad::Maybe<sad::Point2D> maybe_start_point = picojson::to_type<sad::Point2D>(
            picojson::get_property(v, "start_point")
        );
        const sad::Maybe<sad::Point2D> maybe_end_point = picojson::to_type<sad::Point2D>(
            picojson::get_property(v, "end_point")
        );
        const sad::Maybe<sad::Point2D> maybe_center_point = picojson::to_type<sad::Point2D>(
            picojson::get_property(v, "center_point")
        );
        const sad::Maybe<bool> maybe_clamp_floating_points = picojson::to_type<bool>(
            picojson::get_property(v, "clamp_floating_points")
        );
        const bool result = maybe_start_point.exists() && maybe_end_point.exists() && maybe_center_point.exists();
        if (maybe_start_point.exists())
        {
            m_start_point = maybe_start_point.value();
        }

        if (maybe_end_point.exists())
        {
            m_end_point = maybe_end_point.value();
        }

        if (maybe_center_point.exists())
        {
            m_center_point = maybe_center_point.value();
        }

        if (maybe_clamp_floating_points.exists())
        {
            m_clamp_floating_points = maybe_clamp_floating_points.value();
        }

        flag = flag && result;
    }
    return flag;
}

void sad::animations::ArcMovement::start(sad::animations::Instance* i)
{
    PROFILER_EVENT;
    recomputeProperties();
    this->sad::animations::Animation::start(i);
}


void sad::animations::ArcMovement::setState(sad::animations::Instance* i, double time)
{
    PROFILER_EVENT;
    const double time_position = m_easing->eval(time, m_time);
    const double kphi = m_properties.phi * time_position;
    sad::Point2D pos = m_properties.compute(kphi);
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

sad::animations::setstate::AbstractSetStateCommand* sad::animations::ArcMovement::stateCommand(sad::db::Object* o)
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

bool sad::animations::ArcMovement::applicableTo(sad::db::Object* o)
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

void sad::animations::ArcMovement::setClampFloatingPoints(bool clamp)
{
    PROFILER_EVENT;
    m_clamp_floating_points = clamp;
}

bool sad::animations::ArcMovement::clampFloatingPoints() const
{
    PROFILER_EVENT;
    return m_clamp_floating_points;
}

// ================================= PRIVATE METHODS =================================

void sad::animations::ArcMovement::recomputeProperties()
{
    PROFILER_EVENT;
    m_properties = sad::computeEllipticProperties(m_center_point, m_start_point, m_end_point, false);
}
