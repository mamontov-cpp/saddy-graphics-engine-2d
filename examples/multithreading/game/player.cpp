#include "player.h"

const int game::Player::MaxHorizontalVelocity = 10;
const int game::Player::MaxVerticalVelocity = 10;

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

void game::Player::setHorizontalVelocity(double value)
{
    sad::p2d::Body* b = m_body;
    sad::p2d::Vector v = b->tangentialVelocity(); 
    v.setX(value);
    b->sheduleTangentialVelocity(v);
}