#include "player.h"
#include "../renderer.h"

#define BBOX_PERCENT 0.9

static BoundingBox createBBoxForPlayer(const hst::rect<::s3d::point> & rect)
{
	float w2=(rect[1].x()-rect[0].x())/2*BBOX_PERCENT;
	float h2=(rect[2].y()-rect[1].y())/2*BBOX_PERCENT;
	::s3d::point middle=(rect[0]+rect[2])/2.0f;
	return BoundingBox( ::s3d::point(middle.x()-w2,middle.y()+h2,0),::s3d::point(middle.x()+w2,middle.y()-h2,0)  );
}

SAD_DECLARE(Player,Collidable)

Player * Player::instance=NULL;
#ifndef BOUND_X1
#define BOUND_X1 0.0f
#define BOUND_X2 640.0f
#define BOUND_Y1 0.0f
#define BOUND_Y2 480.0f
#endif

void Player::move(const Vector & p)
{
	oldPoint()=newPoint();
	BoundingBox bb(oldPoint());
	::s3d::point nx(p.x(),p.y(),0.0f);
	for (int i=0;i<4;i++)
		bb[i]+=nx;
	if (bb[0].x()<BOUND_X1) 
	{
		float new_x=BOUND_X2-0.01;
		float old_x=bb[1].x();
		for(int i=0;i<4;i++)
			bb[i].setX(bb[i].x()+new_x-old_x);
		oldPoint()=bb;
	}
	if (bb[1].x()>BOUND_X2) 
	{ 
		float new_x=BOUND_X1+0.01;
		float old_x=bb[0].x();
		for(int i=0;i<4;i++)
			bb[i].setX(bb[i].x()+new_x-old_x);
		oldPoint()=bb;
	}
	if (bb[2].y()<BOUND_Y1) 
	{ 
		float new_x=BOUND_Y2-0.01;
		float old_x=bb[0].y();
		for(int i=0;i<4;i++)
			bb[i].setY(bb[i].y()+new_x-old_x);
		oldPoint()=bb;
	}
	if (bb[0].y()>BOUND_Y2) 
	{ 
		float new_x=BOUND_Y1+0.01;
		float old_x=bb[2].y();
		for(int i=0;i<4;i++)
			bb[i].setY(bb[i].y()+new_x-old_x);
		oldPoint()=bb; 
	}
	this->interval()=1;
	this->v()=::s3d::point(
		                    newPoint()[0].x()-oldPoint()[0].x(),
							newPoint()[0].y()-oldPoint()[0].y(),
							0.0f
		                  );
	newPoint()=bb;
	::s3d::point mid=(newPoint()[0]+newPoint()[2])/2.0f;
	this->moveTo(mid);
}

void Player::setAngle(float angle)
{
	float dangle=angle-m_angle;
	this->Sprite::rotate(dangle,0.0f);
	m_angle=angle;
}

extern bool paused;
void Player::render()
{
	if ((fabs(m_velocity[0])>0.00001 || fabs(m_velocity[1])>0.00001) && !paused)
		this->move(Vector(m_velocity[0],m_velocity[1]));
    if (m_first_render)
	{
		::s3d::point d=sad::Renderer::instance().mousePos();
		::s3d::point p=Player::instance->middle();
		m_angle=atan2(d.y()-p.y(),d.x()-p.x());
		this->rotate(m_angle,0);
		m_first_render=false;
	}
	this->Sprite::render();
}

void Player::shoot()
{
	float fx=0.07*cos(m_angle);
	float fy=0.07*sin(m_angle);
   
	//sad::Renderer::instance().getCurrentScene()->markForAddition
	//	( new PlayerBullet(Vector(fx,fy),this->middle());
}

Player::~Player()
{
	//CollisionManager::remove(this);
	instance=NULL;
}

#define PLAYER_WH 11
Player::Player(const hPointF & pos):Collidable(
sad::TextureManager::instance()->get("objects"),
hst::rect<::s3d::point>(::s3d::point(pos.x()-PLAYER_WH,pos.y()+PLAYER_WH,0),::s3d::point(pos.x()+PLAYER_WH,pos.y()-PLAYER_WH,0)),
hRectF(hPointF(0,87),hPointF(87,174))
)
{
	float * sz=const_cast<float*>(texCoords());
	hst::rect<::s3d::point> rect(point(0),point(1),point(2),point(3));
	oldPoint()=createBBoxForPlayer(rect);
	newPoint()=oldPoint();
	m_velocity[0]=m_velocity[1]=0.0f;
	m_first_render=true;
	//CollisionManager::add(this);
	instance=this;
}
