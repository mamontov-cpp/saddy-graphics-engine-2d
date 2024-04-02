#include "coloredbullet.h"

#include <p2d/circle.h>

DECLARE_SOBJ_INHERITANCE(ColoredBullet, sad::phy52d::Object)

ColoredBullet::ColoredBullet()
{
    sad::Sprite2D::Options options(
        "objects",
        sad::Rect2D(sad::Point2D(439, 14), sad::Point2D(458, 33)),
        sad::Rect2D(sad::Point2D(-4, -4), sad::Point2D(4, 4))
    );
    this->setOptions(&options);
    this->setShape(new sad::p2d::Circle(2));
    this->body()->setWeight( sad::p2d::Weight::constant(3.0) );
}

