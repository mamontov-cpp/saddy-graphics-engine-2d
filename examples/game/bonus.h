/*! \file bonus.h
	\author HiddenSeeker

	Describes a bonus item
 */
#include "gameobject.h"
#include "gameobjectconstants.h"
#pragma once

/*! As a default, bonus item is a simple object with settings from
    constant
 */
class Bonus: public GameObject
{
 SAD_OBJECT
 public:
	 Bonus();
};
