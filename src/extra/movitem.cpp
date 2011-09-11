#include "movitem.h"

//#define CLOCK_TEST

#ifdef CLOCK_TEST
	#include "log.h"
#endif

//#define COLLISION_TEST
#ifdef COLLISION_TEST
	sad::Texture * bbox_test;
#endif

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

#ifdef COLLISION_TEST
   bbox_test->enable();
   glBegin(GL_QUADS);

   hPointF t1=m_box[0],t2=m_box[1], t3=m_box[2],t4=m_box[3] ;	
	
   glTexCoord2f(0.0f,0.0f);	
		glVertex3f(t1.x(),t1.y(),ZAX );
   glTexCoord2f(0.0f,1.0f);
		glVertex3f(t2.x(),t2.y(),ZAX );
   glTexCoord2f(1.0f,1.0f);
		glVertex3f(t3.x(),t3.y(),ZAX );
   glTexCoord2f(1.0f,0.0f);
		glVertex3f(t4.x(),t4.y(),ZAX );
    
  glEnd();
	
#endif
	

	if (!paused)
	{
		m_angle+=0.01;
		move();
	}
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
	if ((clock()-m_lastclock)/(float)CLOCKS_PER_SEC*1000>SHOOT_FREQ)
	{
		m_lastclock=clock();
		Vector bdir(0.08*cos(m_angle),0.08*sin(m_angle));
		if (!paused)
		sad::Renderer::instance().getCurrentScene()->markForAddition(new EnemyBullet(bdir,m_draw,0.5));
	}
	this->MovingItem::render();
}

int EnemyEmitter::Type=65;

EnemyEmitter::~EnemyEmitter()
{

}
void EnemyEmitter::render()
{
	(this->*m_r)();
}

#define ADD_SCENE(X) sad::Renderer::instance().getCurrentScene()->markForAddition(new X);
void EnemyEmitter::renderSpawn()
{
	if (paused) return;
#ifdef CLOCK_TEST
    hst::log::inst()->write(clock());
	hst::log::inst()->write(' ');
	hst::log::inst()->write(m_clk);
	hst::log::inst()->write('\n');
#endif
	if ((clock()-m_clk)/(float)CLOCKS_PER_SEC*1000<SPAWN_FREQ)
		return;

#ifdef CLOCK_TEST
	hst::log::inst()->write(hst::string("EnemyEmitter::renderRain()\n"));
#endif
		
	m_clk=clock();

	float rrx=((float)rand()/RAND_MAX)*(BOUND_X2-BOUND_X1)+BOUND_X1;
	float rry=((float)rand()/RAND_MAX)*(BOUND_Y2-BOUND_Y1)+BOUND_Y1;
		
	int mx=(int)(((float)rand()/RAND_MAX)*4);
	if (mx==4) --mx;
	
	float spdx=((float)rand()/RAND_MAX)*0.1f-0.05f;
	float spdy=((float)rand()/RAND_MAX)*0.1f-0.05f;
	
	if (mx==0) ADD_SCENE(EnemyBullet(Vector(spdx,spdy),BoundingBox(hPointF(rrx,rry),0.02,0.02),0.5));
	if (mx==1) ADD_SCENE(ShootingEnemy(Vector(spdx,spdy),BoundingBox(hPointF(rrx,rry),0.02,0.02),0.8));
	if (mx==2) ADD_SCENE(Enemy(Vector(spdx,spdy),BoundingBox(hPointF(rrx,rry),0.02,0.02),0.8));
	if (mx==3) ADD_SCENE(Bonus(Vector(spdx,spdy),BoundingBox(hPointF(rrx,rry),0.02,0.02),0.8));
}
void EnemyEmitter::renderRain()
{
	if (paused) return;
	if ((clock()-m_clk)/(float)CLOCKS_PER_SEC*1000<SPAWN_FREQ4)
		return;

#ifdef CLOCK_TEST
	hst::log::inst()->write(hst::string("EnemyEmitter::renderRain()\n"));
#endif

	m_clk=clock();
	if (rand()<RAND_MAX/2)
	{
		float rr=((float)rand()/RAND_MAX)*(BOUND_X2-BOUND_X1)+BOUND_X1;
		int mx=(int)(((float)rand()/RAND_MAX)*3);
		if (mx==3) --mx;
		
		if (mx==0) ADD_SCENE(EnemyBullet(Vector(0.05,-0.05),BoundingBox(hPointF(rr,BOUND_Y2),0.02,0.02),0.5));
		if (mx==1) ADD_SCENE(Bonus(Vector(0.05,-0.05),BoundingBox(hPointF(rr,BOUND_Y2),0.02,0.02),0.8));
		if (mx==2) ADD_SCENE(Enemy(Vector(0.05,-0.05),BoundingBox(hPointF(rr,BOUND_Y2),0.02,0.02),0.8));
	}
	else
	{
		float rr=((float)rand()/RAND_MAX)*(BOUND_Y2-BOUND_Y1)+BOUND_Y1;
		int mx=(int)(((float)rand()/RAND_MAX)*3);
		if (mx==3) --mx;
		if (mx==0) ADD_SCENE(EnemyBullet(Vector(0.05,-0.05),BoundingBox(hPointF(BOUND_X1,rr),0.02,0.02),0.5));
		if (mx==1) ADD_SCENE(Bonus(Vector(0.05,-0.05),BoundingBox(hPointF(BOUND_X1,rr),0.02,0.02),0.8));
		if (mx==2) ADD_SCENE(Enemy(Vector(0.05,-0.05),BoundingBox(hPointF(BOUND_X1,rr),0.02,0.02),0.8));
	}
}

EnemyEmitter::EnemyEmitter(int what)
{
    m_clk=0;
	m_type=EnemyEmitter::Type;
	if (what==REAL_SPAWN)
		m_r=&EnemyEmitter::renderSpawn;
	else
		m_r=&EnemyEmitter::renderRain;
}
