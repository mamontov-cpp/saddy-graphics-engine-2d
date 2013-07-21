#include "player.h"
#include "game.h"
#include <renderer.h>
#include <extra/geometry2d.h>

#define BBOX_PERCENT 0.9

static BoundingBox createBBoxForPlayer(const hst::rect< ::s3d::point> & rect)
{
	float w2=(float)((rect[1].x()-rect[0].x())/2*BBOX_PERCENT);
	float h2=(float)((rect[2].y()-rect[1].y())/2*BBOX_PERCENT);
	::s3d::point middle=(rect[0]+rect[2])/2.0f;
	return BoundingBox( ::s3d::point(middle.x()-w2,middle.y()+h2,0),::s3d::point(middle.x()+w2,middle.y()-h2,0)  );
}

DECLARE_SOBJ_INHERITANCE(Player,Collidable)

void Player::move(const Vector & p)
{
	oldPoint()=newPoint();
	BoundingBox bb(oldPoint());
	::s3d::point nx((float)(p.x()),(float)(p.y()),0.0f);
	for (int i=0;i<4;i++)
		bb[i]+=nx;
	if (bb[0].x()<BOUND_X1) 
	{
		float new_x=BOUND_X2-0.01f;
		float old_x=bb[1].x();
		for(int i=0;i<4;i++)
			bb[i].setX(bb[i].x()+new_x-old_x);
		oldPoint()=bb;
	}
	if (bb[1].x()>BOUND_X2) 
	{ 
		float new_x=BOUND_X1+0.01f;
		float old_x=bb[0].x();
		for(int i=0;i<4;i++)
			bb[i].setX(bb[i].x()+new_x-old_x);
		oldPoint()=bb;
	}
	if (bb[2].y()<BOUND_Y1) 
	{ 
		float new_x=BOUND_Y2-0.01f;
		float old_x=bb[0].y();
		for(int i=0;i<4;i++)
			bb[i].setY(bb[i].y()+new_x-old_x);
		oldPoint()=bb;
	}
	if (bb[0].y()>BOUND_Y2) 
	{ 
		float new_x=BOUND_Y1+0.01f;
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

void Player::render()
{
	bool moving = non_fuzzy_zero(m_velocity.x()) || non_fuzzy_zero(m_velocity.y()); 
	if (moving && m_game->isPlaying())
	{
		this->move(m_velocity);
	}
	this->Sprite::render();
}



#define PLAYER_WH 11
Player::Player(const hPointF & pos):Collidable(
sad::TextureManager::ref()->get("objects"),
hst::rect< ::s3d::point>(
	::s3d::point(
				 (float)(pos.x()-PLAYER_WH),
				 (float)(pos.y()-PLAYER_WH),
				 0),
	::s3d::point((float)(pos.x()+PLAYER_WH),
				 (float)(pos.y()+PLAYER_WH),
				 0)),
hRectF(hPointF(0,87),hPointF(87,174))
)
{
	hst::rect< ::s3d::point> rect(point(0),point(1),point(2),point(3));
	oldPoint()=createBBoxForPlayer(rect);
	newPoint()=oldPoint();
	m_velocity = Vector(0,0);
	m_lastshot=0;
	CollisionManager::add(this->type(),this);

	m_score = 0;
	m_health = 10;
}



void Player::lookAt(const hPointF & o)
{
	::s3d::point p=this->middle();
	float af=atan2(o.y()-p.y(),o.x()-p.x());
	this->setAngle(af);
}

void Player::tryLookAt(const sad::Event & p)
{
	if (m_game->isPlaying())
	{
		lookAt(hPointF(p.x, p.y));
	}
}

#define BULLET_SPEED 120.45f
#define PLAYER_SHOOTING_FREQUENCY 450
void Player::shoot()
{
	float fx=BULLET_SPEED*cos(m_angle);
	float fy=BULLET_SPEED*sin(m_angle);
   
	PlayerBullet * item = new PlayerBullet(Vector(fx,fy),this->middle());
	sad::Renderer::ref()->getCurrentScene()->add(item);
	m_lastshot=clock();
}

void Player::tryShoot()
{
	if (clock()-m_lastshot>=PLAYER_SHOOTING_FREQUENCY && m_game->isPlaying())
	{	
		this->shoot();
	}
}

int Player::score() const
{
	return m_score;
}

int Player::increaseScore(int delta)
{
	m_score +=delta;
	return m_score;
}

int Player::health() const
{
	return m_health;
}

int Player::increaseHealth(int by)
{
	m_health += by;
	return m_health;
}

int Player::decreaseHealth(int by)
{
	m_health -= by;
	return m_health;
}

/*! A positive speed as passed distance in second
 */
#define P_SPEED 1.0
/*! A negative speed as passed distance in second
 */
#define N_SPEED -1.0

void Player::tryStartMovingLeft(const sad::Event & e)
{
	if (m_game->isPlaying())
	{
		m_velocity.setX(N_SPEED);
		m_stopkeys[0] = e.key;
	}
}

void Player::tryStartMovingRight(const sad::Event & e)
{
	if (m_game->isPlaying())
	{
		m_velocity.setX(P_SPEED);
		m_stopkeys[0] = e.key;
	}
}

void Player::tryStartMovingUp(const sad::Event & e)
{
	if (m_game->isPlaying())
	{
		m_velocity.setY(P_SPEED);
		m_stopkeys[1] = e.key;
	}
}

void Player::tryStartMovingDown(const sad::Event & e)
{
	if (m_game->isPlaying())
	{
		m_velocity.setY(N_SPEED);
		m_stopkeys[1] = e.key;
	}
}

void Player::tryStopMovingHorizontally(const sad::Event & e)
{
	if (e.key == m_stopkeys[0])
	{
		m_velocity.setX(0);
	}
}

void Player::tryStopMovingVertically(const sad::Event & e)
{
	if (e.key == m_stopkeys[1])
	{
		m_velocity.setY(0);
	}
}

void Player::setGame(Game * game)
{
	m_game = game;
}

