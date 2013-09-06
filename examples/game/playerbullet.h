/*! \file playerbullet.h
	\author HiddenSeeker

	Describes a bullet owned by player
 */
#include "gameobject.h"
#pragma once

/*! As a default, player bullet is a simple object with settings from
    constant
 */
class PlayerBullet: public GameObject
{
 SAD_OBJECT
 public:
	 PlayerBullet();
};

