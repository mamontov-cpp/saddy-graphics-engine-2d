#include "statelabel.h"
#include <renderer.h>
#include <log/log.h>
#include "game.h"
#include "player.h"


// Declare StateLabel as inheritor of Label
DECLARE_SOBJ_INHERITANCE(StateLabel,Label)

StateLabel::StateLabel(Game * g) : m_game(g)
{
	// Extract font with label
	this->font()=sad::Renderer::ref()->fonts()->get("times_lg");
	// Pick rendering function, depending of mode (if highscore render, score, otherwise render game state)
	if (g->state() == GameState::START)
	{
		// Make label render at specified place
		this->point()=pointf(HIGHSCORE_X,HIGHSCORE_Y);
		m_text=&StateLabel::updateHighscore;
	}
	else
	{
		// Make label render at specified place
		this->point()=pointf(STATE_X,STATE_Y);
		m_text=&StateLabel::updateGameStatus;
	}
	// Set rendering point to current point
	this->render_point()=this->point();
	m_lastcl=0;
}

StateLabel::~StateLabel()
{
	// Does nothing
}
void StateLabel::render()
{
	if (clock()-m_lastcl>UPDATE_FREQ)
	{
		m_last_updated = clock();
	   (this->*m_text)();
	}
	// We push and restore transformation to make sur, that no transformation
	// will affect label
	
	// You can remove this code, if you think no transformations will occur
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    this->Label::render();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void StateLabel::updateHighscore()
{
	hst::string s = str(fmt::Format("Highscore: {0}") << m_game->highscore());
	this->string()= s;	
}

void StateLabel::updateGameStatus()
{ 
  hst::string s = str(fmt::Format("Health: {0} Score: {1} Highscore: {2}")
					  << m_game->player()->health()
					  << m_game->player()->score()
					  << m_game->highscore()
  );
  string() = s;
}
