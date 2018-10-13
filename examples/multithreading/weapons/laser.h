/*! \file laser.h

    A laser projectile
 */
#pragma once
#include <sprite2d.h>
#include <timer.h>
#include "projectile.h"

class Game;

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
        \param[in] icon an icon
        \param[in] angle an angle
        \param[in] width a width of beam
        \param[in] height a height of beam
        \param[in] time a time for a laser
        \param[in] is_player whether swing is from player's weapon
     */
    Laser(Game* game, game::Actor* actor, const sad::String& icon, double angle, double width, double height, double time, bool is_player = true);
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
