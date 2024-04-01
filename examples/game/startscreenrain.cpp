#include "startscreenrain.h"
#include "game.h"
#include "gameobject.h"
#include <renderer.h>

StartScreenRain::StartScreenRain(Game * g) : PeriodicalEvent(),  m_game(g)
{
    setInterval(1000);
}

StartScreenRain::~StartScreenRain() = default;

/*! Returns random double value in certain interval. Defines in enemy_spawn.cpp
 *  \param[in] val_min minimal value
 *  \param[in] val_max maximal value
 *  \return result
 */
double random_double(double val_min, double val_max);

void StartScreenRain::perform()
{
    const int object_count =  m_game->renderer()->scenes()[0]->objectCount();
    const int max_object_count = 60;
    if (m_game->isPaused() == false &&  object_count < max_object_count)
    {
        // Compute position for new object
        const double padding  = 12; // A small paddings out of screen borders
        const double x_min = padding,  y_min = padding; 
        const double x_max = m_game->renderer()->settings().width() - padding;
        const double y_max = m_game->renderer()->settings().height() - padding;
        double x = padding, y = y_max - padding;
        if (rand() % 4 > 1 )
        {
            x = random_double(x_min, x_max);
        }
        else
        {
            y = random_double(y_min, y_max);
        }
        // Produce object, set new position and speed
        GameObject * o = m_game->produce( static_cast<Objects>(rand() % 4) );
        if (o)
        {
            o->setPosition(sad::p2d::Point(x, y));
            const double horizontal_speed = 50.5, vertical_speed = -50.5;
            o->setHorizontalSpeed(horizontal_speed);
            o->setVerticalSpeed(vertical_speed);
        }
    }
}
