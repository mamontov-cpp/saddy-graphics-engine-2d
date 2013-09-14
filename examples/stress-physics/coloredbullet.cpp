#include "coloredbullet.h"

DECLARE_SOBJ_INHERITANCE(ColoredBullet, p2d::app::Object)

ColoredBullet::ColoredBullet()
{
	this->initFromConstants<ColoredBullet>();
	this->body()->setWeight( p2d::Weight::constant(3.0) );
}

