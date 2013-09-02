/*! \file enemy.h
	\author HiddenSeeker

	Describes an enemy
 */
#include "gameobject.h"
#include "gameobjectconstants.h"
#pragma once

/*! As a default, enemy is a simple object with settings from
    constant
 */
class Enemy: public GameObject
{
 SAD_OBJECT
 public:
	 Enemy();
};
