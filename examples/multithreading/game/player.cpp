#include "player.h"
#include "../game.h"
#include <cstdio>

const int game::Player::MaxHorizontalVelocity = 200;
const int game::Player::MaxVerticalVelocity = 400;

game::Player::Player() : m_own_horizontal_velocity(0), m_sprite(NULL), m_body(NULL), m_is_resting(false), m_resting_platform(NULL), m_fixed_x(false), m_fixed_y(false)
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
    m_fixed_x = false;
    m_fixed_y = false;
}


game::Inventory* game::Player::inventory()
{
    return &m_inventory;
}

bool game::Player::canJump() const
{
    return isResting();
}

bool game::Player::isResting() const
{
    return m_is_resting;
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
    b->setCurrentTangentialVelocity(v);
    b->sheduleTangentialVelocity(v);
    
    m_own_horizontal_velocity = value;
}

void game::Player::incrementVerticalVelocity(double value) const
{
    if (m_body->willTangentialVelocityChange())
    {
        m_body->sheduleTangentialVelocity(m_body->nextTangentialVelocity() + sad::p2d::Vector(0, value));
    }
    else
    {
         m_body->setCurrentTangentialVelocity(m_body->tangentialVelocity() + sad::p2d::Vector(0, value));
    }
}

void game::Player::enableGravity() const
{
    Game::enableGravity(m_body);
}

void game::Player::disableGravity() const
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
    // If we collided second time, it means we're stuck, so force going out of platform
    if (m_resting_platform == b)
    {
        printf("Fixing position\n");
        m_body->shedulePosition(m_body->nextPosition() + sad::p2d::Vector(0, 0.1));
    }
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

void game::Player::move(const sad::Point2D& p) const
{
    if (m_body->willPositionChange())
    {
        m_body->shedulePosition(m_body->nextPosition() + p);
    }
    else
    {
        m_body->move(p);
    }
}


void game::Player::clearFixedFlags()
{
    m_fixed_x = false;
    m_fixed_y = false;
}

bool game::Player::isXCoordinateFixed() const
{
    return m_fixed_x;
}

bool game::Player::isYCoordinateFixed() const
{
    return m_fixed_y;
}


void game::Player::setXCoordinateFixed(bool value)
{
    m_fixed_x = value;
}

void game::Player::setYCoordinateFixed(bool value)
{
    m_fixed_y = value;
}

sad::p2d::Body* game::Player::body() const
{
    return m_body;
}

void game::Player::testResting()
{
    if (m_is_resting)
    {
        sad::Rect2D shape_1 = dynamic_cast<sad::p2d::Rectangle*>(m_body->currentShape())->rect();
        sad::Rect2D shape_2 = dynamic_cast<sad::p2d::Rectangle*>(m_resting_platform->currentShape())->rect();

        sad::p2d::Cutter1D player_part(std::min(shape_1[0].x(), shape_1[2].x()), std::max(shape_1[0].x(), shape_1[2].x()));
        sad::p2d::Cutter1D platform_part(std::min(shape_2[0].x(), shape_2[2].x()), std::max(shape_2[0].x(), shape_2[2].x()));
        if (!sad::p2d::collides(player_part, platform_part))
        {
            this->disableResting();
        }
    }
}
