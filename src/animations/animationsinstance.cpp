#include "animations/animationsinstance.h"

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fuzzyequal.h>

#include <math.h>
#include <fstream>
#include "sadmutex.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/dbtable.h"

DECLARE_SOBJ(sad::animations::Instance);

sad::animations::Instance::Instance()
: m_paused(false),
m_started(false),
m_finished(false),
m_start_time(0),
m_state_command(NULL),
m_state_command_own(false),
m_valid(true),
m_shape(NULL),
m_body(NULL),
m_tree_link_active(true),
m_object_referenced(false)
{

}

sad::animations::Instance::Instance(const sad::animations::Instance& o)
: m_object(o.m_object),
m_animation(o.m_animation),
m_animation_db_link(o.m_animation_db_link),
m_tree_link_active(o.m_tree_link_active),
m_start_time(o.m_start_time),
m_paused(o.m_paused),
m_started(o.m_started),
m_finished(o.m_finished),
m_state_command(o.m_state_command),
m_state_commands(o.m_state_commands),
m_state_command_own(o.m_state_command_own),
m_valid(o.m_valid),
m_shape(o.m_shape),
m_body(o.m_body),
m_basic_area(o.m_basic_area),
m_basic_center(o.m_basic_center),
m_basic_string(o.m_basic_string),
m_object_referenced(o.m_object_referenced)
{
    copyState(o);
}


sad::animations::Instance& sad::animations::Instance::operator=(const sad::animations::Instance& o)
{
    m_object = o.m_object;
    m_animation = o.m_animation;
    m_animation_db_link = o.m_animation_db_link;
    m_tree_link_active = o.m_tree_link_active;
    m_start_time = o.m_start_time;
    m_paused = o.m_paused;
    m_started = o.m_started;
    m_finished = o.m_finished;
    m_state_command = o.m_state_command;
    m_state_commands = o.m_state_commands;
    m_state_command_own = o.m_state_command_own;
    m_valid = o.m_valid;
    m_shape = o.m_shape;
    m_body = o.m_body;
    m_basic_area = o.m_basic_area;
    m_basic_center = o.m_basic_center;
    m_basic_string = o.m_basic_string;
    m_object_referenced = o.m_object_referenced;
    copyState(o);
    return *this;
}

sad::animations::Instance::~Instance()
{
    if (m_object_referenced)
    {
        m_object.get()->delRef();
    }
	if (m_state_command_own) 
	{
		delete m_state_command;
	}
    if (m_body)
    {
        m_body->delRef();
    }
	for(size_t i = 0; i < m_state_commands.size(); i++)
	{
		delete m_state_commands[i];
	}
    delete m_shape;
}

static sad::db::schema::Schema* AnimationInstanceSchema = NULL;

static sad::Mutex AnimationInstanceSchemaLock;

