#include "statelabel.h"
#include "../renderer.h"
SAD_DECLARE(StateLabel,Label)

StateLabel::StateLabel(int what,const hst::string & s)
{
	this->font()=static_cast<sad::TMFont *>(sad::Renderer::ref()->fonts()->get(s));
	if (what==HIGHSCORE)
	{
		m_render=&StateLabel::renderScore;
	}
	else
	{
		m_render=&StateLabel::renderState;
	}
	this->render_point()=this->point();
	m_lastcl=0;
}

StateLabel::~StateLabel()
{

}
void StateLabel::render()
{
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
