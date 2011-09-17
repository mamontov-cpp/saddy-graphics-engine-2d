#include "movitem.h"

SAD_DECLARE(MovingObject,Collidable)
SAD_DECLARE(PlayerBullet,MovingObject)
SAD_DECLARE(EnemyBullet,MovingObject)
SAD_DECLARE(Bonus,MovingObject)
SAD_DECLARE(Enemy,MovingObject)
SAD_DECLARE(ShootingEnemy,MovingObject)
SAD_DECLARE(SuperShootingEnemy,MovingObject)
SAD_DECLARE(EnemyEmitter,sad::BasicNode)

#define ROTATE_SPEED 0.01
MovingObject::~MovingObject() {}
void MovingObject::render()
{
	if (!paused)
	{
	oldPoint()=newPoint();
	BoundingBox bb(oldPoint());
	interval()=1;
	for (int i=0;i<4;i++)
		bb[i]+=v();
	bool del=bb[0].x()<BOUND_X1 || bb[1].x()>BOUND_X2 
		  || bb[2].y()<BOUND_Y1 || bb[3].y()>BOUND_Y2;
    if (del)
		this->die();
	else
	{
	 newPoint()=bb;
	 this->moveBy(newPoint()[0]-oldPoint()[0]);
	 m_angle+=ROTATE_SPEED;
	 rotate(ROTATE_SPEED,0.0f);
	 this->Sprite::render();
	}
	}
	else this->Sprite::render();
}

#define BBOX_PERCENT 0.9
static BoundingBox createBBoxForObject(const hst::rect< ::s3d::point> & rect)
{
	float w2=(rect[1].x()-rect[0].x())/2*BBOX_PERCENT;
	float h2=(rect[2].y()-rect[1].y())/2*BBOX_PERCENT;
	::s3d::point middle=(rect[0]+rect[2])/2.0f;
	return BoundingBox( ::s3d::point(middle.x()-w2,middle.y()+h2,0),::s3d::point(middle.x()+w2,middle.y()-h2,0)  );
}
#define BULLET_WH 4.5f

PlayerBullet::PlayerBullet( const Vector & vec, const ::s3d::point &  pos):
MovingObject(
hst::rect< ::s3d::point>(::s3d::point(pos.x()-BULLET_WH,pos.y()+BULLET_WH,0),::s3d::point(pos.x()+BULLET_WH,pos.y()-BULLET_WH,0)),
hRectF(hPointF(441,0),hPointF(452,11))
)
{
	hst::rect< ::s3d::point> rect(point(0),point(1),point(2),point(3));
	this->oldPoint()=createBBoxForObject(rect);
	this->newPoint()=this->oldPoint();
	this->v()= ::s3d::point(vec.x(),vec.y(),0);
	CollisionManager::add(this->type(),this);
}
PlayerBullet::~PlayerBullet()
{
	//CollisionManager::remove(this);
}

#define EB_WH 4.5
EnemyBullet::EnemyBullet( const Vector & vec, const ::s3d::point &  pos):
MovingObject(
hst::rect< ::s3d::point>(::s3d::point(pos.x()-EB_WH,pos.y()+EB_WH,0),::s3d::point(pos.x()+EB_WH,pos.y()-EB_WH,0)),
hRectF(hPointF(440,12),hPointF(458,33))
)
{
	hst::rect< ::s3d::point> rect(point(0),point(1),point(2),point(3));
	this->oldPoint()=createBBoxForObject(rect);
	this->newPoint()=this->oldPoint();
	this->v()= ::s3d::point(vec.x(),vec.y(),0);
	CollisionManager::add(this->type(),this);
}
EnemyBullet::~EnemyBullet()
{
	//CollisionManager::remove(this);
}

