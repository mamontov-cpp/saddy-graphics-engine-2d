/*! \file actor.h
    
    Defines an actor, which can respond to user actions
 */
#pragma once
#include <p2d/body.h>
#include <p2d/world.h>
#include <sprite2d.h>
#include <animations/animationsinstance.h>
#include <animations/animationsoptionlist.h>
#include <bitset>
#include <functional>
#include <object.h>
#include <scene.h>

#include "actoroptions.h" 
#include "../bots/shootingstrategies/shootingstrategy.h"

class Game;

namespace weapons
{

class Weapon;

}


namespace game
{
// A last key bitset offset
#define LAST_KEY_BITSET_OFFSET (4)
// A precision for resting detection
#define RESTING_DETECTION_PRECISION (0.5)
/*! An actor, that can respond to user actions
 */
class Actor: public sad::Object  // NOLINT(cppcoreguidelines-special-member-functions)
{
SAD_OBJECT
public:
    /*! A button for getting status bit
     */
    enum class Button: int
    {
        ABTN_LEFT = 0,   //!< Left button status
        ABTN_RIGHT = 1,  //!< Right button status
        ABTN_UP = 2,     //!< Up button status
        ABTN_DOWN = 3    //!< Down button status
    };
    /*! Makes new actor options
     */
    Actor();
    /*! Destroys an actor
     */
    ~Actor();
    /*! Sets options for actor, returns false if options are already set
        \param[in] opts options
        \throws logic_error if options are invalid
        \return true if setting is successful, otherwise false
     */
    bool setOptions(game::ActorOptions* opts);
    /*! Tries to start actor going up
     */
    void tryStartGoingUp();
    /*! Tries to start actor from going up
     */
    void tryStopGoingUp();
    /*! Tries to start actor going down
     */
    void tryStartGoingDown();
    /*! Tries to stop actor from going down
     */
    void tryStopGoingDown();
    /*! Tries to start actor going left
     */
    void tryStartGoingLeft();
    /*! Tries to stop actor from going left
     */
    void tryStopGoingLeft();
    /*! Tries to start actor going right
     */
    void tryStartGoingRight();
    /*! Tries to stop actor from going right
     */
    void tryStopGoingRight();

    /*! Called on platform collision
        \param[in] ev collision event
     */
    void onPlatformCollision(const sad::p2d::BasicCollisionEvent & ev);

    /*! Sets game for player
        \param[in] game a game
     */
    void setGame(Game* game);
    /*! Returns game for actor
        \return game
     */
    Game* game() const;
    /*! Resets player's state in game
     */
    void reset() override;
    /*! Sets velocity for actor
     *  \param[in] v velocity
     */
    void setVelocity(const sad::p2d::Vector& v) const;
    /*! Initializes player's sprite according to state
        \param[in] no_sound whether we should disable sound on actor 
     */
    void init(bool no_sound = false);
    /*! Returns whether actor is dying
        \return whether actor is dying
     */
    bool isDying() const;
    /*! Sets dying flag for actor
     *  \param[in] value a value for dying
     */
    void setDying(bool value);

    /*! Returns true if actor is in a floater state
        \return whether actor is floater
     */
    bool isFloater() const;
    /*! Sets a floater state for an actor
        \param[in] is_floater a floater state
     */
    void setFloaterState(bool is_floater);

