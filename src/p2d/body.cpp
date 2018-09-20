#include "p2d/body.h"
#include "p2d/world.h"
#include "p2d/circle.h"
#include "p2d/line.h"
#include <cstdio>

DECLARE_SOBJ(sad::p2d::Body);


double sad::p2d::Body::timeStep() const
{
    return TimeStep;
}

void sad::p2d::Body::notifyRotate(const double & delta)
{
    m_current->rotate(delta);
}

void sad::p2d::Body::notifyMove(const sad::p2d::Vector & delta)
{
    m_current->move(delta);
}

const sad::Vector<sad::Object*>& sad::p2d::Body::userObjects() const
{
    return m_user_objects;
}

void sad::p2d::Body::setUserObject(sad::Object * o)
{
    if (m_user_objects.size())
    {
        for(size_t i = 0; i < m_user_objects.size(); i++)
        {
            m_user_objects[i]->delRef();
        }
    }
    m_user_objects.clear();
    //printf("Set user object %p on body %p\n", o, this);
    if (o)
    {
        m_user_objects << o;
        o->addRef();
    }
}


sad::Object * sad::p2d::Body::userObject() const
{
    if (m_user_objects.size() == 0)
    {
        return NULL;
    }
    return m_user_objects[0];
}

const sad::String & sad::p2d::Body::userType() const
{
    if (m_user_objects.size() == 0)
    {
        return this->metaData()->name();
    }
    return m_user_objects[0]->metaData()->name();
}

sad::p2d::CollisionShape & sad::p2d::Body::at(double time, int index) const
{
    sad::p2d::Body * me = const_cast<sad::p2d::Body *>(this);

    // Light optimization, because most of our collision shapes are POD-like structures
    // We can reduce amount of allocations, using untyped copying instead all of high-level
    // operations
    memcpy(me->Temporary + index, me->m_current, me->m_shapesize);

    (me->Temporary + index)->move(m_tangential->positionDelta(time, me->TimeStep));
    (me->Temporary + index)->rotate(m_angular->positionDelta(time, me->TimeStep));
    return *(me->Temporary + index);
}

void sad::p2d::Body::stepDiscreteChangingValues(double time)
{
    m_tangential->forces().step(time);
    m_angular->forces().step(time);
}

void sad::p2d::Body::stepPositionsAndVelocities(double time)
{
    m_tangential->step(time, this->timeStep());
    m_angular->step(time, this->timeStep());

    m_last_collision.clear();
}

void sad::p2d::Body::trySetTransformer()
{
    if (m_world)
    {
        if (this->m_current->metaIndex() == sad::p2d::Circle::globalMetaIndex())
        {
            p2d::Circle * c = static_cast<p2d::Circle*>(m_current);
            c->setTransformer(m_world->transformer());
        }
    }
}

sad::p2d::Body::Body() : m_is_ghost(false), m_world(NULL)
{    
    m_tangential = new p2d::TangentialMovement();
    m_tangential->addListener( new move_t(this, &p2d::Body::notifyMove) );
    m_tangential->setWeight(&m_weight);
    m_tangential->setBody(this);

    m_angular = new p2d::AngularMovement();
    m_angular->addListener( new rotate_t(this, &p2d::Body::notifyRotate) );
    m_angular->setWeight(&m_weight);
    m_angular->setBody(this);

    p2d::Line * l = new p2d::Line();
    l->setCutter(p2d::cutter(0,0,0,0));
    m_current =  l;

    Temporary = NULL;
    m_lastsampleindex = -1;
    m_samples_are_cached = false;

    m_fixed = false;
    this->TimeStep = 0.0;
}

sad::p2d::Body::~Body()
{
    //printf("Destroying body %p with %p\n", this, m_user_object);
    delete m_tangential;
    delete m_angular;
    delete m_current;
    delete[] Temporary;
    if (m_user_objects.size())
    {
        for(size_t i = 0; i < m_user_objects.size(); i++)
        {
            m_user_objects[i]->delRef();
        }
    }
}


void sad::p2d::Body::setWeight(const sad::p2d::Weight & weight)
{
    m_weight = weight;
}

const sad::p2d::Weight & sad::p2d::Body::weight() const
{
    return m_weight;
}

void sad::p2d::Body::setIsGhost(bool value)
{
    m_is_ghost = value;
}

bool sad::p2d::Body::isGhost() const
{
    return m_is_ghost;
}

void sad::p2d::Body::setWorld(sad::p2d::World * world)
{
    m_world = world;
    this->trySetTransformer();
}

sad::p2d::World * sad::p2d::Body::world()
{
    return m_world;
}

