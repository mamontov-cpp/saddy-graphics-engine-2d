#include "enemy.h"

DECLARE_SOBJ_INHERITANCE(Enemy, GameObject)

Enemy::Enemy()
{
    initFromConstants(this);
    this->setAngularVelocity(1.0);
}
