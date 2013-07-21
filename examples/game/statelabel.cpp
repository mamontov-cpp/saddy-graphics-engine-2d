#include "statelabel.h"
#include <renderer.h>
#include <log/log.h>
#include "game.h"
#include "player.h"

/*! External game 
 */
extern Game * PlayingGame;

// Declare it as inheritor of Label
DECLARE_SOBJ_INHERITANCE(StateLabel,Label)

StateLabel::StateLabel(int mode,const hst::string & s)
{
	// Extract font with label
	this->font()=sad::Renderer::ref()->fonts()->get(s);
	// Pick rendering function, depending of mode (if highscore render, score, otherwise render game state)
	if (mode==HIGHSCORE)
	{
		m_render=&StateLabel::renderScore;
	}
	else
	{
		m_render=&StateLabel::renderState;
	}
	// Set render point to mode
	this->render_point()=this->point();
	m_lastcl=0;
}

StateLabel::~StateLabel()
{
	// Does nothing
}
void StateLabel::render()
{
	// Render function only calls specified callback
	(this->*m_render)();

}

void StateLabel::renderScore()
{
  this->point()=pointf(HIGHSCORE_X,HIGHSCORE_Y);
  this->render_point()=this->point();
  if (clock()-m_lastcl>UPDATE_FREQ)
	{
		m_lastcl=clock();
		hst::string s = str(fmt::Format("Highscore: {0}") << PlayingGame->highscore());
		this->string()= s;	
	}
  //We must clear MODELVIEW, so transformation won't change
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  this->Label::render();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}
void StateLabel::renderState()
{
  this->point()=pointf(STATE_X,STATE_Y);
  this->render_point()=this->point();
  if (clock()-m_lastcl>UPDATE_FREQ)
	{
		m_lastcl=clock();
		hst::string s = str(fmt::Format("Health: {0} Score: {1} Highscore: {2}")
									   << PlayingGame->player()->health()
									   << PlayingGame->player()->score()
									   << PlayingGame->highscore()
		);
		string() = s;	
	}
  //We must clear MODELVIEW, so transformation won't change
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  this->Label::render();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}