void sad::p2d::Body::setShape(sad::p2d::CollisionShape * shape)
{
    delete m_current;
    m_current = shape;
    this->trySetTransformer();
    m_current->move(this->m_tangential->position());
    m_current->rotate(this->m_angular->position());
    m_shapesize = m_current->sizeOfType();

    delete Temporary;
    Temporary = NULL;
    if (m_lastsampleindex > -1)
        Temporary = m_current->clone(m_lastsampleindex + 1);    
}


void sad::p2d::Body::initAngle(double angle) const
{
    m_angular->initPosition(angle);
}

void sad::p2d::Body::initPosition(const p2d::Point& p) const
{
    m_tangential->initPosition(p);
}


void sad::p2d::Body::setCurrentPosition(const sad::p2d::Point & p)
{
    m_tangential->setCurrentPosition(p);
    buildCaches();
}

void sad::p2d::Body::shedulePosition(const sad::p2d::Point & p)
{
    m_tangential->setNextPosition(p);
    buildCaches();
}

void sad::p2d::Body::shedulePositionAt(const sad::p2d::Point & p, double time)
{
    m_tangential->setNextPositionAt(p, time);
    buildCaches();
}


const sad::p2d::Vector & sad::p2d::Body::position() const
{
    return m_tangential->position();
}

bool sad::p2d::Body::willPositionChange() const
{
    return m_tangential->willPositionChange();
}

sad::p2d::Vector sad::p2d::Body::nextPosition() const
{
    return m_tangential->nextPosition();
}

void sad::p2d::Body::setCurrentTangentialVelocity(const p2d::Vector & v)
{
    m_tangential->setCurrentVelocity(v);
    buildCaches();
}

void sad::p2d::Body::sheduleTangentialVelocity(const p2d::Vector & v)
{
    m_tangential->setNextVelocity(v);
}

void sad::p2d::Body::sheduleTangentialVelocityAt(const p2d::Vector & v, double time)
{
    m_tangential->setNextVelocityAt(v, time);
}

const sad::p2d::Vector & sad::p2d::Body::tangentialVelocity() const
{
    return m_tangential->velocity();
}

bool sad::p2d::Body::willTangentialVelocityChange() const
{
    return m_tangential->willVelocityChange();
}

sad::p2d::Vector sad::p2d::Body::nextTangentialVelocity() const
{
    return m_tangential->nextVelocity();
}

void sad::p2d::Body::setCurrentAngle(double angle)
{
    m_angular->setCurrentPosition(angle);
    buildCaches();
}

void sad::p2d::Body::sheduleAngle(double angle)
{
    m_angular->setNextPosition(angle);  
    buildCaches();
}

void sad::p2d::Body::sheduleAngleAt(double angle, double time)
{
    m_angular->setNextPositionAt(angle, time);
    buildCaches();
}

double sad::p2d::Body::angle() const
{
    return m_angular->position();
}

bool sad::p2d::Body::willAngleChange() const
{
    return m_angular->willPositionChange();
}

double sad::p2d::Body::nextAngle() const
{
    return m_angular->nextPosition();
}

void sad::p2d::Body::setCurrentAngularVelocity(double v)
{
    m_angular->setCurrentVelocity(v);
    buildCaches();
}

void sad::p2d::Body::sheduleAngularVelocity(double v)
{
    m_angular->setNextVelocity(v);
}

void sad::p2d::Body::sheduleAngularVelocityAt(double v, double time)
{
    m_angular->setNextVelocityAt(v, time);
}

double sad::p2d::Body::angularVelocity() const
{
    return m_angular->velocity();
}

double sad::p2d::Body::angularVelocityAt(double time) const
{
    return m_angular->velocityAt(time, this->timeStep());
}

bool sad::p2d::Body::willAngularVelocityChange() const
{
    return m_angular->willVelocityChange();
}

double sad::p2d::Body::nextAngularVelocity() const
{
    return m_angular->nextVelocity();
}

void sad::p2d::Body::move(const p2d::Vector & v)
{
    return m_tangential->setCurrentPosition(m_tangential->position() + v);
}


void sad::p2d::Body::rotate(double delta)
{
    return m_angular->setCurrentPosition(m_angular->position() + delta);
}


sad::p2d::CollisionShape * sad::p2d::Body::currentShape()
{
    return m_current;
}

void sad::p2d::Body::clearMoveListeners()
{
    m_tangential->clearListeners();
}

void sad::p2d::Body::clearRotateListeners()
{
    m_angular->clearListeners();
}

void sad::p2d::Body::clearListeners()
{
    this->clearMoveListeners();
    this->clearRotateListeners();
}

