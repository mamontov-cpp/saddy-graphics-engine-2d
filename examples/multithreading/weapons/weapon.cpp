#include "weapon.h"

#include <fuzzyequal.h>

#include <dukpp-03/context.h>

#include "../game/actor.h"
#include "../game.h"

#include "swing.h"
#include "bullet.h"
#include "laser.h"


DECLARE_SOBJ(weapons::Weapon)

// =============================== PUBLIC METHODS ===============================

weapons::Weapon::Weapon()  // NOLINT(cppcoreguidelines-pro-type-member-init)
: m_shooting_interval(1000),
m_amount_of_projectiles(1),
m_delay(0),
m_base_damage(1),
m_min_dangle(0),
m_max_dangle(0)
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
    if (m_settings.Settings.Swing->SoundName.empty()) m_settings.Settings.Swing->SoundName = "swing";
}

void weapons::Weapon::setSettings(const weapons::BulletSettings& s)
{
    this->clearSettings();
    m_settings.Type = weapons::Weapon::WWT_BULLET;
    m_settings.Settings.Bullet = new weapons::BulletSettings(s);
    if (m_settings.Settings.Bullet->SoundName.empty()) m_settings.Settings.Bullet->SoundName = "shooting_1";
}

void weapons::Weapon::setSettings(const weapons::LaserSettings& s)
{
    this->clearSettings();
    m_settings.Type = weapons::Weapon::WWT_LASER;
    m_settings.Settings.Laser = new weapons::LaserSettings(s);
    if (m_settings.Settings.Laser->SoundName.empty()) m_settings.Settings.Laser->SoundName = "shooting_2";
}


void weapons::Weapon::tryShoot(Game* game, game::Actor* actor)
{
    if (m_timer.paused())
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

    double angle = actor->lookupAngle();

    double elapsed = m_timer.elapsed();
    if ( elapsed < m_shooting_interval)
    {
        return;
    }
    m_timer.start();

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
    m_timer.pause();
}

void weapons::Weapon::resume()
{
    m_timer.resume();
}


// =============================== PRIVATE METHODS ===============================

void weapons::Weapon::spawnProjectile(Game* game, game::Actor* actor, double angle, double delay)
{
    if (m_settings.Type == weapons::Weapon::WWT_NONE)
    {
        return;
    }
    int base_dmg = this->baseDamage();
    if (sad::is_fuzzy_zero(delay))
    {
        if (game->isDead(actor))
        {
            return;
        }
        weapons::Projectile* projectile = this->makeProjectile(game, actor, angle);
        projectile->setDamage(actor->modifyDamage(base_dmg));
        game->addProjectile(projectile);
    }
    else
    {
        double dangle = angle - actor->lookupAngle();
        game->addDelayedTask(delay, [=] {
            if (game->isDead(actor))
            {
                return;
            }
            weapons::Weapon* weapon = actor->weapon();
            if (weapon)
            { 
                weapons::Projectile* projectile = actor->weapon()->makeProjectile(game, actor, actor->lookupAngle() + dangle);
                projectile->setDamage(actor->modifyDamage(base_dmg));
                game->addProjectile(projectile);
            }
        });
    }
}

weapons::Projectile* weapons::Weapon::makeProjectile(Game* game, game::Actor* actor, double angle) const
{
    switch (m_settings.Type)
    {
        case weapons::Weapon::WWT_NONE:
            return NULL;
        case weapons::Weapon::WWT_SWING:
            {
                weapons::SwingSettings s = *(m_settings.Settings.Swing);
                actor->game()->playSound(s.SoundName);
                return new weapons::Swing(game, actor, s);
            }
        case weapons::Weapon::WWT_BULLET:
            {
                weapons::BulletSettings s = *(m_settings.Settings.Bullet);
                actor->game()->playSound(s.SoundName);
                return new weapons::Bullet(game, actor, angle, s);;
            }
        case weapons::Weapon::WWT_LASER:
            {
                weapons::LaserSettings s = *(m_settings.Settings.Laser);
                actor->game()->playSound(s.SoundName);
                return new weapons::Laser(game, actor, angle, s);
            }
    }
    return NULL;
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


void weapons::exposeWeapon(void* c)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    sad::dukpp03::ClassBinding* weapon_binding = new sad::dukpp03::ClassBinding();
    weapon_binding->addObjectConstructor<weapons::Weapon>("Weapon");

    weapon_binding->addMethod("setShootingInterval", sad::dukpp03::bind_method::from(&weapons::Weapon::setShootingInterval));
    weapon_binding->addMethod("shootingInterval", sad::dukpp03::bind_method::from(&weapons::Weapon::shootingInterval));
    weapon_binding->addMethod("setAmountOfProjectiles", sad::dukpp03::bind_method::from(&weapons::Weapon::setAmountOfProjectiles));
    weapon_binding->addMethod("amountOfProjectiles", sad::dukpp03::bind_method::from(&weapons::Weapon::amountOfProjectiles));
    weapon_binding->addMethod("setDelay", sad::dukpp03::bind_method::from(&weapons::Weapon::setDelay));
    weapon_binding->addMethod("delay", sad::dukpp03::bind_method::from(&weapons::Weapon::delay));
    weapon_binding->addMethod("setBaseDamage", sad::dukpp03::bind_method::from(&weapons::Weapon::setBaseDamage));
    weapon_binding->addMethod("baseDamage", sad::dukpp03::bind_method::from(&weapons::Weapon::baseDamage));
    weapon_binding->addMethod("setMinAngleDelta", sad::dukpp03::bind_method::from(&weapons::Weapon::setMinAngleDelta));
    weapon_binding->addMethod("minAngleDelta", sad::dukpp03::bind_method::from(&weapons::Weapon::minAngleDelta));
    weapon_binding->addMethod("setMaxAngleDelta", sad::dukpp03::bind_method::from(&weapons::Weapon::setMaxAngleDelta));
    weapon_binding->addMethod("maxAngleDelta", sad::dukpp03::bind_method::from(&weapons::Weapon::maxAngleDelta));

    void (weapons::Weapon::*m_s1)(const weapons::SwingSettings&) = &weapons::Weapon::setSettings;
    void (weapons::Weapon::*m_s2)(const weapons::BulletSettings&) = &weapons::Weapon::setSettings;
    void (weapons::Weapon::*m_s3)(const weapons::LaserSettings&) = &weapons::Weapon::setSettings;

    sad::dukpp03::MultiMethod* set_settings = new sad::dukpp03::MultiMethod();
    set_settings->add(sad::dukpp03::bind_method::from(m_s1));
    set_settings->add(sad::dukpp03::bind_method::from(m_s2));
    set_settings->add(sad::dukpp03::bind_method::from(m_s3));
    weapon_binding->addMethod("setSettings", set_settings);

    ctx->addClassBinding("weapons::Weapon", weapon_binding);
}
