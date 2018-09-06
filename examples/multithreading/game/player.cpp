#include "player.h"
#include "../game.h"
#include <cstdio>

const int game::Player::MaxHorizontalVelocity = 200;
const int game::Player::MaxVerticalVelocity = 200;

game::Player::Player() : m_sprite(NULL), m_body(NULL), m_is_resting(false), m_resting_platform(NULL), m_own_horizontal_velocity(0)
{

}


void game::Player::reset()
{
    m_inventory.reset();
    m_sprite = NULL;
    m_body = NULL;
    m_is_resting = false;
    m_resting_platform = NULL;
    m_own_horizontal_velocity = 0;
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
    printf("Old horizontal velocity: %lf, Old own velocity %lf, Setting %lf\n", v.x(), m_own_horizontal_velocity, value);
    if (m_is_resting)
    {
        v.setX(m_resting_platform->tangentialVelocity().x());
    } 
    else
    {
        v.setX(0.0);
    }
    v.setX(v.x() + value);
    printf("New horizontal velocity: %lf\n", v.x());
    b->sheduleTangentialVelocity(v);
    
    m_own_horizontal_velocity = value;
}

void game::Player::enableGravity()
{
    Game::enableGravity(m_body);
}

void game::Player::disableGravity()
{
    Game::disableGravity(m_body);
}

void game::Player::restOnPlatform(sad::p2d::Body* b, const  sad::p2d::Vector& old_velocity)
{
    if (!b)
    {
        return;
    }
    this->disableGravity();
    m_is_resting = true;
    m_resting_platform = b;
    
    double av = 0;
    b->setCurrentTangentialVelocity(old_velocity);
    b->setCurrentAngularVelocity(av);
    b->sheduleTangentialVelocity(old_velocity);
    b->sheduleAngularVelocity(av);

    sad::p2d::Vector own_velocity = old_velocity;
    own_velocity.setX(own_velocity.x() + m_own_horizontal_velocity);

    m_body->setCurrentTangentialVelocity(own_velocity);
    m_body->setCurrentAngularVelocity(av);
    m_body->sheduleTangentialVelocity(own_velocity);
    m_body->sheduleAngularVelocity(av);
}

void game::Player::disableResting()
{
    this->enableGravity();
    m_is_resting = false;
    m_resting_platform = NULL;
}


sad::Rect2D game::Player::area() const
{
    return m_sprite->area();
}

void game::Player::move(const sad::Point2D& p)
{
    m_body->move(p);
}