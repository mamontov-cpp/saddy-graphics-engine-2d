#include "bonus.h"

DECLARE_SOBJ_INHERITANCE(Bonus, GameObject)

Bonus::Bonus()
{
    this->initFromConstants<Bonus>();
    this->setAngularVelocity(1.0);
}
