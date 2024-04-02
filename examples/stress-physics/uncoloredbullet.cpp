#include "uncoloredbullet.h"

#include <p2d/circle.h>

DECLARE_SOBJ_INHERITANCE(UncoloredBullet, sad::p2d::app::Object)

UncoloredBullet::UncoloredBullet()
{
    sad::Sprite2D::Options options(
        "objects",
        sad::Rect2D(sad::Point2D(441, 0), sad::Point2D(452, 11)),
        sad::Rect2D(sad::Point2D(-4, -4), sad::Point2D(4, 4))
    );
    this->setOptions(&options);
    this->setShape(new sad::p2d::Circle(4));
    this->body()->setWeight( sad::p2d::Weight::constant(0.3) );
}

