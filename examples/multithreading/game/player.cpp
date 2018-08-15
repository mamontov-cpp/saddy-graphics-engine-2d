#include "player.h"


game::Player::Player()
{

}


void game::Player::reset()
{
    m_inventory.clear();
    m_inventory.setNode(NULL);
}


game::Inventory* game::Player::inventory()
{
    return &m_inventory;
}
