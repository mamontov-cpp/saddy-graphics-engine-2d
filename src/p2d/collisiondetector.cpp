#include "p2d/collisiondetector.h"

DECLARE_SOBJ(sad::p2d::CollisionDetector);

sad::p2d::CollisionDetector::~CollisionDetector()
{
}

int sad::p2d::CollisionDetector::sampleCount() const
{
    return 1;
}
