/*! \file swing.h
 
    A simple bullet-like  object, which implements weapon swing
 */
#pragma once
#include <sprite2d.h>
#include <timer.h>
#include "projectile.h"
#include "swingsettings.h"

class Game;

namespace weapons
{
/*! Makes sword swing
 */
class Swing: public weapons::Projectile
{
SAD_OBJECT
    /*! An angle start for left
     */
    static const double LeftAngleStart;
    /*! An angle start for left
     */
    static const double LeftAngleEnd;
    /*! An angle start for right
     */
    static const double RightAngleStart;
    /*! An angle start for right
     */
    static const double RightAngleEnd;

    /*! Constructs a swing
        \param[in] game game
        \param[in] actor an actor
        \param[in] settings a setting for swing
     */
    Swing(Game* game, game::Actor* actor,const weapons::SwingSettings& settings);
    /*! Destroys swing object
     */
    virtual ~Swing();
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
    /*! A left part
     */
    bool m_left;
    /*! A sing sprite
     */
    sad::Sprite2D* m_swing_sprite;
    /*! A body for sprite
     */
    sad::p2d::Body* m_body;
    /*! An actor, from which we should remove a swing
     */
    game::Actor* m_attached_actor;
    /*! A timer objects
     */
    sad::Timer m_timer;
    /*! A maximal time for swing
     */
    double m_max_time;
    /*! A local width
     */
    double m_local_width;
    /*! A local height
     */
    double m_local_height;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(weapons::Swing);
