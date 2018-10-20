#include "bullet.h"

DECLARE_SOBJ_INHERITANCE(weapons::Bullet, weapons::Projectile)

weapons::Bullet::Bullet() : m_sprite(NULL)
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
