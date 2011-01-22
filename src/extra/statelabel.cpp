#include "statelabel.h"

int StateLabel::Type=3;

StateLabel::StateLabel(int what,const hst::string & s)
{
	m_fnt=static_cast<sad::TMFont *>(sad::FontManager::get(s));
	if (what==HIGHSCORE)
	{
		m_render=&StateLabel::renderScore;
	}
	else
	{
		m_render=&StateLabel::renderState;
	}
	m_lastcl=0;
	m_type=StateLabel::Type;
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
		m_str=hst::string("Highscore: ")+hst::string::number(high_score);	
	}
  hRectF rt(hPointF(HIGHSCORE_X,HIGHSCORE_Y),
	            hPointF(HIGHSCORE_X+SYM_WIDTH*m_str.length(),
				HIGHSCORE_Y+SYM_HEIGHT));
  m_fnt->render(m_str,rt,ZAX);
}
void StateLabel::renderState()
{
  if (clock()-m_lastcl>UPDATE_FREQ)
	{
		m_lastcl=clock();
		m_str.clear();
		m_str2.clear();
		m_str3.clear();
		m_str<<hst::string("Health: ")<<hst::string::number(player_health_point);
		m_str2<<hst::string("Score: ")<<hst::string::number(current_score);
		m_str3<<hst::string("Highscore: ")<<hst::string::number(high_score);	
	}
  double flen=PSTATE_X+SYM_WIDTH*m_str.length();
  double fy=PSTATE_Y+SYM_HEIGHT;
  hRectF rt1(hPointF(PSTATE_X,PSTATE_Y),hPointF(flen,fy));
  flen+=SYM_WIDTH*2;
  hRectF rt2(hPointF(flen,PSTATE_Y),hPointF(flen+SYM_WIDTH*m_str2.length(),fy));
  flen+=SYM_WIDTH*(m_str2.length()+2);
  hRectF rt3(hPointF(flen,PSTATE_Y),hPointF(flen+SYM_WIDTH*m_str3.length(),fy));

  m_fnt->render(m_str,rt1,ZAX);
  m_fnt->render(m_str2,rt2,ZAX);
  m_fnt->render(m_str3,rt3,ZAX);

}
