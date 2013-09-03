#include "WorldObjectConstants.h"
#include <cstdlib>

Sprite2DAdapter::Options * WorldObjectConstants<Player>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(0,88),hPointF(88,174));
	o->Rectangle = hRectF(hPointF(-11, -11), hPointF(11, 11));
	return o;
}

p2d::CollisionShape * WorldObjectConstants<Player>::shape()
{
	p2d::Circle * c = new p2d::Circle();
	c->setRadius(11);
	return c;
}

Sprite2DAdapter::Options * WorldObjectConstants<PlayerBullet>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(441,0),hPointF(452,11));
	o->Rectangle = hRectF(hPointF(-4, -4), hPointF(4, 4));
	return o;
}

p2d::CollisionShape * WorldObjectConstants<PlayerBullet>::shape()
{
	p2d::Circle * c = new p2d::Circle();
	c->setRadius(4);
	return c;
}

double WorldObjectConstants<PlayerBullet>::velocity()
{
	return 121;
}

double WorldObjectConstants<PlayerBullet>::interval()
{
	return 400;
}

Sprite2DAdapter::Options * WorldObjectConstants<EnemyBullet>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(439,14),hPointF(458,33));
	o->Rectangle = hRectF(hPointF(-4, -4), hPointF(4, 4));
	return o;
}

double WorldObjectConstants<EnemyBullet>::velocity()
{
	return 121;
}

double WorldObjectConstants<EnemyBullet>::interval()
{
	return 400;
}

p2d::CollisionShape * WorldObjectConstants<EnemyBullet>::shape()
{
	p2d::Circle * c = new p2d::Circle();
	c->setRadius(4);
	return c;
}

Sprite2DAdapter::Options * WorldObjectConstants<Bonus>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(1,1),hPointF(86,86));
	o->Rectangle = hRectF(hPointF(-8, -8), hPointF(8, 8));
	return o;
}

p2d::CollisionShape * WorldObjectConstants<Bonus>::shape()
{
	Sprite2DAdapter::Options * options = WorldObjectConstants<Bonus>::sprite();
	hRectF r = options->Rectangle;
	delete options;

	// For a simple optimization, make bonus a circle
	p2d::Rectangle * c = new p2d::Rectangle();
	c->setRect(r);
	return c;
}

Sprite2DAdapter::Options * WorldObjectConstants<Enemy>::sprite()
{
	/*! A special texture rectangles, used to shape enemies
	 */
	hRectF texture_rectangles[3]={
		hRectF(hPointF(88,0),hPointF(176,88)),
		hRectF(hPointF(266,0),hPointF(354,88)),
		hRectF(hPointF(355,0),hPointF(443,88)),
	};
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = texture_rectangles[ rand () % 3 ];
	o->Rectangle = hRectF(hPointF(-8, -8), hPointF(8, 8));
	return o;
}

p2d::CollisionShape * WorldObjectConstants<Enemy>::shape()
{
	p2d::Circle * c = new p2d::Circle();
	c->setRadius(8);
	return c;
}

Sprite2DAdapter::Options * WorldObjectConstants<ShootingEnemy>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(177,0),hPointF(265,88));
	o->Rectangle = hRectF(hPointF(-10, -10), hPointF(10, 10));
	return o;
}

double WorldObjectConstants<ShootingEnemy>::velocity()
{
	return 100;
}

double WorldObjectConstants<ShootingEnemy>::interval()
{
	return 1000;
}

p2d::CollisionShape * WorldObjectConstants<ShootingEnemy>::shape()
{
	p2d::Circle * c = new p2d::Circle();
	c->setRadius(10);
	return c;
}


Sprite2DAdapter::Options * WorldObjectConstants<SuperShootingEnemy>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(177,0),hPointF(265,88));
	o->Rectangle = hRectF(hPointF(-14, -14), hPointF(14, 14));
	return o;
}

p2d::CollisionShape * WorldObjectConstants<SuperShootingEnemy>::shape()
{
	p2d::Circle * c = new p2d::Circle();
	c->setRadius(14);
	return c;
}

