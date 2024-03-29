/*! \file bullet.h
    
    A list of bullets
 */
#pragma once
#include "projectile.h"
#include "bulletsettings.h"


class Game;

namespace game
{
class Actor;
}

namespace weapons
{

class Bullet: public weapons::Projectile
{
SAD_OBJECT
public:
    /*! Constructs a bullet
        \param[in] game game
        \param[in] actor an actor
        \param[in] angle an angle
        \param[in] settings a settings for laser
     */
    Bullet(Game* game, game::Actor* actor, double angle, const weapons::BulletSettings& settings);
    /*! Destroys bullet
     */
    virtual ~Bullet() override;
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
    virtual game::Actor* actor() const override;
    /*! Kills an object
     */
    virtual void kill() override;
    /*! Updates side for a swing
     */
    virtual void update() override;
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
    /*! Sets restitution coefficient for bullet
        \param[in] value a value for restitution
     */
    void setBounceRestitutionCoefficient(double value);
    /*! Returns restitution coefficient
        \return restitution coefficient
     */
    double bounceRestitutionCoefficient() const;
    /*! Sets value, whether bullet is piercing
        \param[in] value a value
     */
    void setIsPiercing(bool value);
    /*! Returns whether bullet is piercing
        \return whether is piercing
     */
    bool isPiercing() const;
    /*! Called, when bullet hits platform
     * \param[in] body a body
     */
    void onPlatformHit(sad::p2d::Body* body) override;
    /*! Called, when bullet hits wall
     * \param body
     */
    void onWallHit(sad::p2d::Body* body) override;
    /*! Called when bullet hits player
     * \param[in] player a player
     */
    void onPlayerHit(game::Player* player) override;
    /*! Called, when bullet hits enemy (not player)
        \param[in] actor enemy's actor
     */
    void onEnemyHit(game::Actor* actor) override;
protected:
    /*! An inner sprite
     */
    sad::Sprite2D* m_sprite;
    /*! A body for bullet
     */
    sad::p2d::Body* m_body;
    /*! Whether bullet should go through platforms
     */
    bool m_is_ghost;
    /*! Bounce count left for bullet, before decay
     */
    int m_bounce_count_left;
    /*! A bounce restitution coefficient
     */
    double m_bounce_restitution_coefficient;
    /*! Whether bullet is piercing
     */
    bool m_is_piercing;
    /*! Whether bullet should decay
     */
    bool m_should_decay;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(weapons::Bullet)
