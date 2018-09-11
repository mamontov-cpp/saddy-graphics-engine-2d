/*!  \file player.h

     Describes a player, that will exist in game
 */
#pragma once
#include "inventory.h"
#include <sprite2d.h>
#include <p2d/body.h>
#include <actor.h>
#include <animations/animationsinstance.h>
#include <animations/animationsoptionlist.h>

class Game;

namespace game
{

/*! A player
 */
class Player
{
public:
    /*! Maximal horizontal velocity for player
     */
    static const int MaxHorizontalVelocity;
    /*! Maximal vertical velocity for player
     */
    static const int MaxVerticalVelocity;
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
        \param[in] gama a game
     */
    void setGame(Game* game);
    /*! Resets player's items in game
     */
    void reset();
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
    /*! Clears flags, which determine, whether player's position is fixed
     */
    void clearFixedFlags();
    /*! Tests resting a player
     */
    void testResting();
private:
    /*! Player's inventory, that will be carried around
     */
    game::Inventory m_inventory;
    /*! An actor for player
     */
    game::Actor m_actor;
};

}
