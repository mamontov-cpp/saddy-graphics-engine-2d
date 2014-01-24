#include "sprite2dadapter.h"
#include "texturemanager.h"
#include "geometry2d.h"

DECLARE_SOBJ_INHERITANCE(Sprite2DAdapter,sad::SceneNode)

Sprite2DAdapter::Sprite2DAdapter(sad::Texture * tex,const sad::Rect2D & texrect,const sad::Rect2D & bbox)
{
	sad::Point3D p1(bbox[0].x(),bbox[0].y(), 0.0f);
	sad::Point3D p2(bbox[2].x(),bbox[2].y(), 0.0f);
	sad::Rect< ::sad::Point3D > rect(p1, p2);
	m_sprite = new sad::Sprite3D(tex, texrect, rect);
}

Sprite2DAdapter::Sprite2DAdapter(const Sprite2DAdapter & sprite)
{
	m_sprite = new sad::Sprite3D(*(sprite.m_sprite));
}
Sprite2DAdapter::~Sprite2DAdapter()
{
	delete m_sprite;
}

void Sprite2DAdapter::setPos(const sad::Point2D & p)
{
	m_sprite->moveTo(sad::Point3D(p));
}

void Sprite2DAdapter::move(const sad::Point2D & p)
{
	m_sprite->moveBy(sad::Point3D(p));
}

sad::Point2D Sprite2DAdapter::pos() const
{
	return m_sprite->middle();
}

void Sprite2DAdapter::rotate(float angle)
{
	m_sprite->rotate(angle,0);
}

float Sprite2DAdapter::angle() const
{
	return m_sprite->alpha();
}



void Sprite2DAdapter::flipX()
{
	m_sprite->setFlipX(!(m_sprite->flipX()));
}


void Sprite2DAdapter::flipY()
{
	m_sprite->setFlipY(!(m_sprite->flipY()));
}

bool Sprite2DAdapter::isFlippedX() const
{
	return m_sprite->flipX();
}

bool Sprite2DAdapter::isFlippedY() const
{
	return m_sprite->flipY();
}


sad::Point2D Sprite2DAdapter::size() const
{
	return sad::Point2D(m_sprite->area().width(),m_sprite->area().height());
}

void Sprite2DAdapter::setSize(const sad::Point2D & size)
{
  m_sprite->setSize(sad::Size2D(size.x(), size.y()));
}

void Sprite2DAdapter::render()
{
	m_sprite->render();
}

void Sprite2DAdapter::setSprite(sad::Texture * tex, const sad::Rect2D & texrect)
{
   m_sprite->setTexture(tex);
   m_sprite->setTextureCoordinates(texrect);
}

const sad::Rect2D & Sprite2DAdapter::rect() const
{
	const_cast<sad::Rect2D&>(m_buffer) = 	sad::Rect2D(
		m_sprite->renderableArea()[0],
		m_sprite->renderableArea()[1],
		m_sprite->renderableArea()[2],
		m_sprite->renderableArea()[3]
	);
	return m_buffer;
}

void Sprite2DAdapter::setRect(const sad::Rect2D & rect)
{
	sad::Rect< ::sad::Point3D> vrect;
	for(int i = 0; i < 4; i++)
	{
		vrect[i] = ::sad::Point3D((float)(rect[i].x()), (float)(rect[i].y()), 0.0f);
	}
	m_sprite->setRenderableArea(vrect);
}

void Sprite2DAdapter::setColor(const sad::AColor & color)
{
	m_sprite->setColor(color);
}

const sad::AColor & Sprite2DAdapter::color() const 
{
	return m_sprite->color();
}

void Sprite2DAdapter::set(const Sprite2DAdapter::Options & o)
{

	sad::Texture * tex = NULL;
	if (o.TextureContainer.exists())
	{
		sad::TextureContainer * c =  
			sad::TextureManager::ref()->getContainer(o.TextureContainer.value());
		tex = c->get(o.Texture);
	}
	else
	{
		tex  = sad::TextureManager::ref()->get(o.Texture);
	}
	this->setSprite(tex, o.TextureRectangle);
	this->setRect(o.Rectangle);
}


void Sprite2DAdapter::makeSpanBetweenPoints(const sad::Rect2D & r, const sad::Point2D & p1, const sad::Point2D & p2)
{
	sad::Rect2D kr(r);
	for(int i = 0; i < 4; i++)
		kr[i].setX(0);

	double distance = p1.distance(p2) / 2;
	kr[0].setX(kr[0].x() - distance );
	kr[2].setX(kr[2].x() + distance );
	kr[1].setX(kr[1].x() + distance );
	kr[3].setX(kr[3].x() - distance );

	this->rotate(this->angle() * -1);
	this->setRect(kr);
	this->move( (p1 +p2) / 2);
	double angle = atan2(p2.y() - p1.y(), p2.x() - p1.x());
	this->rotate(angle);
}
