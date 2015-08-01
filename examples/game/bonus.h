/*! \file bonus.h
	

	Describes a bonus item
 */
#pragma once
#include "gameobject.h"


/*! As a default, bonus item is a simple object with settings from
    constant
 */
class Bonus: public GameObject
{
 SAD_OBJECT
 public:
	 Bonus();
};
