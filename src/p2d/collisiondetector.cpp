#include "p2d/collisiondetector.h"

p2d::CollisionDetector::~CollisionDetector()
{
}

int p2d::CollisionDetector::sampleCount() const
{
	return 1;
}
