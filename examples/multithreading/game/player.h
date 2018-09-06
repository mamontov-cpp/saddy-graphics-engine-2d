/*!  \file player.h

     Describes a player, that will exist in game
 */
#pragma once
#include "inventory.h"
#include <sprite2d.h>
#include <p2d/body.h>

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
    /*! Resets player's items in game
     */
    void reset();
    /*! Returns player's inventory
     *  \return inventory of player
     */
    game::Inventory* inventory();
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
    void enableGravity();
    /*! Disables gravity for body
     */
    void disableGravity();
    /*! Rests player on platform
        \param[in] b body
        \param[in] old_velocity old platform's velocity
     */
    void restOnPlatform(sad::p2d::Body* b, const  sad::p2d::Vector& old_velocity);
    /*! Disables resting player on platform
     */
    void disableResting();
    /*! Fetches area for user
     */
    sad::Rect2D area() const;
    /*! Moves player by point
     */
    void move(const sad::Point2D& p);
private:
    /*! A player's own horizontal velocity
     */
    double m_own_horizontal_velocity;
    /*! Player's inventory, that will be carried around
     */
    game::Inventory m_inventory;
    /*! A player's sprite
     */
    sad::Sprite2D* m_sprite;
    /*! A player's body
     */
    sad::p2d::Body* m_body;
    /*! Whether player is resting on platform
     */
    bool m_is_resting;
    /*! A platform, where player is resting
     */
    sad::p2d::Body* m_resting_platform;
};

}
