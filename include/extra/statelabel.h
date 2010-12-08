/*!  \file    statelabel.h
     \author  HiddenSeeker

	 Contains a definition of state label
*/
#include "fontmanager.h"
#include "statemachine.h"
#include "scene.h"
#include <time.h>
#pragma once

#define HIGHSCORE    0
#define PLAYERSTATE  1
#define ZAX 0.5
#define HIGHSCORE_X -0.0224357  
#define HIGHSCORE_Y 0.00776622 
#define PSTATE_X    -0.271818
#define PSTATE_Y    0.19 
#define UPDATE_FREQ 500
#define SIMP_WIDTH 0.23
#define SIMP_HEIGHT 0.33
#define SYM_WIDTH  0.005
#define SYM_HEIGHT 0.0125
class StateLabel: public sad::BasicNode
{
 private:
	     void (StateLabel::* m_render)();
		 sad::TMFont * m_fnt;
		 hst::string m_str;
		 hst::string m_str2;
		 hst::string m_str3;
		 clock_t m_lastcl;
 public:
	     static int Type;
	     StateLabel(int what,const hst::string & s);

		 void renderScore();
		 void renderState();

		 void render();
		
		 ~StateLabel();
};