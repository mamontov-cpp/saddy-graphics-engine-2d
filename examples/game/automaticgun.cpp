#include "automaticgun.h"


AbstractAutomaticGun::AbstractAutomaticGun()
{
	m_object = NULL;
	m_dangle = M_PI;
	m_last_shot = 0;
	m_interval = 200; // Fairly random number, as a default
	m_enabled = true;
}

void AbstractAutomaticGun::enable()
{
	m_enabled = true;
}

void AbstractAutomaticGun::disable()
{
	m_enabled = false;
}

void AbstractAutomaticGun::setAngleDifference(double delta)
{
	m_dangle = delta;
}

AbstractAutomaticGun::~AbstractAutomaticGun()
{

}

void AbstractAutomaticGun::setObject(GameObject * o)
{
	m_object = o; 
}

void AbstractAutomaticGun::tryShoot()
{
	double deltaseconds = clock() - m_last_shot;
	double deltamseconds = deltaseconds / CLOCKS_PER_SEC * 1000.0;
	if (deltamseconds > m_interval)
	{
		m_last_shot = clock();
		this->shoot();
	}
}