    /*! Whether player is resting on platform
        \return whether he is resting
     */
    bool isResting() const;
    /*! Whether player can jump
        \return whether player can jump
     */
    bool canJump() const;
    /*! Sets sprite for player
     *  \param[in] sprite a sprite
     */
    void setSprite(sad::Sprite2D* sprite);
    /*! Sets body for player
     *  \param[in] body a body for player
     */
    void setBody(sad::p2d::Body* body);
    /*! Sets horizontal velocity for player
     *  \param[in] value velocity value
     */
    void setHorizontalVelocity(double value);
    /*! Increments vertical velocity of player by value
     */
    void incrementVerticalVelocity(double value);
    /*! Enables gravity for body
     */
    void enableGravity() const;
    /*! Disables gravity for body
     */
    void disableGravity() const;
    /*! Rests player on platform
        \param[in] b body
        \param[in] old_velocity old platform's velocity
     */
    void restOnPlatform(sad::p2d::Body* b, const  sad::p2d::Vector& old_velocity);
    /*! Disables resting player on platform
     */
    void disableResting();
    /*! Fetches area for user
     *  \return area for actor
     */
    sad::Rect2D area() const;
    /*! Returns middle of actor
     *  \return middle of actor
     */
    sad::Point2D middle() const;
    /*! Moves actor by specified vector
     *  \param[in] p point
     */
    void move(const sad::Point2D& p) const;
    /*! Returns body for player
     *  \return body
     */
    sad::p2d::Body* body() const;
    /*! Returns sprite for player
        \return sprite for player
     */
    sad::Sprite2D* sprite() const;
    /*! Tests resting a player
     */
    void testResting();
    /*! Starts moving actor to left
     */
    void startMovingLeft();
    /*! Start moving actor to right
     */
    void startMovingRight();
    /*! Stops actor from moving horizontally
     */
    void stopMovingHorizontally();
    /*! Makes actor try to jump
     */
    void tryJump();
    /*! Makes actor start falling or duck
     */
    void startFallingOrDuck();
    /*! Stops falling or ducking for actor
     */
    void stopFallingOrStopDucking();
    /*! Makes actor stop ducking
     */
    void duck();
    /*!  Makes actor stand, not duck
     */
    void stopDucking();
    /*! Returns whether actor is ducking
        \return whether actor is ducking
     */
    bool isDucking() const;
    /*! Returns whether actor is free falling
        \return whether actor is free falling
     */
    bool isFreefalling() const;
    /*! Returns old velocity for player
        \return old velocity
     */
    const sad::p2d::Vector& oldVelocity() const;
    /*! A notify function for vector
     *  \param[in] v a vector object
     */
    void moveBy(const sad::p2d::Vector& v) const;
    /*! Returns true, called by body function
     *  \return true
     */
    static bool canBeRotated();
    /*! Rotates an actor by a specified angle
     * \param[in] angle a used angle
     */
    void rotate(double angle) const;
    /*! Returns resting platform
        \return resting platform
     */
    sad::p2d::Body* restingPlatform() const;
    /*! Whether actor last moved left
        \return whether last time actor moved left
     */
    bool isLastMovedLeft() const;
    /*! Sets, whether actor should be invincibility
        \param[in] on whether actor is invincible
     */
    void toggleInvincibility(bool on);
    /*! Returns true if actor is invincible
        \return true if actor is invincible
     */
    bool isInvincible() const;
    /*! Return amount of lives for actor
        \return amount of lives
     */
    int lives() const;
    /*! Sets amount of lives for actor
        \param[in] lives amount of lives for actor
     */
    void setLives(int lives);
    /*! Increments amount of lives for actor by specified amount
        \param[in] lives amount of lives
     */
    void incrementLives(int lives);
    /*! Decrement amount of lives and kills an actor if should to
         \param[in] lives amount of lives
     */
    void decrementLives(int lives);
    /*! If actor is not invincible, decrements lives, triggering animations, otherwise does nothing
        \param[in] lives amount of lives
     */
    void tryDecrementLives(int lives);
    /*! Sets action for actor
        \param[in] action for actor, that will be called, when actor is dead
     */
    void onDeath(const std::function<void(game::Actor*)>& action);
    /*! Ensures, that action called before all other actions, when actor is dead
        \param[in] action for actor, that will be called, when actor is dead
     */
    void onBeforeDeath(const std::function<void(game::Actor*)>& action);
    /*!  Sets animation, which is called, when actor is hurt
         \param[in] animation used animation 
     */
    void setHurtAnimation(sad::animations::Animation* animation);
    /*! Modifies damage from weapon, allowing to increase it or decrease
     * \param[in] base_dmg base damage
     * \return value
     */
    int modifyDamage(int base_dmg);
    /*! Returns lookup angle for actor
     */
    double lookupAngle() const;
    /*! Sets lookup angle
     *  \param[in] angle an angle
     */
    void setLookupAngle(double angle);
    /*! Sets weaponry for actor
     *  \param[in] w weapon
     */
    void pushWeapon(weapons::Weapon* w);
    /*! Removes weaponry from actor
     *  \param[in] w weapon
     */
    void removeWeapon(weapons::Weapon* w);
    /*! Activates weapon for actor
     *  \param[in] w weapon
     */
    void activateWeapon(weapons::Weapon* w);
    /*! Removes weapon from actor, together with weapon item
     *  \param[in] w weapon
     *  \param[in] g game
     */
    void removeWeaponWithItem(weapons::Weapon* w, Game* g);
    /*! Returns weaponry for actor
     *  \return weapon
     */
    weapons::Weapon* weapon() const;
    /*! Tries to shoot for actor
     */
    void tryShoot();
    /*! Returns point for spawning a projectile
     *  \param[in] angle an angle
     *  \return point
     */
    sad::Point2D pointForProjectileSpawn(double angle) const;
    /*! Returns scene for actor
     *  \return scene
     */
    sad::Scene* scene() const;
    /*! Plays death animation
     *  \return instance
     */
    sad::animations::Instance* playDeathAnimation();
    /*! Makes actor die
     */
    void die();
    /*! Sets shooting strategy for bot
     *  \param[in] s a strategy
     */
    void setShootingStrategy(bots::shootingstrategies::ShootingStrategy* s);
    /*! Toggles fixed size for actor
        \param[in] fixed_size a fixed size for actor
     */
    void toggleFixedSize(bool fixed_size);
    /*! Returns whether sprite have fixed size
     *  \return whether sprite have fixed size
     */
    bool fixedSize() const;
    /*! Increments attack modifier
     *  \param[in] attack_delta amount of points, modifier will be incremented
     */
    void incrementAttackModifier(int attack_delta);
    /*! Decrements attack modifier
     *  \param[in] attack_delta amount of points, modifier will be incremented 
     */
    void decrementAttackModifier(int attack_delta);
    /*! Increments defense amount for actor
     *  \param[in] delta amount of defense
     */
    void incrementDefense(int delta);
    /*! Decrement defense amount for actor
     *  \param[in] delta amount of defense
     */
    void decrementDefense(int delta);
    /*! Returns defense amount
     *  \return defense
     */
    int defense() const;
    /*! Makes actor take damage
     *  \param[in] base_dmg a base damage for projectile
     */
    void takeDamage(int base_dmg);
    /*! Increments floater state counter. If it's bigger that 0, makes actor float
     */
    void incrementFloaterStateCounter();
    /*! Decrements floater state counter. If it's bigger that 0, makes actor not float
     */
    void decrementFloaterStateCounter();
    /*! Toggles, whether actor is affected by wind
     *  \param[in] affected true if affected
     */
    void toggleIsAffectedByWind(bool affected);
    /*! Returns true if actor  is affected by wind
     *  \return true if actor is affected by wind
     */
    bool isAffectedByWind() const;
    /*! Updates horizontal speed for actor
     */
    void updateHorizontalVelocity();
    /*! Fires on death events for actor
     */
    void fireOnDeathEvents();
    /*! Pauses weapon reloading for actor
     */
    void pauseWeaponsReloading();
    /*! Resumes weapon reloading for actor
     */
    void resumeWeaponsReloading();
    /*! Returns whether actor is clipping through boundaries
     *  \return whether actor is clipping through boundaries
     */
    bool isClipThroughBoundaries() const;
    /*! Whether actor, should clip through boundaries
     *  \param[in] is_clipping values
     */
    void setClipThroughBoundaries(bool is_clipping);
private:
    /*! Compute whether floater should go up or down
        \param[out] is_going_up whether we should go up
        \param[out] is_going_down whether we should go down
     */
    void computeIsGoingUpDownFlags(bool& is_going_up, bool& is_going_down);
    /*! Compute whether floater should go left or right
        \param[out] is_going_left whether we should go left
        \param[out] is_going_right whether we should go right
     */
    void computeIsGoingLeftRightFlags(bool& is_going_left, bool& is_going_right);
    /*! Sets correct angle for floater
     */
    void setAngleForFloater();
    /*! Computes velocity for floater
        \throws std::logic_error on invalid options or not floater
        \return velocity for floater
     */
    sad::p2d::Vector computeVelocityForFloater();
    /*! Sets vertical velocity value
        \param[in] v velocity
     */
    void setVerticalVelocity(double v) const;
    /*! Returns animations list
        \return animations
     */
    sad::animations::Animations* animations() const;
    /*! Plays walking animation
     */
    void playWalkingAnimation();
    /*! Cancels walking animation
     */
    void cancelWalkingAnimation();
    /*! Plays jumping animation
     */
    void playJumpingAnimation();
    /*! Cancels jumping animation
     */
    void cancelJumpingAnimation();
    /*! Starts moving player in specified direction (positive - right, negative - left)
        \param[in] flip_flag flip flag value
        \param[in] velocity a velocity value
     */
    void startMoving(bool flip_flag, double velocity);
    /*! Synchronizes actor's shape with it's area.
        Used for ducking
     */
    void correctShape() const;
    /*! Sets options for sprite
        \param[in] o options
     */
    void setOptionsForSprite(const sad::String& o) const;
    /*! A player's own horizontal velocity
     */
    double m_own_horizontal_velocity;
    /*! A main key states
        A (4 + key number) holds whether this type of button was pressed last
     */
    std::bitset<8> m_key_states;
    /*! A player's sprite
     */
    sad::Sprite2D* m_sprite;
    /*! An old velocity, until player canceled moving or other stuff
     */ 
    sad::p2d::Vector m_old_velocity;
    /*! A player's body
     */
    sad::p2d::Body* m_body;
    /*! Whether player is resting on platform
     */
    bool m_is_resting;
    /*! Whether player is ducking
     */
    bool m_is_ducking;
    /*! Whether we are free falling, using down button
     */
    bool m_is_free_fall;
    /*! True, if walking animation is playing
     */
    bool m_is_walking_animation_playing;
    /*! True, if jumping animation is playing
     */
    bool m_is_jumping_animation_playing;
    /*! A platform, where player is resting
     */
    sad::p2d::Body* m_resting_platform;
    /*! Whether actor is floating object
     */
    bool m_is_floater;
    /*! Whether actor is dying
     */
    bool m_is_dying;

