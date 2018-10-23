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
    /*! Returns whether bullets should go through platforms
        \return whether bullets should go through platforms
     */
    bool isGhost() const;
    /*! Sets, whether bullets should go through platforms
        \param is_ghost whether bullets should go through platforms
     */
    void setIsGhost(bool is_ghost);
    /*! Sets amount of bounces, that can be performed by bullet
        \param[in] amount amount of bounces
     */
    void setBounceCountLeft(int amount);
    /*! Returns amount of bounces, that can be performed by bullet
        \return amount of bounces
     */
    int bounceCountLeft() const;
    /*! Sets resilience coefficience for bullet
        \param[in] value a value for resilience
     */
    void setBounceResilienceCoefficient(double value);
    /*! Returns resilience coefficient
        \return resilience coefficient
     */
    double bounceResilienceCoefficient() const;
    /*! Sets value, whether bullet is piercing
        \param[in] value a value
     */
    void setIsPiercing(bool value);
    /*! Returns whether bullet is piercing
        \return whether is piercing
     */
    bool isPiercing() const;
protected:
    /*! An inner sprite
     */
    sad::Sprite2D* m_sprite;
    /*! Whether bullet should go through platforms
     */
    bool m_is_ghost;
    /*! Bounce count left for bullet, before decay
     */
    int m_bounce_count_left;
    /*! A bounce resilience coefficient
     */
    double m_bounce_resilience_coefficient;
    /*! Whether bullet is piercing
     */
    bool m_is_piercing;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(weapons::Bullet)
