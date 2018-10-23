#include "weapon.h"

#include <fuzzyequal.h>

#include "../game/actor.h"
#include "../game.h"

#include "swing.h"
#include "bullet.h"
#include "laser.h"

// =============================== PUBLIC METHODS ===============================

weapons::Weapon::Weapon()  // NOLINT(cppcoreguidelines-pro-type-member-init)
: m_shooting_interval(1000),
m_amount_of_projectiles(1),
m_delay(0),
m_base_damage(1),
m_min_dangle(0),
m_max_dangle(0),
m_elapsed_time(0),
m_is_paused(false)
{
    m_timer.start();
    m_settings.Type = weapons::Weapon::WWT_NONE;
    m_settings.Settings.Bullet = NULL;
    m_settings.Settings.Swing = NULL;
    m_settings.Settings.Laser = NULL;
}

weapons::Weapon::~Weapon()
{
    this->clearSettings();
}

void weapons::Weapon::setShootingInterval(double interval)
{
    m_shooting_interval = interval;
}

double weapons::Weapon::shootingInterval() const
{
    return m_shooting_interval;
}

void weapons::Weapon::setAmountOfProjectiles(size_t projectiles)
{
    m_amount_of_projectiles = projectiles;
}

size_t weapons::Weapon::amountOfProjectiles() const
{
    return m_amount_of_projectiles;
}

void weapons::Weapon::setDelay(double value)
{
    m_delay = value;
}

double weapons::Weapon::delay() const
{
    return m_delay;
}


void weapons::Weapon::setBaseDamage(int damage)
{
    m_base_damage = damage;
}

int weapons::Weapon::baseDamage() const
{
    return m_base_damage;
}

void weapons::Weapon::setMinAngleDelta(double value)
{
    m_min_dangle = value;
}

double weapons::Weapon::minAngleDelta() const
{
    return m_min_dangle;
}

void weapons::Weapon::setMaxAngleDelta(double value)
{
    m_max_dangle = value;
}

double  weapons::Weapon::maxAngleDelta() const
{
    return m_max_dangle;
}

void weapons::Weapon::setSettings(const weapons::SwingSettings& s)
{
    this->clearSettings();
    m_settings.Type = weapons::Weapon::WWT_SWING;
    m_settings.Settings.Swing = new weapons::SwingSettings(s);
}

void weapons::Weapon::setSettings(const weapons::BulletSettings& s)
{
    this->clearSettings();
    m_settings.Type = weapons::Weapon::WWT_BULLET;
    m_settings.Settings.Bullet = new weapons::BulletSettings(s);
}

void weapons::Weapon::setSettings(const weapons::LaserSettings& s)
{
    this->clearSettings();
    m_settings.Type = weapons::Weapon::WWT_LASER;
    m_settings.Settings.Laser = new weapons::LaserSettings(s);
}


void weapons::Weapon::tryShoot(Game* game, game::Actor* actor, double angle)
{
    if (m_is_paused)
    {
        return;
    }
    if (game->isDead(actor))
    {
        return;
    }
    if (m_settings.Type == weapons::Weapon::WWT_NONE || m_amount_of_projectiles == 0)
    {
        return;
    }

    m_timer.stop();
    if (m_elapsed_time + m_timer.elapsed() < m_shooting_interval)
    {
        return;
    }
    m_elapsed_time = 0;

    double dangle = 0;
    double cur_angle = angle + (m_max_dangle - m_min_dangle) / 2.0;

    if (m_amount_of_projectiles > 1)
    {
        dangle = (m_max_dangle - m_min_dangle) / (m_amount_of_projectiles - 1);
        cur_angle = angle + m_min_dangle;
    }

    double local_delay = 0;
    for(size_t i = 0; i < m_amount_of_projectiles; i++)
    {
        this->spawnProjectile(game, actor, cur_angle, local_delay);
        cur_angle += dangle;
        local_delay += m_delay;
    }
}



