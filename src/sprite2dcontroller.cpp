#include "sprite2dcontroller.h"

Sprite2DController::Sprite2DController(sad::Texture * tex,const hRectF & texrect,const hRectF & bbox)
{
	m_sprite=new Sprite2DAdapter(tex,texrect,bbox);
	m_sizechanged=false;
	m_scene=NULL;
}

Sprite2DController::~Sprite2DController()
{
   if (m_scene==NULL)
		delete m_sprite;
   else
	   m_scene->markForDeletion(m_sprite);
}

void Sprite2DController::addToScene(sad::Scene * scene)
{
	m_scene=scene;
	scene->markForAddition(m_sprite);
}

void Sprite2DController::removeFromScene()
{
	Sprite2DAdapter * newsprite=new Sprite2DAdapter(*m_sprite);
	m_scene->markForDeletion(m_sprite);
	m_scene=NULL;
	m_sprite=newsprite;
}

bool Sprite2DController::wasSizeChanged() const
{
	return m_sizechanged;
}


void Sprite2DController::setPos(const hPointF & p)
{
	m_sprite->setPos(p);
}

void Sprite2DController::move(const hPointF & p)
{
	m_sprite->move(p);
}

hPointF Sprite2DController::pos() const
{
	return m_sprite->pos();
}

void Sprite2DController::rotate(float angle)
{
	m_sprite->rotate(angle);
}

float Sprite2DController::angle() const
{
	return m_sprite->angle();
}

void Sprite2DController::flipX()
{
	m_sprite->flipX();
}

void Sprite2DController::flipY()
{
	m_sprite->flipY();
}

bool Sprite2DController::isFlippedX() const
{
	return m_sprite->isFlippedX();
}

bool Sprite2DController::isFlippedY() const
{
	return m_sprite->isFlippedY();
}

void Sprite2DController::setLeft(float x)
{
	m_sizechanged=true;
	m_sprite->setLeft(x);
}

void Sprite2DController::setRight(float x)
{
	m_sizechanged=true;
	m_sprite->setRight(x);
}

void Sprite2DController::setTop(float y)
{
	m_sizechanged=true;
	m_sprite->setTop(y);
}

void Sprite2DController::setBottom(float y)
{
	m_sizechanged=true;
	m_sprite->setBottom(y);
}

void Sprite2DController::setSize(const hPointF & size, bool registerEvent)
{
	if (registerEvent)
		m_sizechanged=true;
	m_sprite->setSize(size);
}

hPointF Sprite2DController::size() const
{
	return m_sprite->size();
}

void Sprite2DController::setSprite(sad::Texture * tex, const hRectF & texrect)
{
	m_sprite->setSprite(tex,texrect);
}

