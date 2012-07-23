#include "abstractgameobject.h"

AbstractGameObject::~AbstractGameObject()
{
}


void AbstractGameObject::addToScene(InterlockedScene * scene)
{
	scene->add<AbstractGameObject *>();
	scene->get<AbstractGameObject *>()->add(this);
	m_scene=scene;
}

void AbstractGameObject::removeFromScene()
{
	m_scene->get<AbstractGameObject *>()->remove(this);
}

