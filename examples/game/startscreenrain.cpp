#include "startscreenrain.h"
#include "game.h"
#include "gameobject.h"
#include <renderer.h>

StartScreenRain::StartScreenRain(Game * g) : PeriodicalEvent(),  m_game(g)
{
    setInterval(1000);
}

StartScreenRain::~StartScreenRain()
{
    
}

void StartScreenRain::perform()
{
    int ocount = sad::Renderer::ref()->scenes()[0]->objectCount();
        if (m_game->isPaused() == false &&  ocount < 60)
    {
        // Compute position for new object
        double padding  = 12; // A small paddings out of screen borders
        double xmin = padding,  ymin = padding; 
        double xmax = sad::Renderer::ref()->settings().width() - padding;
        double ymax = sad::Renderer::ref()->settings().height() - padding;
        double x = padding, y = ymax - padding;
        if (rand() % 4 > 1 )
        {x=((double)rand()/RAND_MAX)*(xmax-xmin)+xmin; }
       else
        {y=(((double)rand()/RAND_MAX/2)+0.5f)*(ymax-ymin)+ymin; }
        // Produce object, set new position and speed
        GameObject * o = m_game->produce( (Objects) (rand() % 4) );
        if (o)
        {
            o->setPosition(sad::p2d::Point(x, y));
            o->setHorizontalSpeed(50.5);
            o->setVerticalSpeed(-50.5);
        }
    }
}