void sad::p2d::Body::addForce(sad::p2d::Force<sad::p2d::Vector>* force)
{
    m_tangential->forces().add(force);
}

void sad::p2d::Body::addForce(sad::p2d::Force<double>* force)
{
    m_angular->forces().add(force);
}

void sad::p2d::Body::sheduleAddForce(sad::p2d::Force<sad::p2d::Vector>* force, double time)
{
    m_tangential->forces().scheduleAdd(force, time);
}

void sad::p2d::Body::sheduleAddForce(sad::p2d::Force<double>* force, double time)
{
    m_angular->forces().scheduleAdd(force, time);
}

void sad::p2d::Body::sheduleAddForce(sad::p2d::Force<sad::p2d::Vector>* force)
{
    m_tangential->forces().scheduleAdd(force);
}

void sad::p2d::Body::sheduleAddForce(sad::p2d::Force<double>* force)
{
    m_angular->forces().scheduleAdd(force);
}

void sad::p2d::Body::removeForce(sad::p2d::Force<sad::p2d::Vector>* force)
{
    m_tangential->forces().remove(force);
}

void sad::p2d::Body::removeForce(sad::p2d::Force<double>* force)
{
    m_angular->forces().remove(force);
}

void sad::p2d::Body::clearTangentialForces()
{
    m_tangential->forces().clear();
}

void sad::p2d::Body::clearAngularForces()
{
    m_angular->forces().clear();
}

void sad::p2d::Body::clearForces()
{
    this->clearTangentialForces();
    this->clearAngularForces();
}

const sad::Vector<sad::p2d::Force<sad::p2d::Vector>* >&  sad::p2d::Body::tangentialForcesList()
{
    return m_tangential->forces().forces();
}

const sad::Vector<sad::p2d::Force<double>* >& sad::p2d::Body::angularForcesList()
{
    return m_angular->forces().forces();
}

sad::p2d::TangentialActingForces & sad::p2d::Body::tangentialForces()
{
    return m_tangential->forces();
}

sad::p2d::AngularActingForces & sad::p2d::Body::angularForces()
{
    return m_angular->forces();
}

sad::p2d::Vector sad::p2d::Body::averageChangeIndependentTangentialVelocity()
{
    return m_tangential->averageChangeIndependentVelocityPer(this->timeStep());
}

sad::p2d::Vector sad::p2d::Body::tangentialVelocityAt(double time)
{
    return m_tangential->velocityAt(time, this->timeStep());
}

void sad::p2d::Body::buildCaches()
{
    m_tangential->cacheAcceleration();
    m_angular->cacheAcceleration();
    double t = this->TimeStep;
    int k = m_lastsampleindex + 1;
    double slice = t / k;
    for(int i = 0; i < k; i++)
    {
        // Saves inner data, using at. After that, caches results can be used by 
        // any kind of detector to build data
        this->at(t * (i+1), i );
    }
}


sad::p2d::Weight & sad::p2d::Body::weight()
{
    return m_weight;
}


void sad::p2d::Body::correctPosition(const sad::p2d::Vector & distance)
{
    if (this->willPositionChange())
    {
        sad::p2d::Vector olddistance = this->nextPosition() - this->position();
        
        double distancemodulo = p2d::modulo(distance);
        double olddistancemodulo = p2d::modulo(olddistance);
        if (distancemodulo < olddistancemodulo) 
        {
            this->shedulePosition(this->position() + distance);
        }
    }   
    else
    {
        sad::p2d::Vector position = this->position();
        position += distance;
        this->shedulePosition(position);
    }
}

void sad::p2d::Body::correctTangentialVelocity(const p2d::Vector & v)
{
    if (this->willTangentialVelocityChange())
    {
        // Merge two impulses into one
        sad::p2d::Vector currentv = this->tangentialVelocity();
        sad::p2d::Vector impulse = this->nextTangentialVelocity();
        impulse -= currentv;

        impulse += v - currentv;
            
        // Here sum of speeds is computed
        impulse +=  this->tangentialVelocity();
        this->sheduleTangentialVelocity(impulse);
    }
    else
    {
        this->sheduleTangentialVelocity(v);
    }
}


void sad::p2d::Body::setSamplingCount(int samples)
{   
    delete Temporary;
    Temporary = m_current->clone(samples);
    m_lastsampleindex = samples - 1;
}

void sad::p2d::Body::buildCaches(double time_step)
{
    this->TimeStep = time_step;
    this->buildCaches();
}

sad::Maybe<sad::Pair<double, sad::p2d::Vector> >& sad::p2d::Body::lastCollision()
{
    return m_last_collision;
}

