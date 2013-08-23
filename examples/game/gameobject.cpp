#include "gameobject.h"
#include "game.h"

DECLARE_SOBJ_INHERITANCE(GameObject, sad::BasicNode);


GameObject::GameObject()
{
	// We don't init sprite as valid, to preserve working with simple
	// objects
	m_sprite = new Sprite2DAdapter(NULL, hRectF(), hRectF());
	m_body = new p2d::Body();
	
	// Add listeners, needed to synchronize sprite and a game object
	m_body->addMoveListener( 
		new p2d::MovementDeltaListener<GameObject, p2d::Vector>(
			this, 
			&GameObject::notifyMove
		)
	);
	m_body->addRotateListener(
		new p2d::MovementDeltaListener<GameObject, double>(
			this, 
			&GameObject::notifyRotate
		)
	);

	m_hp = 1;
}

GameObject::~GameObject()
{
	if (m_game == NULL)
	{
		delete m_body;
	}
	for(size_t i = 0; i < m_guns.count(); i++)
	{
		delete m_guns[i];
	}
	delete m_sprite;
}


void GameObject::setGame(Game * g)
{
	m_game = g;
}

void GameObject::notifyMove(const p2d::Vector & dist)
{
	m_sprite->move(dist);
}

void GameObject::notifyRotate(const double & angle)
{
	m_sprite->rotate(angle);
}


void GameObject::render()
{
	m_sprite->render();
	for(size_t i = 0; i < m_guns.count(); i++)
	{
		m_guns[i]->tryShoot();;
	}
}


void GameObject::setAngularVelocity(double v)
{
	m_body->setCurrentAngularVelocity(v);
}

void GameObject::setHorizontalSpeed(double v)
{
	p2d::Vector velocity = m_body->tangentialVelocity();
	velocity.setX(v);
	m_body->setCurrentTangentialVelocity(velocity);
}

void GameObject::setVerticalSpeed(double v)
{
	p2d::Vector velocity = m_body->tangentialVelocity();
	velocity.setY(v);
	m_body->setCurrentTangentialVelocity(velocity);
}

void GameObject::stopHorizontal()
{
	p2d::Vector velocity = m_body->tangentialVelocity();
	velocity.setX(0);
	m_body->setCurrentTangentialVelocity(velocity);
}

void GameObject::stopVertical()
{
	p2d::Vector velocity = m_body->tangentialVelocity();
	velocity.setY(0);
	m_body->setCurrentTangentialVelocity(velocity);
} 

void GameObject::stop()
{
	m_body->setCurrentTangentialVelocity(p2d::Vector(0,0));	
}


p2d::Body * GameObject::body()
{
	return m_body;
}

void GameObject::teleportNow(const p2d::Point & p)
{
	m_body->setCurrentPosition(p);
}

void GameObject::teleportLater(const p2d::Point & p)
{
	m_body->shedulePosition(p);
}


int GameObject::hitPoints() const
{
    return m_hp;
}

void GameObject::incrementHP(int count)
{
    m_hp += count;
}

void GameObject::decrementHP(int count)
{
	m_hp -= count;

	// If hp less than zero - remove object
	if (m_hp <= 0 && m_game != NULL)
	{
		m_game->removeObject(this);
	}
}


void GameObject::setAngle(double angle)
{
	this->m_body->setCurrentAngle(angle);
}

void GameObject::lookAt(const hPointF & p)
{
	hPointF c = p - this->m_body->position();
	double angle = angle_of(c.x(), c.y());
	// We roate it counter-clockwise, because object looks to pi
	setAngle(M_PI + angle);
}


Game * GameObject::game()
{
	return m_game;
}

p2d::Point GameObject::position() const
{
	return m_body->position();
}

double GameObject::angle() const
{
	return m_body->angle();
}

void GameObject::addGun(AbstractAutomaticGun * gun)
{
	m_guns << gun;
	gun->setObject(this);
}