#define BONUS_WH 8.5f
Bonus::Bonus( const Vector & vec, const hPointF &  pos):
MovingObject(
hst::rect< ::s3d::point>(::s3d::point(pos.x()-BONUS_WH,pos.y()+BONUS_WH,0),::s3d::point(pos.x()+BONUS_WH,pos.y()-BONUS_WH,0)),
hRectF(hPointF(1,1),hPointF(86,86))
)
{
	hst::rect< ::s3d::point> rect(point(0),point(1),point(2),point(3));
	this->oldPoint()=createBBoxForObject(rect);
	this->newPoint()=this->oldPoint();
	this->v()= ::s3d::point(vec.x(),vec.y(),0);
	CollisionManager::add(this->type(),this);
}
Bonus::~Bonus()
{
	//CollisionManager::remove(this);
}

hRectF  enemy_texc[4]={
hRectF(hPointF(88,0),hPointF(176,88)),
hRectF(hPointF(266,0),hPointF(354,88)),
hRectF(hPointF(355,0),hPointF(443,88)),
hRectF(hPointF(177,0),hPointF(265,88))
};
static const hRectF & createEnemyRect()
{
	return enemy_texc[rand() % 3];
}

#define ENEMY_WH 7.5f
Enemy::Enemy( const Vector & vec, const hPointF &  pos):
MovingObject(
hst::rect< ::s3d::point>(::s3d::point(pos.x()-ENEMY_WH,pos.y()+ENEMY_WH,0),::s3d::point(pos.x()+ENEMY_WH,pos.y()-ENEMY_WH,0)),
createEnemyRect()
)
{
	hst::rect< ::s3d::point> rect(point(0),point(1),point(2),point(3));
	this->oldPoint()=createBBoxForObject(rect);
	this->newPoint()=this->oldPoint();
	this->v()= ::s3d::point(vec.x(),vec.y(),0);
	CollisionManager::add(this->type(),this);
}


Enemy::~Enemy()
{
	//CollisionManager::remove(this);
}

ShootingEnemy::~ShootingEnemy()
{
	//CollisionManager::remove(this);
}
#undef  ENEMY_WH
#define ENEMY_WH 9.5f
ShootingEnemy::ShootingEnemy( const Vector & vec, const hPointF &  pos):
MovingObject(
hst::rect< ::s3d::point>(::s3d::point(pos.x()-ENEMY_WH,pos.y()+ENEMY_WH,0),::s3d::point(pos.x()+ENEMY_WH,pos.y()-ENEMY_WH,0)),
enemy_texc[3]
)
{
	hst::rect< ::s3d::point> rect(point(0),point(1),point(2),point(3));
	this->oldPoint()=createBBoxForObject(rect);
	this->newPoint()=this->oldPoint();
	this->v()= ::s3d::point(vec.x(),vec.y(),0);
	m_lastclock=0;
	CollisionManager::add(this->type(),this);
}
#define SE_BULLET_SPEED 0.8
void ShootingEnemy::render()
{
	m_lastclock++;
	if (m_lastclock>SHOOT_FREQ/5)
	{
		Vector bdir(SE_BULLET_SPEED*cos(m_angle),SE_BULLET_SPEED*sin(m_angle));
		if (!paused)
		sad::Renderer::instance().getCurrentScene()->markForAddition(new EnemyBullet(bdir,this->middle()));
		m_lastclock=0;
	}
	this->MovingObject::render();
}
void SuperShootingEnemy::hit()
{
 --m_lifes; 
 if (!m_lifes) this->die(); 
 if (m_lifes<0)
	 assert(false && "WTF!");
}

#undef  ENEMY_WH
#define ENEMY_WH 13.5f
SuperShootingEnemy::SuperShootingEnemy( const Vector & vec, const hPointF &  pos):
MovingObject(
hst::rect< ::s3d::point>(::s3d::point(pos.x()-ENEMY_WH,pos.y()+ENEMY_WH,0),::s3d::point(pos.x()+ENEMY_WH,pos.y()-ENEMY_WH,0)),
enemy_texc[3]
)
{
	hst::rect< ::s3d::point> rect(point(0),point(1),point(2),point(3));
	this->oldPoint()=createBBoxForObject(rect);
	this->newPoint()=this->oldPoint();
	this->v()= ::s3d::point(vec.x(),vec.y(),0);
	m_lastclock=0;
	m_lifes=EMAX_LIFES;
	CollisionManager::add(this->type(),this);
}
SuperShootingEnemy::~SuperShootingEnemy() 
{
	//CollisionManager::remove(this);
}

