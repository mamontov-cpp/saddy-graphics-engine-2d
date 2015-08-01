/*! \file playerbullet.h
	

	Describes a bullet owned by player
 */
#pragma once
#include "gameobject.h"


/*! As a default, player bullet is a simple object with settings from
    constant
 */
class PlayerBullet: public GameObject
{
 SAD_OBJECT
 public:
	 PlayerBullet();
};

