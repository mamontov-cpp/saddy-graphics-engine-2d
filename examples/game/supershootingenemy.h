/*! \file supershootingenemy.h
	\author HiddenSeeker

	Describes an enemy, that shoots with shooting enemies in four directions
 */
#include "gameobject.h"
#pragma once

/*! Shooting enemy shoots with one enemy bullet
 */
class SuperShootingEnemy: public GameObject
{
 SAD_OBJECT
 public:
	 SuperShootingEnemy();
};
