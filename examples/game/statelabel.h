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

/*! A mode for rendering high scores of player
 */
#define HIGHSCORE    0
/*! A mode for rendering current in-game state
 */
#define PLAYERSTATE  1
/*! An x coordinate 
 */
#define HIGHSCORE_X 260
#define HIGHSCORE_Y 240
#define STATE_X  0
#define STATE_Y  480
/*! A frequence, how often should label text be updated
 */
#define UPDATE_FREQ 500

class StateLabel: public Label
{
 SAD_OBJECT
 private:
	     void (StateLabel::* m_render)();
		 clock_t m_lastcl;
 public:
	     StateLabel(int what,const hst::string & s);

		 void renderScore();
		 void renderState();

		 void render();
		
		 ~StateLabel();
};
