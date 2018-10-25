/*! \file projectile.h
 
    A projectile, that should be stored in game
 */
#pragma once
#include <object.h>

#include "../game/actor.h"

#include <p2d/body.h>

class Game;

namespace game
{

class Actor;
class Player;

}

namespace weapons
{
/*! A projectile that will be stored in game
 */
class Projectile: public sad::Object
{
    SAD_OBJECT
    /*! Constructs new projectile
     */
    Projectile();
    /*! Must be inherited
     */
    virtual ~Projectile();
    /*! Returns actor
        \return actor
     */
    virtual game::Actor* actor() const = 0;
    /*! Kills an object
     */
    virtual void kill()  = 0;
    /*! Updates side for a swing
     */
    virtual void update() = 0;
    /*! A damage from projectile
        \param[in] damage a damage
     */
    void setDamage(int damage);
    /*! Returns simple damage from projectile
        \return projectile damage
     */
    int damage() const;
    /*! Called, when bullet hits enemy (not player)
        \param[in] enemy enemy's actor
     */
    virtual void onEnemyHit(game::Actor* enemy);
    /*! Called, when bullet hits platform
     * \param[in] body a body
     */
    virtual void onPlatformHit(sad::p2d::Body* body);
    /*! Called when bullet hits player
     * \param[in] player
     */
    virtual void onPlayerHit(game::Player* player);
    /*! Called, when bullet hits wall
     * \param body
     */
    virtual void onWallHit(sad::p2d::Body* body);
protected:
    /*! A damage from projectile
     */
    int m_damage;
    /*! An inline game value
     */
    Game* m_game;
};

}


DECLARE_TYPE_AS_SAD_OBJECT_ENUM(weapons::Projectile);
