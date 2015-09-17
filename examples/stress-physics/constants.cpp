#include "constants.h"

#include <cstdlib>


sad::Sprite2D::Options * sad::p2d::app::Constants<UncoloredBullet>::sprite()
{
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = sad::Rect2D(sad::Point2D(441,0),sad::Point2D(452,11));
    o->Rectangle = sad::Rect2D(sad::Point2D(-4, -4), sad::Point2D(4, 4));
    return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<UncoloredBullet>::shape()
{
    p2d::Circle * c = new p2d::Circle();
    c->setRadius(4);
    return c;
}

sad::Sprite2D::Options * sad::p2d::app::Constants<Ball>::sprite()
{
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = sad::Rect2D(sad::Point2D(0,88),sad::Point2D(88,174));
    o->Rectangle = sad::Rect2D(sad::Point2D(-11, -11), sad::Point2D(11, 11));
    return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<Ball>::shape()
{
    sad::p2d::Circle * c = new sad::p2d::Circle();
    c->setRadius(11);
    return c;
}

sad::Sprite2D::Options * sad::p2d::app::Constants<GridNode>::sprite()
{
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = sad::Rect2D(sad::Point2D(178,90),sad::Point2D(265,177));
    o->Rectangle = sad::Rect2D(sad::Point2D(-18, -18), sad::Point2D(18, 18));
    return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<GridNode>::shape()
{
    sad::p2d::Circle * c = new sad::p2d::Circle();
    c->setRadius(18);
    return c;
}


sad::Sprite2D::Options * sad::p2d::app::Constants<GridNodeEdge>::sprite()
{
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = sad::Rect2D(sad::Point2D(89,89),sad::Point2D(176,111));
    o->Rectangle = p2d::app::Constants<GridNodeEdge>::sourceRect();
    return o;
}

sad::Rect2D sad::p2d::app::Constants<GridNodeEdge>::sourceRect()
{
    return sad::Rect2D(sad::Point2D(-43, -11), sad::Point2D(43, 11));
}


sad::Sprite2D::Options * sad::p2d::app::Constants<Platform>::sprite()
{
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = sad::Rect2D(sad::Point2D(267,91),sad::Point2D(437,132));
    o->Rectangle = sad::Rect2D(sad::Point2D(-42, -10), sad::Point2D(42, 10));
    return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<Platform>::shape()
{
    sad::Sprite2D::Options * options = p2d::app::Constants<Platform>::sprite();
    sad::Rect2D r = options->Rectangle;
    delete options;

    // For a simple optimization, make bonus a circle
    p2d::Rectangle * c = new p2d::Rectangle();
    c->setRect(r);
    return c;
}


sad::Sprite2D::Options * sad::p2d::app::Constants<Shooter>::sprite()
{
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = sad::Rect2D(sad::Point2D(177,0),sad::Point2D(265,88));
    o->Rectangle = sad::Rect2D(sad::Point2D(-10, -10), sad::Point2D(10, 10));
    return o;
}


sad::p2d::CollisionShape * sad::p2d::app::Constants<Shooter>::shape()
{
    p2d::Circle * c = new p2d::Circle();
    c->setRadius(20);
    return c;
}

sad::Sprite2D::Options * sad::p2d::app::Constants<ColoredBullet>::sprite()
{
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = sad::Rect2D(sad::Point2D(439,14),sad::Point2D(458,33));
    o->Rectangle = sad::Rect2D(sad::Point2D(-4, -4), sad::Point2D(4, 4));
    return o;
}


sad::p2d::CollisionShape * sad::p2d::app::Constants<ColoredBullet>::shape()
{
    sad::p2d::Circle * c = new sad::p2d::Circle();
    c->setRadius(2);
    return c;
}


sad::Sprite2D::Options * sad::p2d::app::Constants<MovingSmile>::sprite()
{
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = sad::Rect2D(sad::Point2D(177,0),sad::Point2D(265,88));
    o->Rectangle = sad::Rect2D(sad::Point2D(-14, -14), sad::Point2D(14, 14));
    return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<MovingSmile>::shape()
{
    sad::p2d::Circle * c = new sad::p2d::Circle();
    c->setRadius(14);
    return c;
}
