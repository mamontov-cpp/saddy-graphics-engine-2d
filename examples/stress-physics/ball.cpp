#include "ball.h"

DECLARE_SOBJ_INHERITANCE(Ball, sad::p2d::app::Object)

Ball::Ball()
{
	this->initFromConstants<Ball>();
}
