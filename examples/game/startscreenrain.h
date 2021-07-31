/*! \file startscreenrain.h
    

    Describes a rain in start screen
 */
#pragma once
#include <periodicalevent.h>


class Game;

/*! Describes a rain of objects in start screen
 */ 
class StartScreenRain: public sad::PeriodicalEvent
{
 protected:
     Game * m_game; //!< A game which rain is linked to

     /*! Adds a new objects
      */
     virtual void perform() override;
 public:
     /*! Creates new rain, linked to specified game
      */
     StartScreenRain(Game * g);
     /*! Must be implemented to prevent memory leak
      */ 
     virtual ~StartScreenRain() override;
};


