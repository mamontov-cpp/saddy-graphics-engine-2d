/*!  \file player.h

     Describes a player, that will exist in game
 */
#pragma once
#include "inventory.h"
#include "actor.h"

class Game;

namespace game
{

/*! A player
 */
class Player
{
public:
    /*! A player, that will be used in game
     */
    Player();
    /*! Frees data from player
     */
    ~Player();
    /*! Tries to set actor options for player
        \param[in] opts an options
        \throws logic_error if options are invalid
        \return true if setting is succesfull, otherwise false
     */
    bool setActorOptions(game::ActorOptions* opts);
    /*! Called on platform collision
        \param[in] ev collision event
     */
    void onPlatformCollision(const sad::p2d::BasicCollisionEvent & ev);
    /*! Sets game for player
        \param[in] game a game
     */
    void setGame(Game* game);
    /*! Returns game for player
     *   \return game
     */
    Game* game() const;
    /*! Resets player's items in game
     */
    void reset();
    /*! Initializes player's sprite according to state
        \param[in] no_sound whether we should disable sound on actor 
     */
    void init(bool no_sound = false);
    /*! Returns true if actor is in a floater state
        \return whether actor is floater
     */
    bool isFloater() const;
    /*! Sets a floater state for an actor
        \param[in] is_floater a floater state
     */
    void setFloaterState(bool is_floater);
    /*! Returns player's inventory
     *  \return inventory of player
     */
    game::Inventory* inventory();

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

    /*! Sets sprite for player
     *  \param[in] sprite a sprite
     */
    void setSprite(sad::Sprite2D* sprite);
    /*! Sets body for player
     *  \param[in] body a body for player
     */
    void setBody(sad::p2d::Body* body);
    /*! Returns a bodies area for player
     *  \return area for player
     */
    sad::Rect2D area() const;
    /*! Returns middle point for player
     *  \return middle point
     */
    sad::Point2D middle() const;
    /*! Tests resting a player
     */
    void testResting();
    /*! Enables gravity for player
     */
    void enableGravity() const;
    /*! Returns an actor
     */
    game::Actor* actor();
    /*! Whether actor last moved left
        \return whether last time actor moved left
     */
    bool isLastMovedLeft() const;

    /*! Sets, whether player should be invincibility
        \param[in] on whether player is invincible
     */
    void toggleInvincibility(bool on);
    /*! Returns true if player is invincible
        \return true if player is invincible
     */
    bool isInvincible() const;
    /*! Return amount of lives for player
        \return amount of lives
     */
    int lives() const;
    /*! Sets amount of lives for player
        \param[in] lives amount of lives for actor
     */
    void setLives(int lives);
    /*! Increments amount of lives for player by specified amount
        \param[in] lives amount of lives
     */
    void incrementLives(int lives);
    /*! Decrement amount of lives and kills an player if should to
         \param[in] lives amount of lives
     */
    void decrementLives(int lives);
    /*! Sets action for player
        \param[in] action for player, that will be called, when player is dead
     */
    void onDeath(const std::function<void(game::Actor*)>& action);
    /*!  Sets animation, which is called, when player is hurt
         \param[in] animation
     */
    void setHurtAnimation(sad::animations::Animation* animation);
    /*! If player is not invincible, decrements lives, triggering animations, otherwise does nothing
        \param[in] lives amount of lives
     */
    void tryDecrementLives(int lives);
    /*! Toggles whether player is dead
        \param[in] is_dead new value
     */
    void toggleIsDead(bool is_dead);
    /*! Returns true, if player is dead
        \return whether player is dead
     */
    bool isDead() const;
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
    /*! Returns weaponry for actor
     *  \return weapon
     */
    weapons::Weapon* weapon() const;
    /*! Tries to shoot for actor
     */
    void tryShoot();
    /*! Initializes physics for player
     *  \param[in] world a world, where player belongs
     *  \param[in] db database
     */
    void initPhysics(sad::p2d::World* world, sad::db::Database* db);
    /*! Kills a player, making him die
     */
    void die();
    /*! Called on bottom wall collision
     */
    void onBottomWallCollision();
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
    /*! Updates horizontal speed for actor
     */
    void updateHorizontalVelocity();
    /*! Pauses weapon reloading for actor
     */
    void pauseWeaponsReloading();
    /*! Resumes weapon reloading for actor
     */
    void resumeWeaponsReloading();
private:
    /*! Player's inventory, that will be carried around
     */
    game::Inventory m_inventory;
    /*! An actor for player
     */
    game::Actor m_actor;
    /*! Whether player is dead
     */
    bool m_is_dead;
};


/*! Exposes player to context
    \param[in] c context
    \param[in] game a game
 */
void exposePlayer(void* c, Game* game);

}
