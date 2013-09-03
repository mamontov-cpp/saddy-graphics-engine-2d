#include "worldobject.h"
#include "world.h"

DECLARE_SOBJ_INHERITANCE(WorldObject, sad::BasicNode);

WorldObject::WorldObject()
{
	// We don't init sprite as valid, to preserve working with simple
	// objects
	m_sprite = new Sprite2DAdapter(NULL, hRectF(), hRectF());
	m_body = new p2d::Body();
	
	// Set self as user object to make type inference inside collisions
	// possible
	m_body->setUserObject(this);
	
	m_listener1 = new p2d::MovementDeltaListener<WorldObject, p2d::Vector>(
			this, 
			&WorldObject::notifyMove
	);
	m_listener2 = new p2d::MovementDeltaListener<WorldObject, double>(
			this, 
			&WorldObject::notifyRotate
	);
	// Add listeners, needed to synchronize sprite and a game object
	m_body->addMoveListener(m_listener1);
	m_body->addRotateListener(m_listener2);

}

WorldObject::~WorldObject()
{
	if (m_world == NULL)
	{
		delete m_body;
	}
	delete m_sprite;
}


void WorldObject::setWorld(World * g)
{
	m_world = g;
}

void WorldObject::notifyMove(const p2d::Vector & dist)
{
	m_sprite->move(dist);
}

void WorldObject::notifyRotate(const double & angle)
{
	m_sprite->rotate(angle);
}


void WorldObject::render()
{
	m_sprite->render();
}


void WorldObject::setAngularVelocity(double v)
{
	m_body->setCurrentAngularVelocity(v);
}

void WorldObject::setHorizontalSpeed(double v)
{
	p2d::Vector velocity = m_body->tangentialVelocity();
	velocity.setX(v);
	m_body->setCurrentTangentialVelocity(velocity);
}

void WorldObject::setVerticalSpeed(double v)
{
	p2d::Vector velocity = m_body->tangentialVelocity();
	velocity.setY(v);
	m_body->setCurrentTangentialVelocity(velocity);
}

void WorldObject::setTangentialVelocity(const p2d::Vector & v)
{
	m_body->setCurrentTangentialVelocity(v);
}

void WorldObject::stopHorizontal()
{
	p2d::Vector velocity = m_body->tangentialVelocity();
	velocity.setX(0);
	m_body->setCurrentTangentialVelocity(velocity);
}

void WorldObject::stopVertical()
{
	p2d::Vector velocity = m_body->tangentialVelocity();
	velocity.setY(0);
	m_body->setCurrentTangentialVelocity(velocity);
} 

void WorldObject::stop()
{
	m_body->setCurrentTangentialVelocity(p2d::Vector(0,0));	
}


p2d::Body * WorldObject::body()
{
	return m_body;
}




void WorldObject::setAngle(double angle)
{
	this->m_body->setCurrentAngle(angle);
}

void WorldObject::lookAt(const hPointF & p)
{
	hPointF c = p - this->m_body->position();
	double angle = angle_of(c.x(), c.y());
	// We roate it counter-clockwise, because object looks to pi
	setAngle(angle);
}


World * WorldObject::world()
{
	return m_world;
}

p2d::Point WorldObject::position() const
{
	return m_body->position();
}

double WorldObject::angle() const
{
	return m_body->angle();
}



void WorldObject::setPosition(const p2d::Point & p)
{
	m_body->setCurrentPosition(p);
}



