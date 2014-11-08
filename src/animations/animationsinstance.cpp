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
: m_paused(false),
m_started(false),
m_finished(false),
m_start_time(0),
m_state_command(NULL),
m_valid(true),
m_shape(NULL),
m_body(NULL)
{

}

sad::animations::Instance::~Instance()
{
	delete m_state_command;
    delete m_shape;
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
	this->clearSetState();
}

sad::animations::Animation* sad::animations::Instance::animation() const
{
    return m_animation.get();
}

void sad::animations::Instance::setObject(sad::db::Object* o)
{
    m_object.setObject(o);
	this->clearSetState();
}


sad::db::Object* sad::animations::Instance::object() const
{
    return const_cast<sad::animations::Instance*>(this)->m_object.get();
}

void sad::animations::Instance::setAnimationName(const sad::String& name)
{
    m_animation.setPath(name);
	this->clearSetState();
}

const sad::String& sad::animations::Instance::animationName() const
{
    return m_animation.path();
}

void sad::animations::Instance::setObjectId(unsigned long long id)
{
    m_object.setMajorId(id);
	this->clearSetState();
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

void sad::animations::Instance::addCallbackOnEnd(sad::animations::Callback* c)
{
    m_callbacks_on_end << c;
}

void sad::animations::Instance::restart(sad::animations::Animations* animations)
{
	m_timer.stop();
	if (m_started && !m_finished)
	{
        this->cancel(animations);
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
        if (m_started == false)
        {
           this->start(animations);
           return;
        }

        double time = this->computeTime(animations);
        if (m_finished == false)
        {
            this->processTime(animations, time);
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


void sad::animations::Instance::cancel(sad::animations::Animations* animations)
{
    if (m_started && m_valid)
    {
        m_paused = false;
        m_timer.stop();
        m_finished = true;
		this->restoreObjectState(animations);
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

void sad::animations::Instance::setStateCommand(sad::animations::setstate::AbstractSetStateCommand* call)
{
	delete m_state_command;
	m_state_command = call;
}

sad::animations::setstate::AbstractSetStateCommand* sad::animations::Instance::stateCommand() const
{
	return m_state_command;
}

void sad::animations::Instance::setCollisionShape(sad::p2d::CollisionShape* shape)
{
    delete m_shape;
    m_shape = shape;
    if (m_shape)
    {
        m_shape->move(sad::Point2D(0, 0) - m_shape->center());
    }
}


sad::p2d::CollisionShape* sad::animations::Instance::shape()
{
    return m_shape;
}

void sad::animations::Instance::setBody(sad::p2d::Body* body)
{
    m_body = body;
    delete m_shape;
    if (m_body)
    {
        m_shape = m_body->currentShape()->clone();
        if (m_shape)
        {
            m_shape->move(sad::Point2D(0, 0) - m_shape->center());
        }
    }
    else
    {
        m_shape = NULL;
    }
}

sad::p2d::Body* sad::animations::Instance::body() const
{
    return m_body;
}

void sad::animations::Instance::setBasicArea(const sad::Rect2D& r)
{
	m_basic_area = r;
	m_basic_center = (r[0] + r[2]) / 2.0;
}

const sad::Point2D& sad::animations::Instance::basicCenter() const
{
	return m_basic_center;
}

const sad::Rect2D& sad::animations::Instance::basicArea() const
{
	return m_basic_area;
}

// ================================== PROTECTED METHODS ==================================


double sad::animations::Instance::computeTime(sad::animations::Animations* animations)
{
    double elapsed = m_start_time + m_timer.elapsed();
    double result = elapsed;

    sad::animations::Animation* a = m_animation.get();
    // If time is expired, animation is or looped, or should be stopped
    if (elapsed > a->time())
    {
        if (a->looped())
        {
            double t = elapsed - floor(elapsed / a->time()) * a->time();
            result = t;
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
            else
            {
                result = m_start_time;
            }
        }
    }
    return result;
}

void sad::animations::Instance::processTime(sad::animations::Animations* animations, double time)
{
    sad::animations::Animation* a = m_animation.get();
    a->setState(this, time);
}

void sad::animations::Instance::start(sad::animations::Animations* animations)
{
    this->checkIfValid(animations);
    if (m_valid)
    {
		sad::animations::Animation* a = m_animation.get();
        this->saveStateAndCompile(animations);
		a->start(this);
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

    const sad::animations::SavedObjectStateCreators& creators = a->creators();
    for(size_t i = 0; i < creators.size(); i++) {
        if (animations->cache().lookup(o, creators[i]->name()) == false)
        {
			sad::animations::SavedObjectState* state = creators[i]->create(o);
			if (m_body)
			{
				state->storeBodyState(m_body);
			}
            animations->cache().saveState(o, creators[i]->name(), state);
        }
        else
        {
            animations->cache().increment(o, creators[i]->name());
        }
    }

    this->setStateCommand(a->stateCommand(o));
}

void sad::animations::Instance::restoreObjectState(sad::animations::Animations* animations)
{
    const sad::animations::SavedObjectStateCreators& creators = m_animation.get()->creators();
    for(size_t i = 0; i < creators.size(); i++) {
        animations->cache().restore(m_object.get(), creators[i]->name());
    }
}

void sad::animations::Instance::clearSetState()
{
	delete m_state_command;
	m_state_command = NULL;
}
