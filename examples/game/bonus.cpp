#include "bonus.h"

DECLARE_SOBJ_INHERITANCE(Bonus, GameObject)

Bonus::Bonus()
{
    initFromConstants(this);
    this->setAngularVelocity(1.0);
}
