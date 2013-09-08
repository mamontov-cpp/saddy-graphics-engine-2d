#include "p2d/body.h"
#include "p2d/world.h"
#include "p2d/circle.h"
#include "p2d/line.h"

DECLARE_SOBJ(p2d::Body);


double p2d::Body::timeStep() const
{
	if (m_world != NULL)
		return m_world->timeStep();
	return 1.0;
}

void p2d::Body::notifyRotate(const double & delta)
{
	m_current->rotate(delta);
}

void p2d::Body::notifyMove(const p2d::Vector & delta)
{
	m_current->move(delta);
}

void p2d::Body::setUserObject(sad::Object * o)
{
	m_user_object = o;
}


sad::Object * p2d::Body::userObject() const
{
	return m_user_object;
}

const hst::string & p2d::Body::userType() const
{
	if (m_user_object == NULL)
	{
		return this->metaData()->name();
	}
	return m_user_object->metaData()->name();
}

p2d::CollisionShape & p2d::Body::at(double time) const
{
	p2d::Body * me = const_cast<p2d::Body *>(this);

	// Light optimization, because most of our collision shapes are POD-like structures
	// We can reduce amount of allocations, using untyped copying instead all of high-level
	// operations
	memcpy(me->m_temporary, me->m_current, me->m_shapesize);

	me->m_temporary->move(m_tangential->positionDelta(time, me->timeStep()));
	me->m_temporary->rotate(m_angular->positionDelta(time, me->timeStep()));
	return *(me->m_temporary);
}

void p2d::Body::stepDiscreteChangingValues(double time)
{
	m_ghost->step();
	m_tangential->forces().step(time);
	m_angular->forces().step(time);
}

void p2d::Body::stepPositionsAndVelocities(double time)
{
	m_tangential->step(time, this->timeStep());
	m_angular->step(time, this->timeStep());
}

void p2d::Body::trySetTransformer()
{
	if (m_world)
	{
		if (this->m_current->metaData()->canBeCastedTo("p2d::Circle"))
		{
			p2d::Circle * c = hst::checked_cast<p2d::Circle>(m_current);
			c->setTransformer(m_world->transformer());
		}
	}
}

p2d::Body::Body()
{
	m_weight = new p2d::Weight();
	m_ghost = new p2d::GhostOptionsFlow();
	m_world = NULL;
	m_user_object = NULL;
	
	m_tangential = new p2d::TangentialMovement();
	m_tangential->addListener( new move_t(this, &p2d::Body::notifyMove) );
	m_tangential->setWeight(m_weight);

	m_angular = new p2d::AngularMovement();
	m_angular->addListener( new rotate_t(this, &p2d::Body::notifyRotate) );
	m_angular->setWeight(m_weight);

	p2d::Line * l = new p2d::Line();
	l->setCutter(p2d::cutter(0,0,0,0));
	m_current =  l;

	m_temporary = NULL;
}

p2d::Body::~Body()
{
	delete m_weight;
	delete m_tangential;
	delete m_angular;
	delete m_current;
	delete m_temporary;
}

void p2d::Body::setWeight(p2d::Weight * weight)
{
	delete m_weight;
	m_weight = weight;
	m_tangential->setWeight(m_weight);
	m_angular->setWeight(m_weight);
}

void p2d::Body::setWeight(const p2d::Weight & weight)
{
	*m_weight = weight;
}

const p2d::Weight & p2d::Body::weight() const
{
	return *m_weight;
}

void p2d::Body::setCurrentGO(p2d::GhostOptions * ghost)
{
	m_ghost->setCurrent(ghost);
}

void p2d::Body::sheduleGO(p2d::GhostOptions *next)
{
	m_ghost->push(next);	
}

bool p2d::Body::isGhost() const
{
	return m_ghost->value();
}

void p2d::Body::setWorld(p2d::World * world)
{
	m_world = world;
	this->trySetTransformer();
}

p2d::World * p2d::Body::world()
{
	return m_world;
}

void p2d::Body::setShape(p2d::CollisionShape * shape)
{
	delete m_current;
	m_current = shape;
	this->trySetTransformer();
	m_current->move(this->m_tangential->position());
	m_current->rotate(this->m_angular->position());
	
	delete m_temporary;
	m_temporary = m_current->clone();
	m_shapesize = m_current->sizeOfType();
}


void p2d::Body::setCurrentPosition(const p2d::Point & p)
{
	m_tangential->setCurrentPosition(p);
	m_tangential->cacheAcceleration();
}

void p2d::Body::shedulePosition(const p2d::Point & p)
{
	m_tangential->setNextPosition(p);
}

void p2d::Body::shedulePositionAt(const p2d::Point & p, double time)
{
	m_tangential->setNextPositionAt(p, time);
}