sad::db::schema::Schema* sad::animations::Instance::basicSchema()
{
    if (AnimationInstanceSchema == NULL)
    {
        AnimationInstanceSchemaLock.lock();
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
                "animationmajorid",
                new sad::db::MethodPair<sad::animations::Instance, unsigned long long>(
                    &sad::animations::Instance::animationMajorId,
                    &sad::animations::Instance::setAnimationMajorId
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
        AnimationInstanceSchemaLock.unlock();
    }
    return AnimationInstanceSchema;
}

sad::db::schema::Schema* sad::animations::Instance::schema() const
{
    return sad::animations::Instance::basicSchema();
}

void sad::animations::Instance::setTreeName(sad::Renderer* r, const sad::String & tree_name)
{
    this->sad::Object::setTreeName(r, tree_name);
    m_animation.setTree(r, tree_name);
}

void sad::animations::Instance::setTable(sad::db::Table *t)
{
    this->sad::Object::setTable(t);
    m_object.setDatabase(t->database());
	m_animation_db_link.setTable(t);
	m_animation_db_link.setDatabase(t->database());
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
	m_tree_link_active = true;
}

static const int validtypescount = 17;
static sad::String validtypes[] = {
	"sad::animations::Animation",      // 1
	"sad::animations::Blinking",       // 2
	"sad::animations::CameraRotation", // 3
	"sad::animations::CameraShaking",  // 4
	"sad::animations::Color",          // 5
	"sad::animations::FontList",       // 6
	"sad::animations::FontSize",       // 7
	"sad::animations::OptionList",     // 8
	"sad::animations::Parallel",       // 9
	"sad::animations::Resize",         // 10
	"sad::animations::Rotate",         // 11
    "sad::animations::SimpleMovement", // 12
	"sad::animations::Sequential",     // 13
	"sad::animations::TextureCoordinatesContinuous",// 14
	"sad::animations::TextureCoordinatesList",      // 15
	"sad::animations::Typing",                      // 16
	"sad::animations::WayMoving"                    // 17
};

sad::animations::Animation* sad::animations::Instance::animation(bool check) const
{
	sad::animations::Animation* result = NULL;
	if (m_tree_link_active)
	{
		result = m_animation.get();
	}
	else
	{
		sad::db::Object* o = const_cast<sad::animations::Instance*>(this)->m_animation_db_link.get();
		if (o)
		{
			if (check)
			{
				bool valid = false;
				for(size_t i  = 0; (i < validtypescount) && (valid == false); i++)
				{
					valid = valid || o->isInstanceOf(validtypes[i]);
				}
				if (valid)
				{
					result = static_cast<sad::animations::Animation*>(o);
				}
			}
			else
			{
				result = static_cast<sad::animations::Animation*>(o);
			}
		}
	}
    return result;
}

void sad::animations::Instance::setObject(sad::db::Object* o)
{
    if (m_object_referenced)
    {
        m_object.get()->delRef();
    }
    m_object_referenced = false;
    m_object.setObject(o);
    if (o)
    {
        o->addRef();
        m_object_referenced = true;
    }
	this->clearSetState();
}


sad::db::Object* sad::animations::Instance::object() const
{
    sad::animations::Instance* i = const_cast<sad::animations::Instance*>(this);
    sad::db::Object* result = i->m_object.get();
    if (m_object_referenced == false && result)
    {
        i->m_object_referenced = true;
        result->addRef();
    }
    return result;
}

void sad::animations::Instance::setAnimationName(const sad::String& name)
{
    m_animation.setPath(name);
	this->clearSetState();
	m_tree_link_active = name.size() != 0;
}

const sad::String& sad::animations::Instance::animationName() const
{
	if (m_tree_link_active == false)
	{
		const_cast<sad::animations::Instance*>(this)->m_animation.setPath("");
		return m_animation.path();
	}
    return m_animation.path();
}

void sad::animations::Instance::setAnimationMajorId(unsigned long long majorid)
{
	m_animation_db_link.setMajorId(majorid);
	if (majorid > 0)
	{
		m_tree_link_active = false;
	}
}

unsigned long long sad::animations::Instance::animationMajorId()
{
	if (m_tree_link_active == false)
	{
		return m_animation_db_link.majorId();
	}
	return 0;
}

void sad::animations::Instance::setObjectId(unsigned long long id)
{
    if (m_object_referenced)
    {
        m_object.get()->delRef();
    }
    m_object.setMajorId(id);
    m_object_referenced = false;
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

void sad::animations::Instance::addCallbackOnStart(sad::animations::Callback* c)
{
    m_callbacks_on_start << c;    
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
    if (m_paused == false)
    {
        if (m_started == false)
        {
           this->start(animations);
           return;
        }

		if (m_valid)
		{
			double time = this->computeTime(animations);
			if (m_finished == false)
			{
				this->processTime(animations, time);
			}
		}
    }
	
	if (m_valid == false)
	{
		m_timer.stop();
		m_finished = true;
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
		m_started = false;
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

void sad::animations::Instance::setStateCommand(
	sad::animations::setstate::AbstractSetStateCommand* call,
	bool own
)
{
	if (m_state_command_own)
	{
		delete m_state_command;
	}
	m_state_command = call;
	m_state_command_own = own;
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
    if (m_body)
    {
        m_body->delRef();
    }
    m_body = body;
    delete m_shape;
    if (m_body)
    {
        m_body->addRef();
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

const sad::Rect2D& sad::animations::Instance::basicArea() const
{
	return m_basic_area;
}

const sad::Point2D& sad::animations::Instance::basicCenter() const
{
	return m_basic_center;
}

void  sad::animations::Instance::setBasicString(const sad::String& s)
{
	m_basic_string = s;
}

const sad::String& sad::animations::Instance::basicString() const
{
	return m_basic_string;
}

void sad::animations::Instance::setStateCommands(
	const sad::Vector<sad::animations::setstate::AbstractSetStateCommand*>& c
)
{
	for(size_t i = 0; i < m_state_commands.size(); i++)
	{
		delete m_state_commands[i];
	}
	m_state_commands = c;
}

const sad::Vector<sad::animations::setstate::AbstractSetStateCommand*>& 
sad::animations::Instance::stateCommands() 
const
{
	return m_state_commands;
}

void sad::animations::Instance::stopInstanceRelatedToObject(sad::db::Object* object, sad::animations::Animations* a)
{
	if (m_object.get() == object)
	{
		a->remove(this);
	}
}

bool sad::animations::Instance::isRelatedToObject(sad::db::Object* object)
{
	return m_object.get() == object;
}


// ================================== PROTECTED METHODS ==================================


double sad::animations::Instance::computeTime(sad::animations::Animations* animations)
{
    double elapsed = m_start_time + m_timer.elapsed();
    double result = elapsed;

    sad::animations::Animation* a = this->animation(false);
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

void sad::animations::Instance::processTime(sad::animations::Animations*, double time)
{
    sad::animations::Animation* a =  this->animation(false);
    a->setState(this, time);
}

void sad::animations::Instance::start(sad::animations::Animations* animations)
{
    this->checkIfValid(animations);
    if (m_valid)
    {
		sad::animations::Animation* a = this->animation(false);
        this->saveStateAndCompile(animations);
		a->start(this);
        a->setState(this, m_start_time);
        m_timer.start();
        m_started = true;
        fireOnStartCallbacks();
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

void sad::animations::Instance::checkIfValid(sad::animations::Animations*)
{
    sad::animations::Animation* a = this->animation(true);
    sad::db::Object* o = this->object();

    m_valid = (a && o);
    if (m_valid)
    {
		a->updateBeforePlaying();
        m_valid = a->valid() && a->applicableTo(o);
    }
}

void sad::animations::Instance::saveStateAndCompile(sad::animations::Animations* animations)
{
    sad::animations::Animation* a = this->animation(false);
    sad::db::Object* o = this->object();

    const sad::Vector<sad::animations::AbstractSavedObjectStateCreator*>& creators = a->creators();
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
	const sad::Vector<sad::animations::AbstractSavedObjectStateCreator*>& creators = this->animation(false)->creators();
    for(size_t i = 0; i < creators.size(); i++) {
        animations->cache().restore(this->object(), creators[i]->name());
    }
}

void sad::animations::Instance::clearSetState()
{
    if (m_state_command_own)
    {
	    delete m_state_command;
    }
	m_state_command = NULL;
}

void sad::animations::Instance::copyState(const sad::animations::Instance& o)
{
    if (o.m_shape)
    {
        m_shape = o.m_shape->clone(1);
    }
    if (o.m_started && !o.m_paused)
    {
        m_timer.start();
    }
    m_state_commands = o.m_state_commands;
    for(size_t i = 0; i < m_state_commands.size(); i++)
    {
        m_state_commands[i] = m_state_commands[i]->clone();
    }
    m_state_command_own = o.m_state_command_own;
    m_state_command = o.m_state_command;
    if (m_state_command_own && m_state_command)
    {
        m_state_command = m_state_command->clone();
    }
    for(size_t i = 0; i < m_callbacks_on_end.size(); i++)
    {
        delete m_callbacks_on_end[i];
    }
    m_callbacks_on_end.clear();
    for(size_t i = 0; i < o.m_callbacks_on_end.size(); i++)
    {
        m_callbacks_on_end << o.m_callbacks_on_end[i]->clone();
    }
    for(size_t i = 0; i < m_callbacks_on_start.size(); i++)
    {
        delete m_callbacks_on_start[i];
    }
    m_callbacks_on_start.clear();
    for(size_t i = 0; i < o.m_callbacks_on_start.size(); i++)
    {
        m_callbacks_on_start << o.m_callbacks_on_start[i]->clone();
    }
}

void sad::animations::Instance::fireOnStartCallbacks()
{
    for(size_t i = 0; i < m_callbacks_on_start.size(); i++)
    {
        m_callbacks_on_start[i]->invoke();        
    }
}
