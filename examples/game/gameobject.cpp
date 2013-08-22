#include "gameobject.h"

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

}

GameObject::~GameObject()
{
	if (m_game == NULL)
	{
		delete m_body;
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
}

