#include "bullet.h"

#include "../game.h"
#include "../game/actor.h"
#include "../game/player.h"

#include <renderer.h>
#include <sprite2d.h>

#include <pipeline/pipeline.h>

#include <p2d/weight.h>
#include <p2d/world.h>

#include <animations/animationssimplemovement.h>
#include <animations/animationsinstance.h>


DECLARE_SOBJ_INHERITANCE(weapons::Bullet, weapons::Projectile)

/** A lowest bullet speed 
 */
#define LOWEST_BULLET_SPEED (10)

weapons::Bullet::Bullet(Game* game, game::Actor* actor, double angle, const weapons::BulletSettings& settings)
: m_sprite(nullptr),
m_body(nullptr),
m_is_ghost(false),
m_bounce_count_left(0),
m_bounce_restitution_coefficient(1.0),
m_is_piercing(false),
m_should_decay(true)
{
    m_game = game;
    this->setIsGhost(settings.IsGhost);
    this->setBounceCountLeft(settings.MaxBounceCount);
    this->setBounceRestitutionCoefficient(settings.RestitutionCoefficient);
    this->setIsPiercing(settings.IsPiercing);

    bool is_player = game->player()->actor() == actor;
    while(angle < 0)
    {
        angle += 2* M_PI;
    }

    while (angle > 2 * M_PI)
    {
        angle -= 2 * M_PI;
    }

    sad::Renderer* r = game->rendererForMainThread();
    sad::Sprite2D::Options* opts = r->tree()->get<sad::Sprite2D::Options>(settings.IconName);

    sad::Point2D result_middle = actor->pointForProjectileSpawn(angle);

    sad::Sprite2D* sprite = new sad::Sprite2D();
    sad::Scene* main_scene = actor->scene();

    sprite->setScene(actor->scene());
    sprite->setTreeName(r, "");
    sprite->set(settings.IconName);
    double halfwidth = 5, halfheight = 5;
    if (opts)
    {
        halfwidth = opts->Rectangle.width() / 2.0;
        halfheight = opts->Rectangle.height() / 2.0;
    }
    result_middle.setX(result_middle.x() + halfwidth * cos(angle));
    result_middle.setY(result_middle.y() + halfheight * sin(angle));
    sad::Rect2D area(result_middle.x() - halfwidth, result_middle.y() - halfheight, result_middle.x() + halfwidth, result_middle.y() + halfheight);
    sprite->setArea(area);

    main_scene->addNode(sprite);

    m_sprite = sprite;

    sad::p2d::Rectangle* rectangle = new sad::p2d::Rectangle();
    rectangle->setRect(sprite->renderableArea());

    double speed = settings.Speed;
    if (fabs(speed) < LOWEST_BULLET_SPEED)
    {
        speed = LOWEST_BULLET_SPEED; // Set speed to 10 if zero, to avoid slowest bullets
    }

    sad::p2d::Body* body = new sad::p2d::Body();
    body->setCurrentAngularVelocity(settings.AngularSpeed);
    body->setCurrentTangentialVelocity(sad::p2d::Vector(speed * cos(angle), speed * sin(angle)));
    body->attachObject(this);
    body->setShape(rectangle);
    body->initPosition(sprite->middle());
    if (settings.ApplyGravity)
    {
        sad::Point2D gravity = settings.GravityValue;
        if (settings.IsNavigatingGravityToDirection)
        {
            if (!((angle < M_PI / 2.0) || (angle > 1.5 * M_PI)))
            {
                gravity.setX(-(gravity.x)());
            }
        }
        body->addForce(new sad::p2d::TangentialForce(gravity));
    }
    m_body = body;

    if (is_player)
    {
        game->physicsWorld()->addBodyToGroup("player_bullets", body);
    }
    else
    {
        game->physicsWorld()->addBodyToGroup("enemy_bullets", body);
    }
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
    return nullptr;
}

