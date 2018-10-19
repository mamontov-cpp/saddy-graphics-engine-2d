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
        \patam[in] ev collision event
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
    /*! Inits player's sprite according to state
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
    sad::Rect2D area();
    /*! Returns middle point for player
     *  \return middle point
     */
    sad::Point2D middle();
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
private:
    /*! Player's inventory, that will be carried around
     */
    game::Inventory m_inventory;
    /*! An actor for player
     */
    game::Actor m_actor;
};

}
