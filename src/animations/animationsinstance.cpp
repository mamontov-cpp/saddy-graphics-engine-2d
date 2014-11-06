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
: m_paused(false), m_started(false), m_finished(false), m_start_time(0), m_fastcall(NULL), m_valid(true)
{

}

sad::animations::Instance::~Instance()
{
	delete m_fastcall;
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
	this->clearFastCall();
}

sad::animations::Animation* sad::animations::Instance::animation() const
{
    return m_animation.get();
}

void sad::animations::Instance::setObject(sad::db::Object* o)
{
    m_object.setObject(o);
	this->clearFastCall();
}


sad::db::Object* sad::animations::Instance::object() const
{
    return const_cast<sad::animations::Instance*>(this)->m_object.get();
}

void sad::animations::Instance::setAnimationName(const sad::String& name)
{
    m_animation.setPath(name);
	this->clearFastCall();
}

const sad::String& sad::animations::Instance::animationName() const
{
    return m_animation.path();
}

void sad::animations::Instance::setObjectId(unsigned long long id)
{
    m_object.setMajorId(id);
	this->clearFastCall();
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


void sad::animations::Instance::restart()
{
	m_timer.stop();
	if (m_started && !m_finished)
	{
		this->cancel();
	}
	m_started = false;
	m_finished = false;
	m_paused =  false;
}


void sad::animations::Instance::clearFinished()
{
    m_finished = false;
    markAsValid();
}

bool sad::animations::Instance::finished() const
{
    return m_finished;
}


void sad::animations::Instance::process(sad::animations::Animations* animations)
{
    if (m_paused == false && m_valid)
    {
        sad::animations::Animation* a = m_animation.get();
        if (m_started == false)
        {
           this->start(animations);
           return;
        }

        double elapsed = m_start_time + m_timer.elapsed();
        // If time is expired, animation is or looped, or should be stopped
        if (elapsed > a->time())
        {
            if (a->looped())
            {
                double t = elapsed - floor(elapsed / a->time()) * a->time();
                a->setState(this, t);
            }
            else
            {
                m_started = false;
                m_timer.stop();
                this->markAsFinished();
                if (m_finished)
                {
                    this->restoreObjectState(animations);
                }
            }
        }
        else
        {
            a->setState(this, elapsed);
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

void sad::animations::Instance::addedToPipeline()
{
	this->addRef();
}

void sad::animations::Instance::removedFromPipeline()
{
	this->delRef();
}

void sad::animations::Instance::setFastCall(sad::animations::AnimationFastCall* call)
{
	delete m_fastcall;
	m_fastcall = call;
}

sad::animations::AnimationFastCall* sad::animations::Instance::fastCall() const
{
	return m_fastcall;
}

// ================================== PROTECTED METHODS ==================================

void sad::animations::Instance::start(sad::animations::Animations* animations)
{
    this->checkIfValid(animations);
    if (m_valid)
    {
        this->saveStateAndCompile(animations);
        a->setState(this, m_start_time);
        m_timer.start();
        m_started = true;
    }
    else
    {
        this->markAsFinished();
    }
}

void sad::animations::Instance::markAsFinished()
{
    m_finished = true;
    for(size_t i = 0; i < m_callbacks_on_end.size(); i++)
    {
        m_callbacks_on_end[i]->invoke();
    }
}

void sad::animations::Instance::checkIfValid(sad::animations::Animations* animations)
{
    sad::animations::Animation* a = m_animation.get();
    sad::db::Object* o = m_object.get();

    m_valid = (a && o);
    if (m_valid)
    {
        m_valid = a->valid() && a->applicableTo(o);
    }
}

void sad::animations::Instance::saveStateAndCompile(sad::animations::Animations* animations)
{
    sad::animations::Animation* a = m_animation.get();
    sad::db::Object* o = m_object.get();

    const sad::animations::SavedObjectStateDelegate& delegates = a->getCreators();
    for(size_t i = 0; i < delegates->size(); i++) {
        if (animations->cache().lookup(o, delegates[i]->name()) == false)
        {
            animations->cache().saveState(o, delegates[i]->name(), delegates[i]->saveState(o));
        }
        else
        {
            animations->cache().increment(o, delegates[i]->name())
        }
    }

    this->setFastCall(a->fastCallFor(o));
}

void sad::animations::Instance::restoreObjectState(sad::animations::Animations* animations)
{
    const sad::animations::SavedObjectStateDelegate& delegates = a->getCreators();
    for(size_t i = 0; i < delegates->size(); i++) {
        animations->cache().restore(o, delegates[i]->name());
    }
}

void sad::animations::Instance::clearFastCall()
{
	delete m_fastcall;
	m_fastcall = NULL;
}

void sad::animations::Instance::_process()
{

}
