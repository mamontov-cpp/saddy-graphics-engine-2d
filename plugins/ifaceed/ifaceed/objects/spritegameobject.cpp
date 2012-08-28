#include "spritegameobject.h"
#include <log.h>

SpriteGameObject * createSpriteGameObject(const hst::string & spritegroup, int index, 
										  const hPointF & p,
										  Sprite2DConfigObserverPolicy policy 
										 )
{
	SpriteGameObject * result=new SpriteGameObject(spritegroup,index,GlobalSpritesConfig,policy);
	if (result->createSprite(p)==false)
	{
		hst::log::inst()->write(hst::string("Can't create a sprite from group"))
						 .write(spritegroup).write(hst::string(":"))
						 .write(index).write(hst::string("\n"));
		delete result;
		result=NULL;
	}
	return result;
}

SpriteGameObject::SpriteGameObject(const hst::string & spritegroup, int index, 
							       Sprite2DConfig * config,
								   Sprite2DConfigObserverPolicy policy
						           )
{
	m_observer=new Sprite2DConfigObserver(spritegroup,index,config,policy);
}

bool SpriteGameObject::createSprite(const hPointF & p)
{
	return m_observer->createSprite(p);
}

SpriteGameObject::~SpriteGameObject()
{
	delete m_observer;
}

//void SpriteGameObject::addToScene(InterlockedScene * scene)
//{
//	this->AbstractGameObject::addToScene(scene);
//	sprite()->addToScene(scene);
//}

//void SpriteGameObject::removeFromScene()
//{
//	this->AbstractGameObject::removeFromScene();
//	sprite()->removeFromScene();
//}


