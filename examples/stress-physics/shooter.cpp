#include "shooter.h"

DECLARE_SOBJ_INHERITANCE(Shooter, p2d::app::Object)

Shooter::Shooter()
{
	this->initFromConstants<Shooter>();
}
