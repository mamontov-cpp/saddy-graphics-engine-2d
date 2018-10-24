/*! \file weapon.h

    A weapon
 */
#pragma once
#include <timer.h>

#include <object.h>


#include "bulletsettings.h"
#include "swingsettings.h"
#include "lasersettings.h"


class Game;

namespace game
{

class Actor;

}

namespace weapons
{
/*! A weapon, that can be held by actor
 */
class Weapon : public sad::Object
{
SAD_OBJECT
public:
/*! A type of weapon used
 */
enum Type
{
    WWT_NONE = 0,   //!< None
    WWT_SWING = 1,  //!< Swing weapon
    WWT_BULLET = 2, //!< A bullet weapon
    WWT_LASER = 3   //!< A laser weapon
};
    /*! Makes new default weapon
     */
    Weapon();
    /*! Destroys weapon
     */
    ~Weapon();
    /*! Sets shooting interval for weapon
     *  \param[in] interval an interval
     */
    void setShootingInterval(double interval);
    /*! Returns shooting interval for weapon
     * \return shooting interval
     */
    double shootingInterval() const;
    /*! Sets amount of projectiles, emitted by weapon at tile
     *  \param[in] projectiles amount of projectiles
     */
    void setAmountOfProjectiles(size_t projectiles);
    /*! Returns amount of projectiles
     * \return amount of projectiles
     */
    size_t amountOfProjectiles() const;
    /*! Sets delay for emitting next bullet, when shooting with spread
     *  \param[in] value a value
     */
    void setDelay(double value);
    /*! Returns delay for emitting next bullet
     * \return a delay
     */
    double delay() const;
    /*! Sets base damage for weapon
     * \param[in] damage a base damage
     */
    void setBaseDamage(int damage);
    /*! Returns base damage for weapon
     *  \return damage
     */
    int baseDamage() const;
    /*! Sets minimal angle delta spread for shooting
     *  \param[in] value a value
     */
    void setMinAngleDelta(double value);
    /*! Returns minimal angle delta spread for shooting
     *  \return minimal angle
     */
    double minAngleDelta() const;
    /*! Sets maximal angle delta spread for shooting
     *  \param[in] value a value
     */
    void setMaxAngleDelta(double value);
    /*! Returns maximal angle delta spread for shooting
     *  \return minimal angle
     */
    double maxAngleDelta() const;
    /*! Sets settings for swinging
     * \param[in] s settings
     */
    void setSettings(const weapons::SwingSettings& s);
    /*! Sets settings for swinging
     * \param[in] s settings
     */
    void setSettings(const weapons::BulletSettings& s);
    /*! Sets settings for swinging
     * \param[in] s settings
     */
    void setSettings(const weapons::LaserSettings& s);
    /*! Tries to shoot from weapon
     *  \param[in] game a game
     *  \param[in] actor an actor, who will shoot
     */
    void tryShoot(Game* game, game::Actor* actor);
    /*! Pauses weapon
     */
    void pause();
    /*! Resumes weapon
     */
    void resume();
protected:
    /*! Spawns projectiles for weapon
     *  \param[in] game a game
     *  \param[in] actor an actor
     *  \param[in] angle an angle
     *  \param[in] delay a delay
     */
    void spawnProjectile(Game* game, game::Actor* actor, double angle, double delay);
    /*! Clears settings for weapon
     */
    void clearSettings();
    /*! Interval between two sequential shots for weapon
     */
    double m_shooting_interval;
    /*! Amount of projectiles, fired by one shot
     */
    size_t m_amount_of_projectiles;
    /*! A delay between projectiles
     */
    double m_delay;
    /*! A base damage for the projectiles
     */
    int m_base_damage;
    /*! A minimal angle for spreads
     */
    double m_min_dangle;
    /*! A maximal angle for spreads
     */
    double m_max_dangle;
struct
{
    /*! A type of weapon
     */
    weapons::Weapon::Type Type;
    union
    {
        weapons::BulletSettings* Bullet; //!< Bullet settings
        weapons::SwingSettings*  Swing;  //!< Swing settings
        weapons::LaserSettings*  Laser;  //!< Laser settings
    } Settings;
}   m_settings; //!< A settings for weapon

    /*! A timer for weapon
     */
    sad::Timer m_timer;
    /*! A paused timer for weapon
     */
    sad::Timer m_paused_timer;
    /*! Elapsed time
     */
    double m_elapsed_time;
    /*! Is paused flag
     */
    bool m_is_paused;
};


}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(weapons::Weapon)
