#include "enemyspawn.h"
#include "game.h"
#include "gameobject.h"
#include <renderer.h>

EnemySpawn::EnemySpawn(Game * g) : m_game(g)
{
	setInterval(1000);
}

void EnemySpawn::perform()
{
	if (m_game->isPaused() == false)
	{
		// Compute position for new object
		double padding  = 12; // A small paddings out of scren borders
		double xmin = padding,  ymin = padding; 
		double xmax = sad::Renderer::ref()->settings().width() - padding;
		double ymax = sad::Renderer::ref()->settings().height() - padding;
		double x =0 , y = ymax;
		x=((double)rand()/RAND_MAX)*(xmax - xmin) + xmin; 
	    y=((double)rand()/RAND_MAX)*(ymax - ymin) + ymin; 

		double vx=(float)rand()/RAND_MAX * 250 - 150;
		double vy=(float)rand()/RAND_MAX * 250 - 150;
 
		// Produce object, set new position and speed
		GameObject * o = m_game->produce( (Objects) (rand() % 4) );
		if (o)
		{
			o->setPosition(p2d::Point(x, y));
			o->setHorizontalSpeed(vx);
			o->setVerticalSpeed(vy);
		}
	}
}
