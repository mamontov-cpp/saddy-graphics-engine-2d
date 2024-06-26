#include "animations/animationswaymoving.h"
#include "animations/animationsinstance.h"
#include "animations/animationssavedobjectposition.h"

#include "animations/easing/easingfunction.h"

#include "animations/setstate/setpositionproperty.h"
#include "animations/setstate/setpositionviaareacall.h"
#include "animations/setstate/dummycommand.h"

#include "fuzzyequal.h"
#include "scene.h"
#include "camera.h"
#include "geometry2d.h"
#include "sadmutex.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/dbtable.h"

#include "label.h"
#include "sprite2d.h"
#include "db/custom/customobject.h"

#include <util/fs.h>

#include <resource/resourcefile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>
#include "opticksupport.h"


DECLARE_SOBJ_INHERITANCE(sad::animations::WayMoving, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================

sad::animations::WayMoving::WayMoving()
{
    PROFILER_EVENT;
    m_creators.pushCreator<sad::animations::SavedObjectPosition>("sad::animations::SavedObjectPosition");
}

sad::animations::WayMoving::~WayMoving()
{
    PROFILER_EVENT;
    
}


static sad::db::schema::Schema* AnimationWayMovingSchema = nullptr;

static sad::Mutex AnimationWayMovingSchemaInit;

sad::db::schema::Schema* sad::animations::WayMoving::basicSchema()
{
    PROFILER_EVENT;
    if (AnimationWayMovingSchema == nullptr)
    {
        AnimationWayMovingSchemaInit.lock();
        if (AnimationWayMovingSchema == nullptr)
        {
            AnimationWayMovingSchema = new sad::db::schema::Schema();
            AnimationWayMovingSchema->addParent(sad::animations::Animation::basicSchema());

            AnimationWayMovingSchema->add(
                "way",
                new sad::db::MethodPair<sad::animations::WayMoving, unsigned long long>(
                    &sad::animations::WayMoving::wayObjectId,
                    &sad::animations::WayMoving::setWayObjectId
                )
            );
                
            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationWayMovingSchema);
        }
        AnimationWayMovingSchemaInit.unlock();
    }
    return AnimationWayMovingSchema;
}

sad::db::schema::Schema* sad::animations::WayMoving::schema() const
{
    PROFILER_EVENT;
    return sad::animations::WayMoving::basicSchema();
}

void sad::animations::WayMoving::setTable(sad::db::Table * table)
{
    PROFILER_EVENT;
    this->sad::db::Object::setTable(table);
    m_way.setDatabase(table->database());
}


bool sad::animations::WayMoving::loadFromValue(const picojson::value& v)
{
    PROFILER_EVENT;
    bool flag = this->sad::animations::Animation::loadFromValue(v);
    if (flag)
    {
        sad::Maybe<unsigned long long> vector = picojson::to_type<unsigned long long>(
                                                   picojson::get_property(v, "way")
                                                );
        bool result = vector.exists();
        if (result)
        {
            setWayObjectId(vector.value());
        }

        flag = flag && result;
    }
    return flag;
}

void sad::animations::WayMoving::setWay(sad::Way * way)
{
    PROFILER_EVENT;
    m_way.setObject(way);
}

sad::Way* sad::animations::WayMoving::way() const
{
    PROFILER_EVENT;
    return m_way.get();
}

void sad::animations::WayMoving::setWayObjectId(unsigned long long id)
{
    PROFILER_EVENT;
    m_way.setMajorId(id);
}

unsigned long long sad::animations::WayMoving::wayObjectId() const
{
    PROFILER_EVENT;
    return m_way.majorId();
}

void sad::animations::WayMoving::setState(sad::animations::Instance* i, double time)
{
    PROFILER_EVENT;
    sad::Way* way = this->way();

    // This actually allows easing function to work as identity in case of linear function or do nothing in
    // all other cases
    double time_position = m_easing->evalBounded(time - 0.1, way->totalTime()) * way->totalTime();
    sad::Point2D pos = way->getPointInTime(time_position, 0.1);

    i->stateCommandAs<sad::Point2D>()->call(pos);
    if (i->body())
    {
        i->body()->shedulePosition(pos);
    }
}

sad::animations::setstate::AbstractSetStateCommand* sad::animations::WayMoving::stateCommand(sad::db::Object* o)
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

bool sad::animations::WayMoving::applicableTo(sad::db::Object* o)
{
    PROFILER_EVENT;
    bool result = false;
    if (o && way() != nullptr)
    {
        double waytime = way()->totalTime();
        bool areapropertyexists = o->getProperty<sad::Rect2D>("area").exists();
        bool waytimenotzero = !sad::is_fuzzy_zero(waytime);
        result = areapropertyexists && waytimenotzero;
        if (result)
        {
            setTime(waytime);
        }
    }
    return result;
}
