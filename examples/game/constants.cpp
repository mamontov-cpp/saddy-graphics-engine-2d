#include "constants.h"
#include <cstdlib>

sad::Sprite2D::Options * sad::p2d::app::Constants<Player>::sprite()
{
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = sad::Rect2D(sad::Point2D(0,88),sad::Point2D(88,174));
    o->Rectangle = sad::Rect2D(sad::Point2D(-11, -11), sad::Point2D(11, 11));
    return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<Player>::shape()
{
    sad::p2d::Circle * c = new sad::p2d::Circle();
    c->setRadius(11);
    return c;
}

sad::Sprite2D::Options * sad::p2d::app::Constants<PlayerBullet>::sprite()
{
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = sad::Rect2D(sad::Point2D(441,0),sad::Point2D(452,11));
    o->Rectangle = sad::Rect2D(sad::Point2D(-4, -4), sad::Point2D(4, 4));
    return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<PlayerBullet>::shape()
{
    sad::p2d::Circle * c = new sad::p2d::Circle();
    c->setRadius(4);
    return c;
}

double sad::p2d::app::Constants<PlayerBullet>::velocity()
{
    return 121;
}

double sad::p2d::app::Constants<PlayerBullet>::interval()
{
    return 400;
}

sad::Sprite2D::Options * sad::p2d::app::Constants<EnemyBullet>::sprite()
{
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = sad::Rect2D(sad::Point2D(439,14),sad::Point2D(458,33));
    o->Rectangle = sad::Rect2D(sad::Point2D(-4, -4), sad::Point2D(4, 4));
    return o;
}

double sad::p2d::app::Constants<EnemyBullet>::velocity()
{
    return 121;
}

double sad::p2d::app::Constants<EnemyBullet>::interval()
{
    return 400;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<EnemyBullet>::shape()
{
    sad::p2d::Circle * c = new sad::p2d::Circle();
    c->setRadius(4);
    return c;
}

sad::Sprite2D::Options * sad::p2d::app::Constants<Bonus>::sprite()
{
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = sad::Rect2D(sad::Point2D(1,1),sad::Point2D(86,86));
    o->Rectangle = sad::Rect2D(sad::Point2D(-8, -8), sad::Point2D(8, 8));
    return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<Bonus>::shape()
{
    sad::Sprite2D::Options * options = p2d::app::Constants<Bonus>::sprite();
    sad::Rect2D r = options->Rectangle;
    delete options;

    // For a simple optimization, make bonus a circle
    sad::p2d::Rectangle * c = new sad::p2d::Rectangle();
    c->setRect(r);
    return c;
}

sad::Sprite2D::Options * sad::p2d::app::Constants<Enemy>::sprite()
{
    /*! A special texture rectangles, used to shape enemies
     */
    sad::Rect2D texture_rectangles[3]={
        sad::Rect2D(sad::Point2D(88,0),sad::Point2D(176,88)),
        sad::Rect2D(sad::Point2D(266,0),sad::Point2D(354,88)),
        sad::Rect2D(sad::Point2D(355,0),sad::Point2D(443,88)),
    };
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = texture_rectangles[ rand () % 3 ];
    o->Rectangle = sad::Rect2D(sad::Point2D(-8, -8), sad::Point2D(8, 8));
    return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<Enemy>::shape()
{
    sad::p2d::Circle * c = new sad::p2d::Circle();
    c->setRadius(8);
    return c;
}

sad::Sprite2D::Options * sad::p2d::app::Constants<ShootingEnemy>::sprite()
{
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = sad::Rect2D(sad::Point2D(177,0),sad::Point2D(265,88));
    o->Rectangle = sad::Rect2D(sad::Point2D(-10, -10), sad::Point2D(10, 10));
    return o;
}

double sad::p2d::app::Constants<ShootingEnemy>::velocity()
{
    return 100;
}

double sad::p2d::app::Constants<ShootingEnemy>::interval()
{
    return 1000;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<ShootingEnemy>::shape()
{
    sad::p2d::Circle * c = new sad::p2d::Circle();
    c->setRadius(10);
    return c;
}


sad::Sprite2D::Options * sad::p2d::app::Constants<SuperShootingEnemy>::sprite()
{
    sad::Sprite2D::Options * o = new sad::Sprite2D::Options();
    o->Texture = "objects";
    o->TextureRectangle = sad::Rect2D(sad::Point2D(177,0),sad::Point2D(265,88));
    o->Rectangle = sad::Rect2D(sad::Point2D(-14, -14), sad::Point2D(14, 14));
    return o;
}

sad::p2d::CollisionShape * sad::p2d::app::Constants<SuperShootingEnemy>::shape()
{
    p2d::Circle * c = new p2d::Circle();
    c->setRadius(14);
    return c;
}

