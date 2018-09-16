#include "player.h"

const int game::Player::MaxHorizontalVelocity = 200;
const int game::Player::MaxVerticalVelocity = 400;

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

void game::Player::reset()
{
    m_inventory.reset();
    m_actor.reset();
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

void game::Player::clearFixedFlags()
{
    m_actor.clearFixedFlags();
}

void game::Player::testResting()
{
    m_actor.testResting();
}

void game::Player::checkBoundaryCollision(double left_bound, double right_bound, double up_bound, double bottom_bound)
{
    m_actor.checkBoundaryCollision(left_bound, right_bound, up_bound, bottom_bound);
}

void game::Player::enableGravity() const
{
    m_actor.enableGravity();
}
