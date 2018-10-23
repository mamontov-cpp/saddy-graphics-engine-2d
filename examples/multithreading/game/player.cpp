#include "player.h"
#include <object.h>

#include "../game.h"

DECLARE_COMMON_TYPE(game::Player)

game::Player::Player() : m_is_dead(false)
{

}


game::Player::~Player()
{

}

bool game::Player::setActorOptions(game::ActorOptions* opts)
{
    return m_actor.setOptions(opts);
}

void game::Player::onPlatformCollision(const sad::p2d::BasicCollisionEvent & ev)
{
    m_actor.onPlatformCollision(ev);
}


void game::Player::setGame(Game* game)
{
    m_actor.setGame(game);
}

Game* game::Player::game() const
{
    return m_actor.game();
}

void game::Player::reset()
{
    m_inventory.reset();
    m_actor.reset();
    m_is_dead = false;
    this->setLives(Game::BasicPlayerLivesCount);
}


void game::Player::init(bool no_sound)
{
    m_actor.init(no_sound);
}

bool game::Player::isFloater() const
{
    return m_actor.isFloater();
}

void game::Player::setFloaterState(bool is_floater)
{
    m_actor.setFloaterState(is_floater);
}

game::Inventory* game::Player::inventory()
{
    return &m_inventory;
}


void game::Player::tryStartGoingUp()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStartGoingUp();
}

void game::Player::tryStopGoingUp()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStopGoingUp();
}

void game::Player::tryStartGoingDown()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStartGoingDown();
}

void game::Player::tryStopGoingDown()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStopGoingDown();
}

void game::Player::tryStartGoingLeft()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStartGoingLeft();
}

void game::Player::tryStopGoingLeft()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStopGoingLeft();
}


void game::Player::tryStartGoingRight()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStartGoingRight();
}

void game::Player::tryStopGoingRight()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStopGoingRight();
}

void game::Player::setSprite(sad::Sprite2D* sprite)
{
    m_actor.setSprite(sprite);
}

void game::Player::setBody(sad::p2d::Body* body)
{
    m_actor.setBody(body);
}

sad::Rect2D game::Player::area() const
{
    if (m_is_dead)
    {
        return {};
    }
    return dynamic_cast<sad::p2d::Rectangle*>(m_actor.body()->currentShape())->rect();
}

sad::Point2D game::Player::middle() const
{
    if (m_is_dead)
    {
        return {};
    }
    return m_actor.middle();
}

void game::Player::testResting()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.testResting();
}

void game::Player::enableGravity() const
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.enableGravity();
}


game::Actor* game::Player::actor()
{
    return &m_actor;
}

bool game::Player::isLastMovedLeft() const
{
    return m_actor.isLastMovedLeft();
}

void game::Player::toggleInvincibility(bool on)
{
    m_actor.toggleInvincibility(on);
}

bool game::Player::isInvincible() const
{
    return m_actor.isInvincible();
}

int game::Player::lives() const
{
    return m_actor.lives();
}

void game::Player::setLives(int lives)
{
    m_actor.setLives(lives);
}

 void game::Player::incrementLives(int lives)
{
     m_actor.incrementLives(lives);
}

void game::Player::decrementLives(int lives)
{
    m_actor.decrementLives(lives);
}

void game::Player::onDeath(const std::function<void(game::Actor*)>& action)
{
    m_actor.onDeath(action);
}

void game::Player::setHurtAnimation(sad::animations::Animation* animation)
{
    m_actor.setHurtAnimation(animation);
}

void game::Player::tryDecrementLives(int lives)
{
     m_actor.tryDecrementLives(lives);
}

void game::Player::toggleIsDead(bool is_dead)
{
    m_is_dead = is_dead;
}

bool game::Player::isDead() const
{
    return m_is_dead;
}

