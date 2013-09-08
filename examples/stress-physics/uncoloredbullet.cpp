#include "uncoloredbullet.h"

DECLARE_SOBJ_INHERITANCE(UncoloredBullet, p2d::app::Object)

UncoloredBullet::UncoloredBullet()
{
	this->initFromConstants<UncoloredBullet>();
	this->body()->setWeight( p2d::Weight::constant(0.3) );
}

