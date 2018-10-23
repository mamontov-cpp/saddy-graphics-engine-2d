#include "bullet.h"

DECLARE_SOBJ_INHERITANCE(weapons::Bullet, weapons::Projectile)

weapons::Bullet::Bullet()
: m_sprite(NULL),
m_is_ghost(false),
m_bounce_count_left(0),
m_bounce_resilience_coefficient(1.0),
m_is_piercing(false)
{
    
}

weapons::Bullet::~Bullet()
{

}

void weapons::Bullet::setSprite(sad::Sprite2D* sprite)
{
    m_sprite = sprite;
}


sad::Sprite2D* weapons::Bullet::sprite() const
{
    return m_sprite;
}


void weapons::Bullet::moveBy(const sad::Point2D& p) const
{
    m_sprite->moveBy(p);
}

void weapons::Bullet::rotate(double da) const
{
    m_sprite->rotate(da);
}

bool weapons::Bullet::canBeRotated()
{
    return true;
}

game::Actor* weapons::Bullet::actor() const
{
    return NULL;
}

void weapons::Bullet::kill()
{
    
}

void weapons::Bullet::update()
{
    
}

bool weapons::Bullet::isGhost() const
{
    return m_is_ghost;
}

void weapons::Bullet::setIsGhost(bool is_ghost)
{
    m_is_ghost = is_ghost;
}

void weapons::Bullet::setBounceCountLeft(int amount)
{
    m_bounce_count_left = amount;
}

int weapons::Bullet::bounceCountLeft() const
{
    return m_bounce_count_left;
}

void weapons::Bullet::setBounceResilienceCoefficient(double value)
{
    m_bounce_resilience_coefficient = value;
}

double weapons::Bullet::bounceResilienceCoefficient() const
{
    return m_bounce_resilience_coefficient;
}

void weapons::Bullet::setIsPiercing(bool value)
{
    m_is_piercing = value;
}

bool weapons::Bullet::isPiercing() const
{
    return m_is_piercing;
}
