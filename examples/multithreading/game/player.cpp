#include "player.h"


game::Player::Player()
{

}


void game::Player::reset()
{
    m_inventory.reset();
}


game::Inventory* game::Player::inventory()
{
    return &m_inventory;
}
