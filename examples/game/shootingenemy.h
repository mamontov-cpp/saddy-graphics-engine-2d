/*! \file shootingenemy.h
	\author HiddenSeeker

	Describes an enemy, that shoots with enemy bullets
 */
#include "gameobject.h"
#include "gameobjectconstants.h"
#pragma once

/*! Shooting enemy shoots with one enemy bullet
 */
class ShootingEnemy: public GameObject
{
 SAD_OBJECT
 public:
	 ShootingEnemy();
};
