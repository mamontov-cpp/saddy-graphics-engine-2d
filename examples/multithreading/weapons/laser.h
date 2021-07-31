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
    virtual game::Actor* actor() const override;
    /*! Kills a swing
     */
    virtual void kill() override;
    /*! Updates side for a swing
     */
    virtual void update() override;
private:
    /*! Constructs rectangle for laser
     *  \param[in] middle a middle data
     *  \param[in] width a width of beam
     *  \param[in] height a height of beam
     *  \param[in] angle an angle for laser
     *  \return rectangle
     */
    static sad::Rect2D rect(const sad::Point2D& middle, double width, double height, double angle);
    /*! A local actor
     */
    game::Actor* m_actor;
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
    /*! A delta angle for placing laser
     */
    double m_dangle;
    /*! A sprite width for laser
     */
    double m_sprite_width;
    /*! A physical width for laser
     */
    double m_physical_width;
    /*! A height for laser
     */
    double m_height;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(weapons::Laser);
