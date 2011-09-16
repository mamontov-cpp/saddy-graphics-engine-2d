#include "movitem.h"

SAD_DECLARE(MovingObject,Collidable)
SAD_DECLARE(PlayerBullet,MovingObject)
SAD_DECLARE(EnemyBullet,MovingObject)
SAD_DECLARE(Bonus,MovingObject)
SAD_DECLARE(Enemy,MovingObject)
SAD_DECLARE(ShootingEnemy,MovingObject)
SAD_DECLARE(EnemyEmitter,sad::BasicNode)

#ifdef WORK

PlayerBullet::PlayerBullet(const Vector &vec, const BoundingBox &draw, float percent): MovingItem(NULL,vec,draw,percent)
{
  m_tex=sad::TextureManager::instance()->get("playerbullet");   
  CollisionManager::add(this);
}
PlayerBullet::~PlayerBullet()
{
	CollisionManager::remove(this);
}

EnemyBullet::EnemyBullet(const Vector &vec, const BoundingBox &draw, float percent): MovingItem(NULL,vec,draw,percent)
{
  m_tex=sad::TextureManager::instance()->get("enemybullet");   
  CollisionManager::add(this);
}
EnemyBullet::~EnemyBullet()
{
	CollisionManager::remove(this);
}



Bonus::Bonus(const Vector &vec, const BoundingBox &draw, float percent): MovingItem(NULL,vec,draw,percent)
{
  m_tex=sad::TextureManager::instance()->get("bonus");   
  CollisionManager::add(this);
}
Bonus::~Bonus()
{
	CollisionManager::remove(this);
}


#define GET_TEX(X) sad::TextureManager::instance()->get(X)
Enemy::Enemy(const Vector &vec, const BoundingBox &draw, float percent): MovingItem(NULL,vec,draw,percent)
{
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


ShootingEnemy::ShootingEnemy(const Vector &vec, const BoundingBox &draw, float percent): MovingItem(NULL,vec,draw,percent)
{
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
	if (what==REAL_SPAWN)
		m_r=&EnemyEmitter::renderSpawn;
	else
		m_r=&EnemyEmitter::renderRain;
}

#endif