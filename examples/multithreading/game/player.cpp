#include "player.h"
#include <object.h>

#include "../game.h"

DECLARE_COMMON_TYPE(game::Player)

game::Player::Player()
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
    m_actor.tryStartGoingUp();
}

void game::Player::tryStopGoingUp()
{
    m_actor.tryStopGoingUp();
}

void game::Player::tryStartGoingDown()
{
    m_actor.tryStartGoingDown();
}

void game::Player::tryStopGoingDown()
{
    m_actor.tryStopGoingDown();
}

void game::Player::tryStartGoingLeft()
{
    m_actor.tryStartGoingLeft();
}

void game::Player::tryStopGoingLeft()
{
    m_actor.tryStopGoingLeft();
}


void game::Player::tryStartGoingRight()
{
    m_actor.tryStartGoingRight();
}

void game::Player::tryStopGoingRight()
{
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

sad::Rect2D game::Player::area()
{
    return dynamic_cast<sad::p2d::Rectangle*>(m_actor.body()->currentShape())->rect();
}

sad::Point2D game::Player::middle()
{
    return m_actor.middle();
}

void game::Player::testResting()
{
    m_actor.testResting();
}

void game::Player::enableGravity() const
{
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


