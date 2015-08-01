/*! \file enemybullet.h
	

	Describes a bullet owned by enemy
 */
#pragma once
#include "gameobject.h"


/*! As a default, enemy bullet is a simple object with settings from
    constant
 */
class EnemyBullet: public GameObject
{
 SAD_OBJECT
 public:
	 EnemyBullet();
};