const p2d::Vector & p2d::Body::position() const
{
	return m_tangential->position();
}

bool p2d::Body::willPositionChange() const
{
	return m_tangential->willPositionChange();
}

p2d::Vector p2d::Body::nextPosition() const
{
	return m_tangential->nextPosition();
}

void p2d::Body::setCurrentTangentialVelocity(const p2d::Vector & v)
{
	m_tangential->setCurrentVelocity(v);
}

void p2d::Body::sheduleTangentialVelocity(const p2d::Vector & v)
{
	m_tangential->setNextVelocity(v);
}

void p2d::Body::sheduleTangentialVelocityAt(const p2d::Vector & v, double time)
{
	m_tangential->setNextVelocityAt(v, time);
}

const p2d::Vector & p2d::Body::tangentialVelocity() const
{
	return m_tangential->velocity();
}

bool p2d::Body::willTangentialVelocityChange() const
{
	return m_tangential->willVelocityChange();
}

p2d::Vector p2d::Body::nextTangentialVelocity() const
{
	return m_tangential->nextVelocity();
}

void p2d::Body::setCurrentAngle(double angle)
{
	m_angular->setCurrentPosition(angle);
	m_angular->cacheAcceleration();
}

void p2d::Body::sheduleAngle(double angle)
{
	m_angular->setNextPosition(angle);	
}

void p2d::Body::sheduleAngleAt(double angle, double time)
{
	m_angular->setNextPositionAt(angle, time);
}

double p2d::Body::angle() const
{
	return m_angular->position();
}

bool p2d::Body::willAngleChange() const
{
	return m_angular->willPositionChange();
}

double p2d::Body::nextAngle() const
{
	return m_angular->nextPosition();
}

void p2d::Body::setCurrentAngularVelocity(double v)
{
	m_angular->setCurrentVelocity(v);
}

void p2d::Body::sheduleAngularVelocity(double v)
{
	m_angular->setNextVelocity(v);
}

void p2d::Body::sheduleAngularVelocityAt(double v, double time)
{
	m_angular->setNextVelocityAt(v, time);
}

double p2d::Body::angularVelocity() const
{
	return m_angular->velocity();
}

double p2d::Body::angularVelocityAt(double time) const
{
	return m_angular->velocityAt(time, this->timeStep());
}

bool p2d::Body::willAngularVelocityChange() const
{
	return m_angular->willVelocityChange();
}

double p2d::Body::nextAngularVelocity() const
{
	return m_angular->nextVelocity();
}

void p2d::Body::move(const p2d::Vector & v)
{
	return m_tangential->setCurrentPosition(m_tangential->position() + v);
}


void p2d::Body::rotate(double delta)
{
	return m_angular->setCurrentPosition(m_angular->position() + delta);
}


p2d::CollisionShape * p2d::Body::currentShape()
{
	return m_current;
}


void p2d::Body::clearListeners()
{
	m_tangential->clearListeners();
	m_angular->clearListeners();
}

p2d::TangentialActingForces & p2d::Body::tangentialForces()
{
	return m_tangential->forces();
}

p2d::AngularActingForces & p2d::Body::angularForces()
{
	return m_angular->forces();
}

p2d::Vector p2d::Body::averageChangeIndependentTangentialVelocity()
{
	return m_tangential->averageChangeIndependentVelocityPer(this->timeStep());
}

p2d::Vector p2d::Body::tangentialVelocityAt(double time)
{
	return m_tangential->velocityAt(time, this->timeStep());
}

void p2d::Body::buildAccelerationCache()
{
	m_tangential->cacheAcceleration();
	m_angular->cacheAcceleration();
}


p2d::Weight & p2d::Body::weight()
{
	return *m_weight;
}


void p2d::Body::correctPosition(const p2d::Vector & distance)
{
	if (this->willPositionChange())
	{
		p2d::Vector position = distance;
		position += this->nextPosition() - this->position();
		position /= 2.0;
		this->shedulePosition(this->position() + position);
	}	
	else
	{
		p2d::Vector position = this->position();
		position += distance;
		this->shedulePosition(position);
	}
}

void p2d::Body::correctTangentialVelocity(const p2d::Vector & v)
{
	if (this->willTangentialVelocityChange())
	{
		// Merge two impulses into one
		p2d::Vector impulse = this->nextTangentialVelocity();
		impulse -= this->tangentialVelocity();

		impulse += v - this->tangentialVelocity();
			
		impulse /= 2.0;
		// Here sum of speeds is computed
		impulse +=  this->tangentialVelocity();
		this->sheduleTangentialVelocity(impulse);
	}
	else
	{
		this->sheduleTangentialVelocity(v);
	}
}


