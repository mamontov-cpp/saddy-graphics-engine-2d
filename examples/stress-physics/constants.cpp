#include "constants.h"

#include <cstdlib>


Sprite2DAdapter::Options * sad::p2d::app::Constants<UncoloredBullet>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(441,0),hPointF(452,11));
	o->Rectangle = hRectF(hPointF(-4, -4), hPointF(4, 4));
	return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<UncoloredBullet>::shape()
{
	p2d::Circle * c = new p2d::Circle();
	c->setRadius(4);
	return c;
}

Sprite2DAdapter::Options * sad::p2d::app::Constants<Ball>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(0,88),hPointF(88,174));
	o->Rectangle = hRectF(hPointF(-11, -11), hPointF(11, 11));
	return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<Ball>::shape()
{
	sad::p2d::Circle * c = new sad::p2d::Circle();
	c->setRadius(11);
	return c;
}

Sprite2DAdapter::Options * sad::p2d::app::Constants<GridNode>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(178,90),hPointF(265,177));
	o->Rectangle = hRectF(hPointF(-18, -18), hPointF(18, 18));
	return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<GridNode>::shape()
{
	sad::p2d::Circle * c = new sad::p2d::Circle();
	c->setRadius(18);
	return c;
}


Sprite2DAdapter::Options * sad::p2d::app::Constants<GridNodeEdge>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(89,89),hPointF(176,111));
	o->Rectangle = p2d::app::Constants<GridNodeEdge>::sourceRect();
	return o;
}

hRectF sad::p2d::app::Constants<GridNodeEdge>::sourceRect()
{
	return hRectF(hPointF(-43, -11), hPointF(43, 11));
}


Sprite2DAdapter::Options * sad::p2d::app::Constants<Platform>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(267,91),hPointF(437,132));
	o->Rectangle = hRectF(hPointF(-42, -10), hPointF(42, 10));
	return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<Platform>::shape()
{
	Sprite2DAdapter::Options * options = p2d::app::Constants<Platform>::sprite();
	hRectF r = options->Rectangle;
	delete options;

	// For a simple optimization, make bonus a circle
	p2d::Rectangle * c = new p2d::Rectangle();
	c->setRect(r);
	return c;
}


Sprite2DAdapter::Options * sad::p2d::app::Constants<Shooter>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(177,0),hPointF(265,88));
	o->Rectangle = hRectF(hPointF(-10, -10), hPointF(10, 10));
	return o;
}


sad::p2d::CollisionShape * sad::p2d::app::Constants<Shooter>::shape()
{
	p2d::Circle * c = new p2d::Circle();
	c->setRadius(20);
	return c;
}

Sprite2DAdapter::Options * sad::p2d::app::Constants<ColoredBullet>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(439,14),hPointF(458,33));
	o->Rectangle = hRectF(hPointF(-4, -4), hPointF(4, 4));
	return o;
}


sad::p2d::CollisionShape * sad::p2d::app::Constants<ColoredBullet>::shape()
{
	sad::p2d::Circle * c = new sad::p2d::Circle();
	c->setRadius(2);
	return c;
}


Sprite2DAdapter::Options * sad::p2d::app::Constants<MovingSmile>::sprite()
{
	Sprite2DAdapter::Options * o = new Sprite2DAdapter::Options();
	o->Texture = "objects";
	o->TextureRectangle = hRectF(hPointF(177,0),hPointF(265,88));
	o->Rectangle = hRectF(hPointF(-14, -14), hPointF(14, 14));
	return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<MovingSmile>::shape()
{
	sad::p2d::Circle * c = new sad::p2d::Circle();
	c->setRadius(14);
	return c;
}
