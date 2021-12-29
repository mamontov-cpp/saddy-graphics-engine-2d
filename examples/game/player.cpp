#include "player.h"
#include "playerbullet.h"
#include "game.h"
#include <renderer.h>
#include "automaticgun.h"
#include <geometry2d.h>
#include <log/log.h>

DECLARE_SOBJ_INHERITANCE(Player,GameObject)

Player::Player() : m_stop_keys{sad::KeyboardKey::A, sad::KeyboardKey::A}
{
    m_score = 0;
    m_hp = 10;

    this->initFromConstants<Player>();
    m_gun = new AutomaticGun<PlayerBullet>();
    m_gun->setAngleDifference(0);
    m_gun->disable();
    this->addGun( m_gun );
}

void Player::tryLookAt(const sad::input::MouseMoveEvent & p)
{
    if (this->game()->isPlaying())
    {
        this->lookAt(p.pos2D());
    }
}


void Player::startShooting()
{
    m_gun->enable();
}

void Player::stopShooting()
{
    m_gun->disable();
}

int Player::score() const
{
    return m_score;
}

int Player::increaseScore(int delta)
{
    m_score +=delta;
    this->game()->trySetHighscore(m_score);
    return m_score;
}

/*! A positive speed as passed distance in second
 */
#define P_SPEED 200.0
/*! A negative speed as passed distance in second
 */
#define N_SPEED -200.0

void Player::tryStartMovingLeft(const sad::input::KeyPressEvent & e)
{
    if (this->game()->isPlaying())
    {
        this->setHorizontalSpeed(N_SPEED);
        m_stop_keys[0] = e.Key;
    }
}

void Player::tryStartMovingRight(const sad::input::KeyPressEvent & e)
{
    if (this->game()->isPlaying())
    {
        this->setHorizontalSpeed(P_SPEED);
        m_stop_keys[0] = e.Key;
    }
}

void Player::tryStartMovingUp(const sad::input::KeyPressEvent & e)
{
    if (this->game()->isPlaying())
    {
        this->setVerticalSpeed(P_SPEED);
        m_stop_keys[1] = e.Key;
    }
}

void Player::tryStartMovingDown(const sad::input::KeyPressEvent & e)
{
    if (this->game()->isPlaying())
    {
        this->setVerticalSpeed(N_SPEED);
        m_stop_keys[1] = e.Key;
    }
}

void Player::tryStopMovingHorizontally(const sad::input::KeyReleaseEvent & e)
{
    if (e.Key == m_stop_keys[0])
    {
        this->setHorizontalSpeed(0);
    }
}

void Player::tryStopMovingVertically(const sad::input::KeyReleaseEvent & e)
{
    if (e.Key == m_stop_keys[1])
    {
        this->setVerticalSpeed(0);
    }
}

