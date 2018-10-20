/*! \file bullet.h
    
    A list of bullets
 */
#pragma once
#include "projectile.h"

namespace weapons
{

class Bullet: public weapons::Projectile
{
SAD_OBJECT
public:
    /*! Creates new bullet
     */
    Bullet();
    /*! Destroys bullet
     */
    virtual ~Bullet();
    /*! Sets a sprite for bullet
        \param[in] sprite a sprite
     */
    void setSprite(sad::Sprite2D* sprite);
    /*! Returns a sprite
        \return sprite
     */
    sad::Sprite2D* sprite() const;
    /*! Moves sprite by specified vector
        \param[in] p point
     */
    void moveBy(const sad::Point2D& p) const;
    /*! Rotates a sprite by specified angle
        \param[in] da angle
     */
    void rotate(double da) const;
    /*! Returns true
        \return true
     */
    static bool canBeRotated();
    /*! Returns actor
        \return actor
     */
    virtual game::Actor* actor() const;
    /*! Kills an object
     */
    virtual void kill();
    /*! Updates side for a swing
     */
    virtual void update();
protected:
    /*! An inner sprite
     */
    sad::Sprite2D* m_sprite;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(weapons::Bullet)