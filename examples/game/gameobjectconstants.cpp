#include "gameobjectconstants.h"
#include <cstdlib>

Sprite2DAdapter::Options * GameObjectConstants<Player>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(0,87),hPointF(87,174));
	o->Rectangle = hRectF(hPointF(-11, -11), hPointF(11, 11));
	return o;
}

p2d::CollisionShape * GameObjectConstants<Player>::shape()
{
	p2d::Circle * c = new p2d::Circle();
	c->setRadius(11);
	return c;
}

Sprite2DAdapter::Options * GameObjectConstants<PlayerBullet>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(441,0),hPointF(452,11));
	o->Rectangle = hRectF(hPointF(-4, -4), hPointF(4, 4));
	return o;
}

p2d::CollisionShape * GameObjectConstants<PlayerBullet>::shape()
{
	p2d::Circle * c = new p2d::Circle();
	c->setRadius(4);
	return c;
}

Sprite2DAdapter::Options * GameObjectConstants<EnemyBullet>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(439,14),hPointF(458,33));
	o->Rectangle = hRectF(hPointF(-4, -4), hPointF(4, 4));
	return o;
}

double GameObjectConstants<EnemyBullet>::velocity()
{
	return 121;
}

p2d::CollisionShape * GameObjectConstants<EnemyBullet>::shape()
{
	p2d::Circle * c = new p2d::Circle();
	c->setRadius(4);
	return c;
}

Sprite2DAdapter::Options * GameObjectConstants<Bonus>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(1,1),hPointF(86,86));
	o->Rectangle = hRectF(hPointF(-8, -8), hPointF(8, 8));
	return o;
}

p2d::CollisionShape * GameObjectConstants<Bonus>::shape()
{
	Sprite2DAdapter::Options * options = GameObjectConstants<Bonus>::sprite();
	hRectF r = options->Rectangle;
	delete options;

	p2d::Rectangle * c = new p2d::Rectangle();
	c->setRect(r);
	return c;
}

Sprite2DAdapter::Options * GameObjectConstants<Enemy>::sprite()
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

p2d::CollisionShape * GameObjectConstants<Enemy>::shape()
{
	p2d::Circle * c = new p2d::Circle();
	c->setRadius(8);
	return c;
}

Sprite2DAdapter::Options * GameObjectConstants<ShootingEnemy>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(177,0),hPointF(265,88));
	o->Rectangle = hRectF(hPointF(-10, -10), hPointF(10, 10));
	return o;
}

p2d::CollisionShape * GameObjectConstants<ShootingEnemy>::shape()
{
	p2d::Circle * c = new p2d::Circle();
	c->setRadius(10);
	return c;
}


Sprite2DAdapter::Options * GameObjectConstants<SuperShootingEnemy>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(177,0),hPointF(265,88));
	o->Rectangle = hRectF(hPointF(-14, -14), hPointF(14, 14));
	return o;
}

p2d::CollisionShape * GameObjectConstants<SuperShootingEnemy>::shape()
{
	p2d::Circle * c = new p2d::Circle();
	c->setRadius(14);
	return c;
}

