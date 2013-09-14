/*! \file enemybullet.h
	\author HiddenSeeker

	Describes a bullet owned by enemy
 */
#include "gameobject.h"
#pragma once

/*! As a default, enemy bullet is a simple object with settings from
    constant
 */
class EnemyBullet: public GameObject
{
 SAD_OBJECT
 public:
	 EnemyBullet();
};