    /*! A walking animation for actor
     */
    sad::animations::OptionList* m_walking_animation;
    /*! A walking animation instance
     */
    sad::animations::Instance* m_walking_instance;
    
    /*! A jumping animation for player
     */
    sad::animations::OptionList* m_jumping_animation;
    /*! A jumping animation instance
     */
    sad::animations::Instance* m_jumping_instance;
    /*! A game for player
     */
    Game* m_game;
    /*! An in-game options for actor
     */
    game::ActorOptions* m_options;
    /*! Whether last time we moved left
     */
    bool m_is_last_moved_left;
    /*! An invincibility flag for making an actor invincible
     */
    bool m_is_invincible;
    /*! Amount of lives for actor
     */
    int m_lives;
    /*! An action, that should be called, when actor is dying
     */
    std::vector<std::function<void(game::Actor*)> > m_on_death_actions;
    /*! An animation, which is called, when actor is hurt
     */
    sad::animations::Animation* m_hurt_animation;
    /*! A lookup angle for actor
     */
    double m_lookup_angle;
    /*! A weapon for actor;
     */
    sad::Vector<weapons::Weapon*> m_weapons;
    /*! Whether actor has fixed size
     */
    bool m_fixed_size;
    /*! Attack modifiers stack up and add damage to attack of weapon
     */
    int m_attack_modifier;
    /*! A defence modifier increments defence for actor
     */
    int m_defense;
    /*! A counter for floater state
     */
    int m_floater_state_counter;
    /*! Toggles whether actor is affected by wind
     */
    bool m_affected_by_wind;
    /*! Whether actor will clip through boundaries
     */
    bool m_clip_through_boundaries;
};

/*! Exposes actor to context
    \param[in] c context
 */
void exposeActor(void* c);

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(game::Actor)

