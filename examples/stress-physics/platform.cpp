#include "platform.h"

#include <p2d/rectangle.h>

DECLARE_SOBJ_INHERITANCE(Platform, sad::p2d::app::Object)

Platform::Platform()
{
    const sad::Rect2D area(sad::Point2D(-42, -10), sad::Point2D(42, 10));

    sad::Sprite2D::Options options(
        "objects",
        sad::Rect2D(sad::Point2D(267, 91), sad::Point2D(437, 132)),
        area
    );
    this->setOptions(&options);
    this->setShape(new sad::p2d::Rectangle(area));
}
