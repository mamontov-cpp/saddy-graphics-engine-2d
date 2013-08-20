#include "p2d/body.h"
#include "p2d/world.h"

DECLARE_SOBJ(p2d::Body);


double p2d::Body::timeStep() const
{
	if (m_world == NULL)
		return 1.0;
	return m_world->timeStep();
}

void p2d::Body::notifyRotate(double delta)
{
	m_current->rotate(delta);
}

void p2d::Body::notifyMove(const p2d::Vector & delta)
{
	m_current->move(delta);
}

p2d::Body::~Body()
{
	delete m_weight;
	delete m_central;
	delete m_tangential;
	delete m_current;
	delete m_temporary;
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
	delete me->m_temporary;
	me->m_temporary = me->m_current->clone();
	me->m_temporary->move(m_central->positionDelta(time, me->timeStep()));
	me->m_temporary->rotate(m_tangential->positionDelta(time, me->timeStep()));
	return *(me->m_temporary);
}

void p2d::Body::stepDiscreteChangingValues()
{
	m_ghost->step();
	m_central->force()->step();
	m_tangential->force()->step();
}

void p2d::Body::stepPositionsAndVelocities(double time)
{
	m_central->step(time, this->timeStep());
	m_tangential->step(time, this->timeStep());
}

