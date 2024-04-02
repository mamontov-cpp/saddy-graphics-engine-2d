#include "abstractautomaticgun.h"
#include "gameobject.h"

AbstractAutomaticGun::AbstractAutomaticGun()
{
    m_object = nullptr;
    m_dangle = 0;
    setInterval(200); // Fairly random number, as a default

    // An additional group for objects
    this->addAdditionalGroupForObjects("GameObject");
}

double AbstractAutomaticGun::angle()
{
    return 0;
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

sad::phy52d::App * AbstractAutomaticGun::app()
{
    return m_object->app();
}
