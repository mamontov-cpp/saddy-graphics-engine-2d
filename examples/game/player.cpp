#include "player.h"
#include "playerbullet.h"
#include "game.h"
#include <renderer.h>
#include "automaticgun.h"
#include <geometry2d.h>

DECLARE_SOBJ_INHERITANCE(Player,GameObject)

Player::Player()
{
	m_score = 0;
	m_hp = 10;

	this->initFromConstants<Player>();
	m_gun = new AutomaticGun<PlayerBullet>();
	m_gun->setAngleDifference(0);
	m_gun->disable();
	this->addGun( m_gun );
}

void Player::tryLookAt(const sad::Event & p)
{
	if (this->game()->isPlaying())
	{
		this->lookAt(sad::Point2D(p.x, p.y));
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

void Player::tryStartMovingLeft(const sad::Event & e)
{
	if (this->game()->isPlaying())
	{
	    this->setHorizontalSpeed(N_SPEED);
		m_stopkeys[0] = e.key;
	}
}

void Player::tryStartMovingRight(const sad::Event & e)
{
	if (this->game()->isPlaying())
	{
		this->setHorizontalSpeed(P_SPEED);
		m_stopkeys[0] = e.key;
	}
}

void Player::tryStartMovingUp(const sad::Event & e)
{
	if (this->game()->isPlaying())
	{
		this->setVerticalSpeed(P_SPEED);
		m_stopkeys[1] = e.key;
	}
}

void Player::tryStartMovingDown(const sad::Event & e)
{
	if (this->game()->isPlaying())
	{
		this->setVerticalSpeed(N_SPEED);
		m_stopkeys[1] = e.key;
	}
}

void Player::tryStopMovingHorizontally(const sad::Event & e)
{
	if (e.key == m_stopkeys[0])
	{
		this->setHorizontalSpeed(0);
	}
}

void Player::tryStopMovingVertically(const sad::Event & e)
{
	if (e.key == m_stopkeys[1])
	{
		this->setVerticalSpeed(0);
	}
}

