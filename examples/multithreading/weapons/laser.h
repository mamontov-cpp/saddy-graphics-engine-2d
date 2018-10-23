/*! \file laser.h

    A laser projectile
 */
#pragma once
#include <sprite2d.h>
#include <timer.h>
#include "projectile.h"
#include "lasersettings.h"

class Game;

namespace game
{
class Actor;
}

namespace weapons
{

/*! A simple laser projectile
 */
class Laser : public weapons::Projectile
{
SAD_OBJECT
    /*! Constructs a laser
        \param[in] game game
        \param[in] actor an actor
        \param[in] angle an angle
        \param[in] settings a settings for laser
     */
    Laser(Game* game, game::Actor* actor, double angle, const weapons::LaserSettings& settings);
    /*! Destroys swing object
     */
    virtual ~Laser();
    /*! Returns actor
        \return actor
     */
    virtual game::Actor* actor() const;
    /*! Kills a swing
     */
    virtual void kill();
    /*! Updates side for a swing
     */
    virtual void update();
private:
    /*! A game
     */
    Game* m_game;
    /*! A local actor
     */
    game::Actor* m_actor;
    /*! A last actor middle point
     */
    sad::Point2D m_last_actor_middle;
    /*! A sing sprite
     */
    sad::Sprite2D* m_sprite;
    /*! A body for sprite
     */
    sad::p2d::Body* m_body;
    /*! A timer objects
     */
    sad::Timer m_timer;
    /*! A maximal time for swing
     */
    double m_max_time;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(weapons::Laser);
