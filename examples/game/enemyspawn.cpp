#include "enemyspawn.h"
#include "game.h"
#include "gameobject.h"
#include <renderer.h>

EnemySpawn::EnemySpawn(Game * g) : m_game(g)
{
    setInterval(1000);
}

/*! Returns random double value in certain interval
 *  \param[in] val_min minimal value
 *  \param[in] val_max maximal value
 *  \return result
 */
double random_double(double val_min, double val_max)
{
    return (static_cast<double>(rand()) / RAND_MAX) * (val_max - val_min) + val_min;
}

void EnemySpawn::perform()
{
    if (m_game->isPaused() == false)
    {
        // Compute position for new object
        const double padding  = 12; // A small paddings out of screen borders
        const double x_min = padding,  y_min = padding; 
        const double x_max = m_game->renderer()->settings().width() - padding;
        const double y_max = m_game->renderer()->settings().height() - padding;
        const double x = random_double(x_min, x_max), y = random_double(y_min, y_max);

        const double min_speed = -150.0;
        const double max_speed = 250.0;

        const double x_speed = random_double(min_speed, max_speed); 
        const double y_speed = random_double(min_speed, max_speed);
 
        // Produce object, set new position and speed
        GameObject * o = m_game->produce( static_cast<Objects>(rand() % 4) );
        if (o)
        {
            o->setPosition(sad::p2d::Point(x, y));
            o->setHorizontalSpeed(x_speed);
            o->setVerticalSpeed(y_speed );
        }
    }
}
