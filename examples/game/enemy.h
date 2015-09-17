/*! \file enemy.h
    

    Describes an enemy
 */
#pragma once
#include "gameobject.h"


/*! As a default, enemy is a simple object with settings from
    constant
 */
class Enemy: public GameObject
{
 SAD_OBJECT
 public:
     Enemy();
};
