#include "player.h"
#include "../game.h"


#include <cstdio>

#include <p2d/world.h>

const int game::Player::MaxHorizontalVelocity = 200;
const int game::Player::MaxVerticalVelocity = 400;

game::Player::Player() 
: m_own_horizontal_velocity(0), 
m_sprite(NULL), 
m_body(NULL), 
m_is_resting(false),
m_is_ducking(false),
m_is_free_fall(false),
m_resting_platform(NULL),
m_fixed_x(false),
m_fixed_y(false)
{
    m_walking_animation ;

    //m_walking_animation->addRef();
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
    m_old_velocity = v;
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
        sad::p2d::Vector v = m_body->nextTangentialVelocity();
        if (!(this->isXCoordinateFixed()))
        {
            v.setX(m_own_horizontal_velocity);
        }        
        m_body->sheduleTangentialVelocity(v + sad::p2d::Vector(0, value));
    }
    else
    {
         sad::p2d::Vector v = m_body->tangentialVelocity();
         if (m_is_resting)
         {
             v.setX(m_own_horizontal_velocity);
             v.setY(0.0);
         }
         m_body->sheduleTangentialVelocity(m_body->tangentialVelocity() + sad::p2d::Vector(0, value));
    }
    printf("Next tangential velocity after increment: %lf, %lf\n", m_body->nextTangentialVelocity().x(), m_body->nextTangentialVelocity().y());
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
    m_is_resting = true;
    m_is_free_fall = false;
    m_is_ducking = false;
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

    m_old_options.clear();
    if (!sad::is_fuzzy_zero(m_own_horizontal_velocity))
    {
         m_sprite->set("enemies_list/playerRed_walk1ng");
    } 
    else 
    {
        m_sprite->set("enemies_list/playerRed_standng");
    }
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

sad::Sprite2D* game::Player::sprite() const
{
    return m_sprite;
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
        else
        {
            sad::p2d::Vector old_velocity =  m_resting_platform->tangentialVelocity();
            sad::p2d::Vector own_velocity = old_velocity;
            own_velocity.setX(own_velocity.x() + m_own_horizontal_velocity);
            if (this->isYCoordinateFixed())
            {
                own_velocity.setY(m_body->tangentialVelocity().y());
            }
            if (this->isXCoordinateFixed())
            {
                own_velocity.setX(m_body->tangentialVelocity().x());
            }

            m_body->setCurrentTangentialVelocity(own_velocity);
            m_body->sheduleTangentialVelocity(own_velocity);
        }

    }
}


void game::Player::pushOptions(const sad::String& new_options)
{
    if (new_options != m_sprite->optionsName())
    {
        printf("Is not equal\n");
        m_old_options << m_sprite->optionsName();
    }
    printf("Setting options %s\n", new_options.c_str());
    m_sprite->set(new_options);
}

void game::Player::popOptions()
{
    if (m_old_options.size())
    {
        printf("Restoring options %s\n", m_old_options[m_old_options.size() - 1].c_str());
        m_sprite->set(m_old_options[m_old_options.size() - 1]);
        m_old_options.removeAt(m_old_options.size() - 1);
    }
}

void game::Player::startMovingLeft()
{
    this->startMoving(true, game::Player::MaxHorizontalVelocity * -1);
}

void game::Player::startMovingRight()
{
    this->startMoving(false, game::Player::MaxHorizontalVelocity);
}

void game::Player::stopMovingHorizontally()
{
    if (m_is_resting)
    {
        if (!m_is_ducking)
        {
            this->m_sprite->set("enemies_list/playerRed_standng");
        }
    }
    else
    {
        if (!m_is_free_fall)
        {
            this->m_sprite->set("enemies_list/playerRed_up2ng");
        }
    }
    this->setHorizontalVelocity(0);
}

void game::Player::tryJump()
{
    if (this->canJump()) 
    {
        this->stopFallingOrStopDucking();
        this->incrementVerticalVelocity(game::Player::MaxVerticalVelocity);
        this->disableResting();
        this->sprite()->set("enemies_list/playerRed_up1ng");
    }
}

void game::Player::startFallingOrDuck()
{
    if (!m_is_resting) {
        m_is_free_fall = true;
        this->incrementVerticalVelocity(game::Player::MaxVerticalVelocity * -1);
        this->pushOptions("enemies_list/playerRed_fallng");
    } else {
        this->duck();
    }
}

void game::Player::stopFallingOrStopDucking()
{
    if (!m_is_resting) {
        m_is_free_fall = false;
        this->incrementVerticalVelocity(game::Player::MaxVerticalVelocity);
        this->popOptions();
    } else {
        this->stopDucking();
    }
}

void game::Player::duck()
{
    if (!m_is_ducking && m_is_resting)
    {
        m_is_ducking = true;
        this->pushOptions("enemies_list/playerRed_duckng");
        correctShape();
    }
}

void game::Player::stopDucking()
{ 
    if (m_is_ducking && m_is_resting)
    {
        m_is_ducking = false;
        this->popOptions();
        correctShape();
    }
}

bool game::Player::isDucking() const
{
    return m_is_ducking;
}

bool game::Player::isFreefalling() const
{
    return m_is_free_fall;
}

const sad::p2d::Vector& game::Player::oldVelocity() const
{
    return m_old_velocity;
}

// ===================================== PRIVATE METHODS =====================================

void game::Player::startMoving(bool flip_flag, double velocity)
{
    m_sprite->setFlipX(flip_flag);
    if (m_is_resting)
    {
        if (!m_is_ducking)
        {
            m_sprite->set("enemies_list/playerRed_walk1ng");
        }
    }
    this->setHorizontalVelocity(velocity);
}

void game::Player::correctShape()
{
    sad::p2d::Rectangle*  shape = dynamic_cast<sad::p2d::Rectangle*>(m_body->currentShape());
    sad::Point2D start_point = shape->rect()[0];
    sad::Point2D width_height(this->m_sprite->area().width(), this->m_sprite->area().height());
    sad::Rect2D corrected_rect(start_point, start_point + width_height);
    this->m_sprite->setArea(corrected_rect);
    shape->setRect(corrected_rect);
}
