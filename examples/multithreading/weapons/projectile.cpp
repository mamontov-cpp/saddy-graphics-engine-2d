#include "projectile.h"

#include "../game.h"
#include "../game/actor.h"
#include "../game/player.h"

DECLARE_SOBJ(weapons::Projectile)

weapons::Projectile::Projectile() : m_damage(1), m_game(nullptr)
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

void weapons::Projectile::onEnemyHit(game::Actor* actor)
{
    if (m_game)
    {
        if (actor)
        {
            if (!actor->isInvincible())
            {
                m_game->playSound("hit_enemy");
            }
        }
    }

}

void weapons::Projectile::onPlatformHit(sad::p2d::Body*)
{

}

void weapons::Projectile::onPlayerHit(game::Player* player)
{
    if (m_game)
    {
        if (player)
        {
            if (!player->isInvincible())
            {
                m_game->playSound("hurt");
            }
        }
    }
}

void weapons::Projectile::onWallHit(sad::p2d::Body*)
{

}
