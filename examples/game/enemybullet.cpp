#include "enemybullet.h"

DECLARE_SOBJ_INHERITANCE(EnemyBullet, GameObject)

EnemyBullet::EnemyBullet()
{
	this->initFromConstants<EnemyBullet>();
}


