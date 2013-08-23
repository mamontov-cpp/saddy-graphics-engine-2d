/*! \file enemyspawn.h
	\author HiddenSeeker

	Describes an spawned enemy waves at specified times
 */
#include <input.h>
#include <periodicalevent.h>
#pragma once

class Game;

/*! Descibes a rain of objects in start screen
 */ 
class EnemySpawn: public PeriodicalEvent
{
 protected:
	 Game * m_game; //!< A game which rain is linked to

	 /*! Adds a new objects
	  */
	 virtual void perform();
 public:
	 /*! Creates new spawn, linked to specified game
	  */
	 EnemySpawn(Game * g);
};

