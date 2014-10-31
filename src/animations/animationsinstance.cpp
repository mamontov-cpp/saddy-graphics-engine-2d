#include "animations/animationsinstance.h"

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fuzzyequal.h>

#include <math.h>
#include <fstream>

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/dbtable.h"


sad::animations::Instance::Instance()
: m_paused(false), m_started(false), m_finished(false)
{

}

sad::animations::Instance::~Instance()
{

}

static sad::db::schema::Schema* AnimationInstanceSchema = NULL;


sad::db::schema::Schema* sad::animations::Instance::basicSchema()
{
    if (AnimationInstanceSchema == NULL)
    {
        AnimationInstanceSchema = new sad::db::schema::Schema();
        AnimationInstanceSchema->addParent(sad::db::Object::basicSchema());

        AnimationInstanceSchema->add(
            "animation",
            new sad::db::MethodPair<sad::animations::Instance, sad::String>(
                &sad::animations::Instance::animationName,
                &sad::animations::Instance::setAnimationName
            )
        );
        AnimationInstanceSchema->add(
            "object",
            new sad::db::MethodPair<sad::animations::Instance, unsigned long long>(
                &sad::animations::Instance::objectId,
                &sad::animations::Instance::setObjectId
            )
        );
		AnimationInstanceSchema->add(
            "starttime",
            new sad::db::MethodPair<sad::animations::Instance, double>(
                &sad::animations::Instance::startTime,
                &sad::animations::Instance::setStartTime
            )
        );

        sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationInstanceSchema);
    }
    return AnimationInstanceSchema;
}

sad::db::schema::Schema* sad::animations::Instance::schema() const
{
    return sad::animations::Instance::basicSchema();
}

void sad::animations::Instance::setTreeName(sad::Renderer* r, const sad::String & tree_name)
{
    m_animation.setTree(r, tree_name);
}

void sad::animations::Instance::setTable(sad::db::Table *t)
{
    m_object.setDatabase(t->database());
}

static sad::String AnimationsInstanceSerializableName = "sad::animations::Instance";

const sad::String& sad::animations::Instance::serializableName() const
{
    return AnimationsInstanceSerializableName;
}

void sad::animations::Instance::setAnimation(sad::animations::Animation* o)
{
    m_animation.attach(o);
}

sad::animations::Animation* sad::animations::Instance::animation() const
{
    return m_animation.get();
}

void sad::animations::Instance::setObject(sad::db::Object* o)
{
    m_object.setObject(o);
}


sad::db::Object* sad::animations::Instance::object() const
{
    return const_cast<sad::animations::Instance*>(this)->m_object.get();
}

void sad::animations::Instance::setAnimationName(const sad::String& name)
{
    m_animation.setPath(name);
}

const sad::String& sad::animations::Instance::animationName() const
{
    return m_animation.path();
}

void sad::animations::Instance::setObjectId(unsigned long long id)
{
    m_object.setMajorId(id);
}

unsigned long long sad::animations::Instance::objectId() const
{
    return m_object.majorId();
}

void sad::animations::Instance::setStartTime(double time)
{
	m_start_time = time;
}

double sad::animations::Instance::startTime() const
{
	return m_start_time;
}

sad::Vector<sad::db::Variant>& sad::animations::Instance::oldState()
{
    return m_oldstate;
}

const sad::Vector<sad::db::Variant>& sad::animations::Instance::oldState() const
{
    return m_oldstate;
}


void sad::animations::Instance::addCallbackOnEnd(sad::animations::Callback* c)
{
    m_callbacks_on_end << c;
}


void sad::animations::Instance::clearFinished()
{
    m_finished = false;
}

bool sad::animations::Instance::finished() const
{
    return m_finished;
}


void sad::animations::Instance::process()
{
    if (m_paused == false)
    {
        sad::animations::Animation* a = m_animation.get();
        sad::db::Object* o = m_object.get();
        if (a && o)
        {
            if (sad::is_fuzzy_equal(a->time(), 0))
            {
                return ;
            }
            if (m_started == false)
            {
                if (a->saveState(this))
                {
					a->setState(o, m_start_time);
                    m_timer.start();
                    m_started = true;
                }
                else
                {
                    m_finished = true;
                    for(size_t i = 0; i < m_callbacks_on_end.size(); i++)
                    {
                        m_callbacks_on_end[i]->invoke();
                    }
                }
            }
            else
            {
                double elapsed = m_start_time + m_timer.elapsed();
                // If time is expired, animation is or looped, or should be stopped
                if (elapsed > a->time())
                {
                    if (a->looped())
                    {
                        double t = elapsed - floor(elapsed / a->time()) * a->time();
                        a->setState(o, t);
                    }
                    else
                    {
                        m_started = false;
                        m_finished = true;
                        m_timer.stop();
                        for(size_t i = 0; i < m_callbacks_on_end.size(); i++)
                        {
                            m_callbacks_on_end[i]->invoke();
                        }
                        if (m_finished)
                        {
                            a->resetState(this);
                        }
                    }
                }
                else
                {
                    a->setState(o, elapsed);
                }
            }
        }
    }
}

void sad::animations::Instance::pause()
{
    if (m_started)
    {
        m_paused = true;
        m_timer.pause();
    }
}


void sad::animations::Instance::resume()
{
    if (m_started)
    {
        m_paused = false;
        m_timer.resume();
    }
}


void sad::animations::Instance::cancel()
{
    sad::animations::Animation* a = m_animation.get();
    sad::db::Object* o = m_object.get();

    if (m_started && a && o)
    {
        m_paused = false;
        m_timer.stop();
        m_finished = true;
        a->resetState(this);
    }
}

// ================================== PROTECTED METHODS ==================================

void sad::animations::Instance::_process()
{

}
