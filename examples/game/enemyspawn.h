/*! \file enemyspawn.h
    

    Describes an spawned enemy waves at specified times
 */
#pragma once
#include <periodicalevent.h>


class Game;

/*! Describes a rain of objects in start screen
 */ 
class EnemySpawn: public sad::PeriodicalEvent
{
 protected:
     Game * m_game; //!< A game which rain is linked to

     /*! Adds a new objects
      */
     virtual void perform() override;
 public:
     /*! Creates new spawn, linked to specified game
      */
     EnemySpawn(Game * g);
};

