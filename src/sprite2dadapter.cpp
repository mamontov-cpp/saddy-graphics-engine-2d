#include "sprite2dadapter.h"
#include "texturemanager.h"
#include "geometry2d.h"

DECLARE_SOBJ_INHERITANCE(Sprite2DAdapter,sad::BasicNode)


void Sprite2DAdapter::createSprite(sad::Texture * tex,const sad::Rect2D & texrect,const sad::Rect2D & bbox, 
								   bool normalize)
{
	delete m_sprite;
	m_rect=bbox;
	sad::Rect< ::sad::Point3D > rect(::sad::Point3D((float)(bbox[0].x()),(float)(bbox[0].y()),0.0f),
								 ::sad::Point3D((float)(bbox[2].x()),(float)(bbox[2].y()),0.0f));
	m_texrect=texrect;
	m_sprite=new Sprite(tex,rect,m_texrect);
	m_sprite->setColor(m_color);
	
}
Sprite2DAdapter::Sprite2DAdapter(sad::Texture * tex,const sad::Rect2D & texrect,const sad::Rect2D & bbox)
{
	m_flipx=false;
	m_flipy=false;
	m_angle=0;
	m_sprite=NULL;
	m_color = sad::AColor(255,255,255,0);
	createSprite(tex,texrect,bbox);
}

Sprite2DAdapter::Sprite2DAdapter(const Sprite2DAdapter & sprite)
{
	m_sprite=new Sprite(*(sprite.m_sprite));
	m_rect=sprite.m_rect;
	m_texrect=sprite.m_texrect;
	m_flipx=sprite.m_flipx;
	m_flipy=sprite.m_flipy;
	m_angle=sprite.m_angle;
	m_color = sad::AColor(255,255,255,0);
}
Sprite2DAdapter::~Sprite2DAdapter()
{
	delete m_sprite;
}

void Sprite2DAdapter::setPos(const sad::Point2D & p)
{
	sad::Point2D vector=p-pos();
	move(vector);
}

void Sprite2DAdapter::move(const sad::Point2D & p)
{
	::sad::Point3D vector((float)(p.x()),(float)(p.y()),0);
	m_sprite->moveBy(vector);
	for (int i=0;i<4;i++)
		m_rect[i]+=p;
}

sad::Point2D Sprite2DAdapter::pos() const
{
	sad::Point2D p = m_rect.p0();
	p+=m_rect.p2();
	p/=2.0;
	return p;
}

void Sprite2DAdapter::rotate(float angle)
{
	m_angle+=angle;
	m_sprite->rotate(angle,0);
}

float Sprite2DAdapter::angle() const
{
	return m_angle;
}


void Sprite2DAdapter::swapPoints(int p1, int p2)
{
	const float * coords=m_sprite->texCoords();
	float tmpx=coords[p1 << 1];
	float tmpy=coords[p1 << 1 | 1];
	m_sprite->setTexCoords(p1, coords[p2<<1],coords[p2<<1 | 1]);
	m_sprite->setTexCoords(p2,tmpx,tmpy);
}
void Sprite2DAdapter::flipX()
{
	swapPoints(0,1);
	swapPoints(3,2);
	m_flipx=!m_flipx;
}


void Sprite2DAdapter::flipY()
{
	swapPoints(0,3);
	swapPoints(1,2);
	m_flipy=!m_flipy;
}

bool Sprite2DAdapter::isFlippedX() const
{
	return m_flipx;
}

bool Sprite2DAdapter::isFlippedY() const
{
	return m_flipy;
}


void Sprite2DAdapter::updateBoundingRect()
{
	float angle=m_angle;
	rotate(-angle);
	for (int i=0;i<4;i++)
		m_sprite->point(i)=::sad::Point3D((float)(m_rect[i].x()),(float)(m_rect[i].y()),0.0f);
	rotate(angle);
}

void Sprite2DAdapter::flipBoundingRectX()
{
	sad::Rect2D m_copy=m_rect;
	m_rect[0]=m_copy[1];
	m_rect[1]=m_copy[0];
	m_rect[2]=m_copy[3];
	m_rect[3]=m_copy[2];
}

void Sprite2DAdapter::flipBoundingRectY()
{
	sad::Rect2D m_copy=m_rect;
	for (int i=0;i<4;i++)
		m_rect[i]=m_copy[3-i];
}

void Sprite2DAdapter::setLeft(float x)
{
	//Change rectangle, if rectangle becames flipped
	bool mustflip=m_rect[1].x()<x;
	m_rect[0].setX(x);
	m_rect[3].setX(x);
	if (mustflip)
		flipBoundingRectX();
	updateBoundingRect();
}

void Sprite2DAdapter::setRight(float x)
{
	//Change rectangle, if rectangle becames flipped
	bool mustflip=m_rect[0].x()>x;
	m_rect[1].setX(x);
	m_rect[2].setX(x);
	if (mustflip)
		flipBoundingRectX();	
	updateBoundingRect();
}

void Sprite2DAdapter::setTop(float y)
{
	//Change rectangle, if rectangle becames flipped
	bool mustflip=m_rect[0].y()>y;
	m_rect[2].setY(y);
	m_rect[3].setY(y);
	if (mustflip)
		flipBoundingRectY();	
	updateBoundingRect();
}

void Sprite2DAdapter::setBottom(float y)
{
	//Change rectangle, if rectangle becames flipped
	bool mustflip=m_rect[3].y()<y;
	m_rect[0].setY(y);
	m_rect[1].setY(y);
	if (mustflip)
		flipBoundingRectY();	
	updateBoundingRect();
}

sad::Point2D Sprite2DAdapter::size() const
{
	return sad::Point2D(m_rect.width(),m_rect.height());
}

void Sprite2DAdapter::setSize(const sad::Point2D & size)
{
  sad::Point2D pos=this->pos();
  sad::Point2D halfsize=size/2;
  m_rect=sad::Rect2D(pos-halfsize,pos+halfsize);
  updateBoundingRect();
}

void Sprite2DAdapter::render()
{
	m_sprite->render();
}

void Sprite2DAdapter::setSprite(sad::Texture * tex, const sad::Rect2D & texrect)
{
   createSprite(tex,texrect,m_rect);
   //Restore sprite rotation
   float angle=m_angle;
   m_angle=0;
   rotate(angle);
   // Flip on X axis if need to
   bool flipx=m_flipx;
   m_flipx=false;
   if (flipx)
	   flipX();
   // Flip on Y axis if need to
   bool flipy=m_flipy;
   m_flipy=false;
   if (flipy)
	   flipY(); 
}

const sad::Rect2D & Sprite2DAdapter::rect() const
{
	return m_rect;
}

void Sprite2DAdapter::setRect(const sad::Rect2D & rect)
{
	m_rect = rect;
	if (m_sprite)
	{
		sad::Rect< ::sad::Point3D> vrect;
		for(int i = 0; i < 4; i++)
		{
			vrect[i] = ::sad::Point3D((float)(rect[i].x()), (float)(rect[i].y()), 0.0f);
		}
		m_sprite->setBBox(vrect);
	}
}

void Sprite2DAdapter::setColor(const sad::AColor & color)
{
	m_color = color;
	if (m_sprite)
		m_sprite->setColor(color);
}

const sad::AColor & Sprite2DAdapter::color() const 
{
	return m_color;
}

void Sprite2DAdapter::set(const Sprite2DAdapter::Options & o)
{

	sad::Texture * tex = NULL;
	if (o.TextureContainer.exists())
	{
		sad::TextureContainer * c =  
			sad::TextureManager::ref()->getContainer(o.TextureContainer.data());
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
