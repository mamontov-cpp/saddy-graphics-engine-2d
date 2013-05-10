/*!  \file    statelabel.h
     \author  HiddenSeeker

	 Contains a definition of state label
*/
#include "../fontmanager.h"
#include "statemachine.h"
#include "../scene.h"
#include "../label.h"
#include <time.h>
#pragma once

#define HIGHSCORE    0
#define PLAYERSTATE  1
#define HIGHSCORE_X 260
#define HIGHSCORE_Y 240
#define STATE_X  0
#define STATE_Y  480
#define UPDATE_FREQ 500

class StateLabel: public Label
{
 SAD_NODE
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
