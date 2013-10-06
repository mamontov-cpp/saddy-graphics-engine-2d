#include "coloredbullet.h"

DECLARE_SOBJ_INHERITANCE(ColoredBullet, sad::p2d::app::Object)

ColoredBullet::ColoredBullet()
{
	this->initFromConstants<ColoredBullet>();
	this->body()->setWeight( sad::p2d::Weight::constant(3.0) );
}

