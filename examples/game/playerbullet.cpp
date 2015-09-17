#include "playerbullet.h"

DECLARE_SOBJ_INHERITANCE(PlayerBullet, GameObject)

PlayerBullet::PlayerBullet()
{
    this->initFromConstants<PlayerBullet>();
}


