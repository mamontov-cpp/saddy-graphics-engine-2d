#include "automaticgun.h"


AbstractAutomaticGun::AbstractAutomaticGun()
{
	m_object = NULL;
	m_dangle = 0;
	setInterval(200); // Fairly random number, as a default
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
	// Try to perform periodical event
	this->tryPerform();
}
