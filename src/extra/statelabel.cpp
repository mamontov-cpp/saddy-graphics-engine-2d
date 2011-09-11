#include "statelabel.h"

SAD_DECLARE(StateLabel,Label)

StateLabel::StateLabel(int what,const hst::string & s)
{
	this->font()=static_cast<sad::TMFont *>(sad::FontManager::get(s));
	if (what==HIGHSCORE)
	{
		m_render=&StateLabel::renderScore;
		this->point()=pointf(HIGHSCORE_X,HIGHSCORE_Y);
	}
	else
	{
		this->point()=pointf(STATE_X,STATE_Y);
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
  if (clock()-m_lastcl>UPDATE_FREQ)
	{
		m_lastcl=clock();
		this->string()=hst::string("Highscore: ")+hst::string::number(high_score);	
	}
  this->Label::render();
}
void StateLabel::renderState()
{
  if (clock()-m_lastcl>UPDATE_FREQ)
	{
		m_lastcl=clock();
		string().clear();
		string()<<hst::string("Health: ")<<hst::string::number(player_health_point);
		string()<<hst::string("  Score: ")<<hst::string::number(current_score);
		string()<<hst::string("  Highscore: ")<<hst::string::number(high_score);	
	}
  this->Label::render();
}
