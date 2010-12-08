#include "movitem.h"


BoundingBox MovingItem::rect()
{
	return m_box;
}
MovingItem::~MovingItem()
{

}

void MovingItem::move()
{
 float fps=(float)sad::Renderer::instance().fps();
 if (fps<10) fps=80;
  Vector dir=m_direct*(1.0f/fps);
  m_box.moveBy(dir);
  m_draw.moveBy(dir);
  
  hPointF center=(m_draw[0]+m_draw[2])/2;
  if (center.x()<-0.275042 || 
	  center.x()>0.274406  ||
	  center.y()<-0.205373 ||
	  center.y()>0.206282  
	  )
  sad::Renderer::instance().getCurrentScene()->markForDeletion(this);
}
MovingItem::MovingItem(sad::Texture * tex,
		               const Vector & vec,
		               const BoundingBox &  draw, 
			           float percent
			           )
{
	m_angle=0.0f;
	m_tex=tex;
	m_direct=vec;
	m_draw=draw;
    
	m_box=draw.enlarged(draw.width()*(percent-1),draw.height()*(percent-1));
}

void MovingItem::render()
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

	
	glTexCoord2f(0.0f,0.0f);	
		glVertex3f(m_x-dx,m_y-dy,ZAX );
	glTexCoord2f(0.0f,1.0f);
		glVertex3f(m_x-dy,m_y+dx,ZAX );
	glTexCoord2f(1.0f,1.0f);
		glVertex3f(m_x+dx,m_y+dy,ZAX );
	glTexCoord2f(1.0f,0.0f);
		glVertex3f(m_x+dy,m_y-dx,ZAX );
    
	glEnd();

	m_angle+=0.01;

	move();
}

int PlayerBullet::Type=25;

PlayerBullet::PlayerBullet(const Vector &vec, const BoundingBox &draw, float percent): MovingItem(NULL,vec,draw,percent)
{
  m_type=PlayerBullet::Type;
  m_tex=sad::TextureManager::instance()->get("playerbullet");   
  CollisionManager::add(this);
}
PlayerBullet::~PlayerBullet()
{
	CollisionManager::remove(this);
}

int EnemyBullet::Type=26;

EnemyBullet::EnemyBullet(const Vector &vec, const BoundingBox &draw, float percent): MovingItem(NULL,vec,draw,percent)
{
  m_type=EnemyBullet::Type;
  m_tex=sad::TextureManager::instance()->get("enemybullet");   
  CollisionManager::add(this);
}
EnemyBullet::~EnemyBullet()
{
	CollisionManager::remove(this);
}


int Bonus::Type=27;

Bonus::Bonus(const Vector &vec, const BoundingBox &draw, float percent): MovingItem(NULL,vec,draw,percent)
{
  m_type=Bonus::Type;
  m_tex=sad::TextureManager::instance()->get("bonus");   
  CollisionManager::add(this);
}
Bonus::~Bonus()
{
	CollisionManager::remove(this);
}


int Enemy::Type=28;
#define GET_TEX(X) sad::TextureManager::instance()->get(X)
Enemy::Enemy(const Vector &vec, const BoundingBox &draw, float percent): MovingItem(NULL,vec,draw,percent)
{
  m_type=Enemy::Type;
  int r=(int)(((float)rand())/RAND_MAX*3.0f);
  if (r==3)--r;
  if (r==0) m_tex=GET_TEX("halfsmile");
  if (r==1) m_tex=GET_TEX("neutral");
  if (r==2) m_tex=GET_TEX("smile");
  CollisionManager::add(this);
}
Enemy::~Enemy()
{
	CollisionManager::remove(this);
}

int ShootingEnemy::Type=29;

ShootingEnemy::ShootingEnemy(const Vector &vec, const BoundingBox &draw, float percent): MovingItem(NULL,vec,draw,percent)
{
  m_type=ShootingEnemy::Type;
  m_tex=GET_TEX("largesmile");
  m_lastclock=0;
  CollisionManager::add(this);
}
ShootingEnemy::~ShootingEnemy()
{
	CollisionManager::remove(this);
}

void ShootingEnemy::render()
{
	if (clock()-m_lastclock>SHOOT_FREQ)
	{
		m_lastclock=clock();
		Vector bdir(0.08*cos(m_angle),0.08*sin(m_angle));
		sad::Renderer::instance().getCurrentScene()->markForAddition(new EnemyBullet(bdir,m_draw,0.5));
	}
	this->MovingItem::render();
}