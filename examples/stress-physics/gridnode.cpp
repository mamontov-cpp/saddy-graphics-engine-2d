#include "gridnode.h"
#include <p2d/circle.h>

DECLARE_SOBJ_INHERITANCE(GridNode, sad::phy52d::Object)

GridNode::GridNode()
{
    sad::Sprite2D::Options options(
        "objects",
        sad::Rect2D(sad::Point2D(178, 90), sad::Point2D(265, 177)),
        sad::Rect2D(sad::Point2D(-18, -18), sad::Point2D(18, 18))
    );
    this->setOptions(&options);
    this->setShape(new sad::p2d::Circle(18));
}
