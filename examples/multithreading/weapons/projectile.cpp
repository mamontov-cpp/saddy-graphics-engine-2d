#include "projectile.h"

DECLARE_SOBJ(weapons::Projectile)

weapons::Projectile::Projectile() : m_damage(1)
{
    
}


weapons::Projectile::~Projectile()
{
    
}

void weapons::Projectile::setDamage(int damage)
{
    m_damage = damage;
}

int weapons::Projectile::damage() const
{
    return m_damage;
}