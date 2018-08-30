#include "player.h"


game::Player::Player() : m_sprite(NULL), m_body(NULL)
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


void game::Player::setSprite(sad::Sprite2D* sprite)
{
    m_sprite = sprite;
}

void game::Player::setBody(sad::p2d::Body* body)
{
    m_body = body;
}