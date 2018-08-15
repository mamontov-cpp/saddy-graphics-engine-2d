/*!  \file player.h

     Describes a player, that will exist in game
 */
#pragma once
#include "inventory.h"

namespace game
{

/*!A player
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
        \return inventory of player
     */
    game::Inventory* inventory();
private:
    /*! Player's inventory, that will be carried around
     */
    game::Inventory m_inventory;
};

}
