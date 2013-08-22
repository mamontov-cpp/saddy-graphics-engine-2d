/*!  \file    statelabel.h
     \author  HiddenSeeker

	 Contains a definition of label, which, depending on mode can show 
	 in-game state or current player highscore
*/
#include "../fontmanager.h"
#include "statemachine.h"
#include "../scene.h"
#include "../label.h"
#include <time.h>
#pragma once

/*! An x coordinate for representing a label at highscore screen 
 */
#define HIGHSCORE_X 260
/*! An y coordinate for representing a label at highscore screen
 */
#define HIGHSCORE_Y 240
/*! An x coordinare for representing a label at in-game screen
 */
#define STATE_X  0
/*! An y coordinate for representing a label at in-game screen
 */
#define STATE_Y  480
/*! A frequence, how often should label text be updated
 */
#define UPDATE_FREQ 500

class Game;

/*! A state label represents an in-game status: highscore, lives or a
    score. 

	At start screen, state label should represent a highscore
 */
class StateLabel: public Label
{
 SAD_OBJECT
 private:
	     /*! A current text updationg method, that 
		     depending of state, renders
		     various data at game screen
		  */
	     void (StateLabel::* m_text)();
		 /*! A time, when last time label was up-to-date
		  */
		 clock_t m_last_updated;
	     /*! A game, where data is stored
		  */
		 Game * m_game;
		 /*! Sets text of label to current highscore
		  */
		 void updateHighscore();
		 /*! Sets text of label to current game status, that consists 
			 of score, hiscore and count of player lives
		  */
		 void updateGameStatus();
 public:
	     /*! Creates a label, basing on game state
			 \param[in]  g game state
		  */
	     StateLabel(Game * g);
		 void render();
		 /*! Does nothing, since the object does not own anything
		  */
		 ~StateLabel();
};