void SuperShootingEnemy::render()
{
	m_lastclock++;
	if (m_lastclock>SHOOT_FREQ/3)
	{
		if (paused) return;
		float a=atan2(v().y(),v().x());
		a+=M_PI_4;
		::s3d::point p=this->middle();
		hPointF m(p.x(),p.y());
		for (int i=0;i<4;i++)
		{
			Vector bdir(SE_BULLET_SPEED*cos(a),SE_BULLET_SPEED*sin(a));
			sad::Renderer::instance().getCurrentScene()->markForAddition(new ShootingEnemy(bdir,m));
			a+=M_PI_2;
		}
		m_lastclock=0;
	}
	this->MovingObject::render();
}
EnemyEmitter::EnemyEmitter(int what)
{
    m_clk=0;
	if (what==REAL_SPAWN)
		m_r=&EnemyEmitter::renderSpawn;
	else
		m_r=&EnemyEmitter::renderRain;
}

EnemyEmitter::~EnemyEmitter()
{

}
void EnemyEmitter::render()
{
	(this->*m_r)();
}

::s3d::point convertTo3d(const hPointF & x)
{
	return ::s3d::point(x.x(),x.y(),0.0f);
}

template<typename T>
static void addToScene(const Vector & v, const hPointF & p)
{
	sad::Renderer::instance().getCurrentScene()->markForAddition(new T(v,p));
}
static void addEnemyBullet(const Vector & v, const hPointF & p)
{
	sad::Renderer::instance().getCurrentScene()->markForAddition(new EnemyBullet(v,convertTo3d(p)));
}
void (*adders[5])(const Vector & v, const hPointF & p)=
{
	addEnemyBullet,
	addToScene<Enemy>,
	addToScene<ShootingEnemy>,
	addToScene<Bonus>,
	addToScene<SuperShootingEnemy>
};
#define BOSS_SPAWN_TIME 8*CLOCKS_PER_SEC
void createRandomEnemy(const Vector & v, const hPointF & p)
{
	int wh=rand() % 5;
	static clock_t clk;
	if (clock()-clk<BOSS_SPAWN_TIME && wh==4)  wh=2;
	if (wh==4) { clk=clock();   }
	adders[wh](v,p);
}
#define RAIN_SPEED 0.5
int xmax=BOUND_X2-12;
int xmin=BOUND_X1+12;
int ymin=BOUND_Y1+12;
int ymax=BOUND_Y2-12;
void EnemyEmitter::renderRain()
{
 if (paused) return;
 if ((clock()-m_clk)/(float)CLOCKS_PER_SEC*1000<SPAWN_FREQ4)
		return;
 m_clk=clock();
 float x=xmin;float y=ymax;
 if (rand() % 4 > 1 )
 {x=((float)rand()/RAND_MAX)*(xmax-xmin)+xmin;}
 else
 {y=(((float)rand()/RAND_MAX/2)+0.5)*(ymax-ymin)+ymin;}
 adders[rand() % 4](Vector(RAIN_SPEED,-1*RAIN_SPEED),hPointF(x,y));
}

void EnemyEmitter::renderSpawn()
{
 if (paused) return;
 if ((clock()-m_clk)/(float)CLOCKS_PER_SEC*1000<SPAWN_FREQ)
		return;
 m_clk=clock();
 if (sad::Renderer::instance().getCurrentScene()->objectCount()>65)
	 return;
 float x=((float)rand()/RAND_MAX)*(xmax-xmin)+xmin;
 float y=((float)rand()/RAND_MAX)*(ymax-ymin)+ymin;
 float vx=(float)rand()/RAND_MAX*2.5*RAIN_SPEED-1.25*RAIN_SPEED;
 float vy=(float)rand()/RAND_MAX*2.5*RAIN_SPEED-1.25*RAIN_SPEED;
 createRandomEnemy(Vector(vx,vy),hPointF(x,y));
}