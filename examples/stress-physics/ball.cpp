#include "ball.h"

DECLARE_SOBJ_INHERITANCE(Ball, p2d::app::Object)

Ball::Ball()
{
	this->initFromConstants<Ball>();
}
