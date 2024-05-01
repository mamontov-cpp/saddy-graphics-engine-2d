#include "p2d/collisiondetector.h"
#include "opticksupport.h"

DECLARE_SOBJ(sad::p2d::CollisionDetector);

sad::p2d::CollisionDetector::~CollisionDetector()
{
    PROFILER_EVENT;
}

int sad::p2d::CollisionDetector::sampleCount() const
{
    PROFILER_EVENT;
    return 1;
}
