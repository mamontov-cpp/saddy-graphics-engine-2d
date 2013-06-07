#include "statelabel.h"
#include "../renderer.h"

// We put some game state data to extern to use it, when rendering state of label
extern int high_score, current_score;
extern int player_health_point;
extern bool paused;

// Declare it as inheritor of Label
SAD_DECLARE(StateLabel,Label)

StateLabel::StateLabel(int mode,const hst::string & s)
{
	// Extract font with label
	this->font()=static_cast<sad::TMFont *>(sad::Renderer::ref()->fonts()->get(s));
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
		this->string()=hst::string("Highscore: ")+hst::string::number(high_score);	
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
		string().clear();
		string()<<hst::string("Health: ")<<hst::string::number(player_health_point);
		string()<<hst::string("  Score: ")<<hst::string::number(current_score);
		string()<<hst::string("  Highscore: ")<<hst::string::number(high_score);	
	}
  //We must clear MODELVIEW, so transformation won't change
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  this->Label::render();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}