void weapons::Bullet::kill()
{
    if (!m_sprite)
    {
        return;
    }
    if (!m_game)
    {
        return;
    }
    sad::Scene* scene = m_sprite->scene();
    sad::Point2D middle = m_sprite->middle();

    sad::String options_name = m_sprite->optionsName();
    sad::Sprite2D::Options* opts = scene->renderer()->tree()->get<sad::Sprite2D::Options>(options_name);

    Game* game = m_game;
    m_game->rendererForMainThread()->pipeline()->appendTask([=]() {
        if (m_sprite)
        {
            scene->removeNode(m_sprite);
        }
        if (m_body)
        {
            if (game->physicsWorld())
            { 
                game->physicsWorld()->removeBody(m_body);
            }
        }
    });


    if (m_should_decay)
    {
        double dist  = 10;
        sad::Point2D vectors[4] = {
            sad::Point2D(-dist, -dist),
            sad::Point2D(dist, -dist),
            sad::Point2D(-dist, dist),
            sad::Point2D(dist, dist)
        };
        for (auto vector : vectors)
        {
            sad::Sprite2D* anim_sprite = new sad::Sprite2D();
            anim_sprite->setScene(scene);
            anim_sprite->setTreeName(scene->renderer(), "");
            anim_sprite->set(options_name);

            sad::Rect2D rect(
                middle.x() - opts->Rectangle.width() / 4.0, -middle.x() - opts->Rectangle.height() / 4.0,
                middle.x() + opts->Rectangle.width() / 4.0, -middle.x() + opts->Rectangle.height() / 4.0
            );
            anim_sprite->setArea(rect);
            scene->addNode(anim_sprite);

            sad::animations::SimpleMovement* movement = new sad::animations::SimpleMovement();
            movement->setTime(300);
            movement->setStartingPoint(middle);
            movement->setEndingPoint(middle + vector);

            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setObject(anim_sprite);
            instance->setAnimation(movement);
            instance->end([=] { anim_sprite->scene()->removeNode(anim_sprite); });

            scene->renderer()->animations()->add(instance);
        }
    }
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

void weapons::Bullet::setBounceRestitutionCoefficient(double value)
{
    m_bounce_restitution_coefficient = value;
}

double weapons::Bullet::bounceRestitutionCoefficient() const
{
    return m_bounce_restitution_coefficient;
}

void weapons::Bullet::setIsPiercing(bool value)
{
    m_is_piercing = value;
}

bool weapons::Bullet::isPiercing() const
{
    return m_is_piercing;
}


void weapons::Bullet::onPlatformHit(sad::p2d::Body* b)
{
    if (!m_game)
    {
        return;
    }
    if (!this->isGhost())
    {
        m_game->playSound("hit");
        if (this->bounceCountLeft() <= 0)
        {
            m_game->killProjectile(this);
        }
        else
        {
            m_bounce_count_left -= 1;
            sad::p2d::BounceSolver* bs = m_game->bounceSolverForBullets();
            bs->pushRestitutionCoefficient(this->bounceRestitutionCoefficient());
            sad::p2d::Weight w = b->weight();
            b->setWeight(sad::p2d::Weight::infinite());
            bs->bounce(m_body, b);
            // Restore weight;
            b->setWeight(w);
        }
    }
}


void weapons::Bullet::onWallHit(sad::p2d::Body*)
{
    m_should_decay = false;
    m_game->killProjectile(this);
}


void weapons::Bullet::onPlayerHit(game::Player* player)
{
    this->weapons::Projectile::onPlayerHit(player);
    if (this->isPiercing())
    {
        return;
    }
    if (player)
    {
        if (!player->isInvincible())
        {
            m_game->killProjectile(this);
        }
    }
}

void weapons::Bullet::onEnemyHit(game::Actor* enemy)
{
    this->weapons::Projectile::onEnemyHit(enemy);
    if (this->isPiercing())
    {
        return;
    }
    if (enemy)
    {
        if (!enemy->isInvincible())
        {
            m_game->killProjectile(this);
        }
    }
}


