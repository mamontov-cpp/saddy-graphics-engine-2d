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
    /*! Sets sprite for player
     *  \param[in] sprite a sprite
     */
    void setSprite(sad::Sprite2D* sprite);
    /*! Sets body for player
     *  \param[in] body a body for player
     */
    void setBody(sad::p2d::Body* body);
private:
    /*! Player's inventory, that will be carried around
     */
    game::Inventory m_inventory;
    /*! A player's sprite
     */
    sad::Sprite2D* m_sprite;
    /*! A player's body
     */
    sad::p2d::Body* m_body;
};

}
