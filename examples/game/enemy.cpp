#include "enemy.h"

DECLARE_SOBJ_INHERITANCE(Enemy, GameObject)

Enemy::Enemy()
{
    this->initFromConstants<Enemy>();
    this->setAngularVelocity(1.0);
}
