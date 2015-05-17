#include "uncoloredbullet.h"

DECLARE_SOBJ_INHERITANCE(UncoloredBullet, sad::p2d::app::Object)

UncoloredBullet::UncoloredBullet()
{
	this->initFromConstants<UncoloredBullet>();
	this->body()->setWeight( sad::p2d::Weight::constant(0.3) );
}

