#include "animations/animationsresize.h"
#include "animations/animationsinstance.h"
#include "animations/animationssavedobjectsize.h"

#include "animations/easing/easingfunction.h"

#include "animations/setstate/methodcall.h"
#include "animations/setstate/setproperty.h"
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


DECLARE_SOBJ_INHERITANCE(sad::animations::Resize, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================


sad::animations::Resize::Resize()
{
    PROFILER_EVENT;
    m_creators.pushCreator<sad::animations::SavedObjectSize>("sad::animations::SavedObjectSize");
}

sad::animations::Resize::~Resize()
{
    PROFILER_EVENT;
    
}

static sad::db::schema::Schema* AnimationResizeSchema = nullptr;

static sad::Mutex AnimationResizeSchemaLock;
sad::db::schema::Schema* sad::animations::Resize::basicSchema()
{
    PROFILER_EVENT;
    if (AnimationResizeSchema == nullptr)
    {
        AnimationResizeSchemaLock.lock();
        if (AnimationResizeSchema == nullptr)
        {
            AnimationResizeSchema = new sad::db::schema::Schema();
            AnimationResizeSchema->addParent(sad::animations::Animation::basicSchema());
            void (sad::animations::Resize::* setstartsize)(const sad::Point2D&) = 
                &sad::animations::Resize::setStartSize;
            void (sad::animations::Resize::* setendsize)(const sad::Point2D&) = 
                &sad::animations::Resize::setEndSize;

            AnimationResizeSchema->add(
                "start_size",
                new sad::db::MethodPair<sad::animations::Resize, sad::Point2D>(
                    &sad::animations::Resize::startSize,
                    setstartsize
                )
            );
            AnimationResizeSchema->add(
                "end_size",
                new sad::db::MethodPair<sad::animations::Resize, sad::Point2D>(
                    &sad::animations::Resize::endSize,
                    setendsize
                )
            );
                
            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationResizeSchema);
        }
        AnimationResizeSchemaLock.unlock();
    }
    return AnimationResizeSchema;
}

sad::db::schema::Schema* sad::animations::Resize::schema() const
{
    PROFILER_EVENT;
    return sad::animations::Resize::basicSchema();
}

bool sad::animations::Resize::loadFromValue(const picojson::value& v)
{
    PROFILER_EVENT;
    bool flag = this->sad::animations::Animation::loadFromValue(v);
    if (flag)
    {
        sad::Maybe<sad::Point2D> startsize = picojson::to_type<sad::Point2D>(
                                              picojson::get_property(v, "start_size")
                                          );
        sad::Maybe<sad::Point2D> endsize   = picojson::to_type<sad::Point2D>(
                                              picojson::get_property(v, "end_size")
                                          );
        bool result = startsize.exists() && endsize.exists();
        if (result)
        {
            m_start_size = startsize.value();
            m_end_size = endsize.value();
        }

        flag = flag && result;
    }
    return flag;
}


void sad::animations::Resize::setStartSize(const sad::Point2D& v)
{
    PROFILER_EVENT;
    m_start_size = v;
}

void sad::animations::Resize::setStartSize(const sad::Size2D& v)
{
    PROFILER_EVENT;
    m_start_size.setX(v.Width);
    m_start_size.setY(v.Height);
}

void sad::animations::Resize::setEndSize(const sad::Point2D& v)
{
    PROFILER_EVENT;
    m_end_size = v;
}

void sad::animations::Resize::setEndSize(const sad::Size2D& v)
{
    PROFILER_EVENT;
    m_end_size.setX(v.Width);
    m_end_size.setY(v.Height);
}

const sad::Point2D& sad::animations::Resize::startSize() const
{
    PROFILER_EVENT;
    return m_start_size;
}

const sad::Point2D& sad::animations::Resize::endSize() const
{
    PROFILER_EVENT;
    return m_end_size;    
}


void sad::animations::Resize::start(sad::animations::Instance* i)
{
    PROFILER_EVENT;
    i->setBasicArea(i->object()->getProperty<sad::Rect2D>("area").value());
}

void sad::animations::Resize::setState(sad::animations::Instance* i, double time)
{
    PROFILER_EVENT;
    double distx = m_end_size.x() - m_start_size.x();
    double disty = m_end_size.y() - m_start_size.y();

    double time_position = m_easing->eval(time, m_time);
    double px =  (m_start_size.x() + distx * time_position) / 2.0;
    double py = (m_start_size.y() + disty * time_position) / 2.0;
    
    sad::Rect2D area = i->object()->getProperty<sad::Rect2D>("area").value();
    
    sad::Point2D pr = area.p0();
    pr += area.p2();
    pr /= 2.0;

    sad::Rect2D r(
        -px, -py, 
        px, py
    );

    sad::moveBy(pr, r);

    i->stateCommandAs<sad::Rect2D>()->call(r);
    if (i->body())
    {
        sad::p2d::CollisionShape* s = i->shape()->clone();
        s->resizeBy(sad::Point2D(px, py));
        i->body()->setShape(s);
    }
}

sad::animations::setstate::AbstractSetStateCommand* sad::animations::Resize::stateCommand(sad::db::Object* o)
{
    PROFILER_EVENT;
    if (this->applicableTo(o))
    {
        sad::animations::setstate::AbstractSetStateCommand* c = nullptr;
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
    PROFILER_EVENT;
    bool result = false;
    if (o && m_valid)
    {
        result = o->getProperty<sad::Rect2D>("area").exists();
    }
    return result;
}
