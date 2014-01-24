#include "sprite2dcontroller.h"

Sprite2DController::Sprite2DController(sad::Texture * tex,const sad::Rect2D & texrect,const sad::Rect2D & bbox)
{
	m_sprite=new sad::Sprite2D(tex,texrect,bbox);
	m_sizechanged=false;
}

Sprite2DController::~Sprite2DController()
{

}



bool Sprite2DController::wasSizeChanged() const
{
	return m_sizechanged;
}


void Sprite2DController::setPos(const sad::Point2D & p)
{
	m_sprite->moveTo(p);
}

void Sprite2DController::move(const sad::Point2D & p)
{
	m_sprite->moveBy(p);
}

sad::Point2D Sprite2DController::pos() const
{
	return m_sprite->middle();
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
	return m_sprite->flipX();
}

bool Sprite2DController::isFlippedY() const
{
	return m_sprite->flipY();
}


void Sprite2DController::setSize(const sad::Point2D & size, bool registerEvent)
{
	if (registerEvent)
		m_sizechanged=true;
	m_sprite->setSize(sad::Size2D(size.x(), size.y()));
}

sad::Point2D Sprite2DController::size() const
{
	sad::Point2D result(m_sprite->size().Width, m_sprite->size().Height);
	return result;
}

void Sprite2DController::setSprite(sad::Texture * tex, const sad::Rect2D & texrect)
{
	m_sprite->setTexture(tex);
	m_sprite->setTextureCoordinates(texrect);
}

void Sprite2DController::setColor(const sad::AColor & clr)
{
	if (m_sprite)
		m_sprite->setColor(clr);
}

sad::AColor Sprite2DController::color() const
{
	if (m_sprite)	
		return m_sprite->color();
	return sad::AColor();
}

