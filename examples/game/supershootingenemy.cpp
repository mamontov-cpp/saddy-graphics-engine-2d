#include "supershootingenemy.h"
#include "shootingenemy.h"
#include "game.h"
#include "automaticgun.h"

DECLARE_SOBJ_INHERITANCE(SuperShootingEnemy, GameObject)

SuperShootingEnemy::SuperShootingEnemy()
{
	this->initFromConstants<SuperShootingEnemy>();
	m_hp = 3; // Set hp to 3, as tough enemy

	// Super shooting enemy will shoot shooting enemies in all four directions
	double diffs[4] = { 0, M_PI / 2, M_PI, M_PI * 1.5};
	for(int i = 0; i < 4; i++)
	{
		AbstractAutomaticGun * gun = new AutomaticGun<ShootingEnemy>();
		gun->setAngleDifference(diffs[i]);
		this->addGun(gun);
	}
	this->setAngularVelocity(1.0);
}
