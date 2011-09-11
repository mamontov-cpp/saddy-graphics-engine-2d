#include "player.h"
#include "lightmodel.h"

SAD_DECLARE(Player,Collidable)

Player * Player::instance=NULL;

BoundingBox Player::rect() { return m_box; }


void Player::move(const Vector & p)
{
	m_box.moveBy(p);
	m_draw.moveBy(p);

	bool ch=false;
	float m_point;
	if (m_draw.p().x()<BOUND_X1) 
	{
		m_point=m_draw.p().x();
		m_box.moveBy(hPointF(BOUND_X2-m_point,0));
		m_draw.p().setX(BOUND_X2); 
	    ch=true; 
	}
	if (m_draw.p().x()>BOUND_X2) 
	{ 
		m_point=m_draw.p().x();
		m_box.moveBy(hPointF(BOUND_X1-m_point,0));
		m_draw.p().setX(BOUND_X1); 
		ch=true; 
	}
	if (m_draw.p().y()<BOUND_Y1) 
	{ 
		m_point=m_draw.p().y();
		m_box.moveBy(hPointF(0,BOUND_Y2-m_point));
		m_draw.p().setY(BOUND_Y2); 
		ch=true; 
	}
	if (m_draw.p().y()>BOUND_Y2) 
	{ 
		m_point=m_draw.p().y();
		m_box.moveBy(hPointF(0,BOUND_Y1-m_point));
		m_draw.p().setY(BOUND_Y1); 
		ch=true; 
	}


	if (ch)
		CollisionManager::updateSingle(this);

}

void Player::setAngle(float angle)
{
	m_angle=angle;
}


void Player::render()
{
	float m_x=m_draw.p().x()+m_draw.width()/2;
	float m_y= m_draw.p().y()+m_draw.height()/2;
	float m_hw=m_draw.width()/2;
	float m_hh=m_draw.height()/2;

    float m_cos=m_hw*cos(m_angle);
	float m_sin=m_hh*sin(m_angle);
	float dx=m_cos-m_sin;
	float dy=m_cos+m_sin;

	m_tex->enable();
    
	glBegin(GL_QUADS);

	
	glTexCoord2f(0.0f,1.0f);	
		glVertex3f(m_x-dx,m_y-dy,ZAX );
	glTexCoord2f(0.0f,0.0f);
		glVertex3f(m_x-dy,m_y+dx,ZAX );
	glTexCoord2f(1.0f,0.0f);
		glVertex3f(m_x+dx,m_y+dy,ZAX );
	glTexCoord2f(1.0f,1.0f);
		glVertex3f(m_x+dy,m_y-dx,ZAX );
    
	glEnd();

}

void Player::shoot()
{
	float fx=0.07*cos(m_angle);
	float fy=0.07*sin(m_angle);
   
	sad::Renderer::instance().getCurrentScene()->markForAddition
		( new PlayerBullet(Vector(fx,fy),m_draw,0.9) );
}

Player::~Player()
{
	CollisionManager::remove(this);
	instance=NULL;
}
const BoundingBox & Player::prect()
{
	return m_draw;
}

Player::Player(const BoundingBox &  draw, float percent)
{
	m_angle=0.0f;
	m_tex=sad::TextureManager::instance()->get("sad");
	m_draw=draw;
    
	m_box=draw.enlarged(draw.width()*(percent-1),draw.height()*(percent-1));

	CollisionManager::add(this);
	instance=this;
}