void weapons::Weapon::pause()
{
    m_is_paused = true;
    m_timer.stop();
    m_elapsed_time += m_timer.elapsed();
    m_paused_timer.start();
}

void weapons::Weapon::resume()
{
    m_is_paused = false;
    m_paused_timer.stop();
    m_elapsed_time += m_paused_timer.elapsed();
    m_timer.start();
}


// =============================== PRIVATE METHODS ===============================

void weapons::Weapon::spawnProjectile(Game* game, game::Actor* actor, double angle, double delay)
{
    if (m_settings.Type == weapons::Weapon::WWT_NONE)
    {
        return;
    }
    if (sad::is_fuzzy_zero(delay))
    {
        if (game->isDead(actor))
        {
            return;
        }
        double base_dmg = this->baseDamage();
        if (m_settings.Type == weapons::Weapon::WWT_SWING)
        {
            weapons::SwingSettings s = *(m_settings.Settings.Swing);
            weapons::Swing* swing = new weapons::Swing(game, actor, s);
            swing->setDamage(actor->modifyDamage(base_dmg));
            game->addProjectile(swing);
        }
        if (m_settings.Type == weapons::Weapon::WWT_BULLET)
        {
            weapons::BulletSettings s = *(m_settings.Settings.Bullet);
            weapons::Bullet* bullet = game->spawnBullet(actor, angle, s);
            bullet->setDamage(actor->modifyDamage(base_dmg));
        }
        if (m_settings.Type == weapons::Weapon::WWT_LASER)
        {
            weapons::LaserSettings s = *(m_settings.Settings.Laser);
            weapons::Laser* laser = new weapons::Laser(game, actor, angle, s);
            laser->setDamage(actor->modifyDamage(base_dmg));
            game->addProjectile(laser);
        }
    }
    else
    {
        double base_dmg = this->baseDamage();
        if (m_settings.Type == weapons::Weapon::WWT_SWING)
        {
            weapons::SwingSettings s = *(m_settings.Settings.Swing);
            game->addDelayedTask(delay, [=] {
                if (game->isDead(actor))
                {
                    return;
                }
                weapons::Swing* swing = new weapons::Swing(game, actor, s);
                swing->setDamage(actor->modifyDamage(base_dmg));
                game->addProjectile(swing);
            });
        }
        if (m_settings.Type == weapons::Weapon::WWT_BULLET)
        {
            weapons::BulletSettings s = *(m_settings.Settings.Bullet);
            game->addDelayedTask(delay, [=] {
                if (game->isDead(actor))
                {
                    return;
                }
                weapons::Bullet* bullet = game->spawnBullet(actor, angle, s);
                bullet->setDamage(actor->modifyDamage(base_dmg));
            });
        }
        if (m_settings.Type == weapons::Weapon::WWT_LASER)
        {
            weapons::LaserSettings s = *(m_settings.Settings.Laser);
            game->addDelayedTask(delay, [=] {
                if (game->isDead(actor))
                {
                    return;
                }
                weapons::Laser* laser = new weapons::Laser(game, actor, angle, s);
                laser->setDamage(actor->modifyDamage(base_dmg));
                game->addProjectile(laser);
            });
        }
    }
}

void weapons::Weapon::clearSettings()
{
    if (m_settings.Type != weapons::Weapon::WWT_NONE)
    {
        if (m_settings.Type == weapons::Weapon::WWT_SWING)
        {
            delete m_settings.Settings.Swing;
        }
        if (m_settings.Type == weapons::Weapon::WWT_BULLET)
        {
            delete m_settings.Settings.Bullet;
        }
        if (m_settings.Type == weapons::Weapon::WWT_LASER)
        {
            delete m_settings.Settings.Laser;
        }
    }
    m_settings.Type = weapons::Weapon::WWT_NONE;
    m_settings.Settings.Bullet = NULL;
    m_settings.Settings.Swing = NULL;
    m_settings.Settings.Laser = NULL;
}
