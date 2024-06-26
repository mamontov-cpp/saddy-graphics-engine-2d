#include "laser.h"

#include <db/dbdatabase.h>

#include <p2d/rectangle.h>

#include <scene.h>

#include "../game.h"
#include "../game/player.h"


// ============================================= PUBLIC METHODS =============================================

DECLARE_SOBJ_INHERITANCE(weapons::Laser, weapons::Projectile)

weapons::Laser::Laser(Game* game, game::Actor* actor, double angle, const weapons::LaserSettings& settings)
: m_actor(actor), m_sprite(nullptr), m_body(nullptr), m_max_time(settings.Time), m_dangle(0), m_sprite_width(0), m_physical_width(0), m_height(0)
{
    m_game = game;
    bool is_player =  game->player()->actor() == actor;
    m_dangle = angle - actor->lookupAngle();
    sad::Point2D point = actor->pointForProjectileSpawn(angle);
    sad::Renderer* r = game->rendererForMainThread();
    sad::Sprite2D::Options* opts = r->tree()->get<sad::Sprite2D::Options>(settings.IconName);
    double height = settings.Height;
    m_height = height;
    m_sprite_width = settings.SpriteWidth;
    m_physical_width = settings.PhysicalWidth;
    if (opts)
    {
        sad::db::Database* db = r->database("gamescreen");
        sad::Scene* main_scene = db->objectByName<sad::Scene>("main");

        sad::Sprite2D* sprite = new sad::Sprite2D();
        sprite->setScene(main_scene);
        sprite->setTreeName(r, "");
        sprite->set(*opts);
        sad::Point2D middle(point.x() + height / 2.0 * cos(angle), point.y() + height / 2.0 *  sin(angle));

        sad::Rect2D rect(
            middle.x() - height / 2.0, middle.y() - m_sprite_width / 2.0,
            middle.x() + height / 2.0, middle.y() + m_sprite_width / 2.0
        );
        sprite->setArea(rect);
        sprite->setAngle(angle);
        main_scene->addNode(sprite);

        sad::p2d::Rectangle* rectangle = new sad::p2d::Rectangle();
        rectangle->setRect(weapons::Laser::rect(middle, m_physical_width, m_height, angle));

        sad::p2d::Body* body = new sad::p2d::Body();
        body->setCurrentAngularVelocity(0);
        body->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0));
        body->setUserObject(this);
        body->setShape(rectangle);
        body->initPosition(sprite->middle());
        if (is_player)
        {
            game->physicsWorld()->addBodyToGroup("player_bullets", body);
        }
        else
        {
            game->physicsWorld()->addBodyToGroup("enemy_bullets", body);
        }

        m_sprite = sprite;
        m_body = body;
    }
    m_timer.start();
}


weapons::Laser::~Laser()
{
    
}


game::Actor* weapons::Laser::actor() const
{
    return nullptr;
}

void weapons::Laser::kill()
{
    if (m_body)
    {
        m_game->physicsWorld()->removeBody(m_body);
    }
    if (m_sprite)
    {
        m_sprite->scene()->removeNode(m_sprite);
    }
}

void weapons::Laser::update()
{
    if (m_sprite)
    {
        if (m_game->isDead(m_actor))
        {
            m_game->killProjectile(this);
            return;
        }
        double angle = m_actor->lookupAngle() + m_dangle;
        sad::Point2D point = m_actor->pointForProjectileSpawn(angle);
        sad::Point2D middle(point.x() + m_height / 2.0 * cos(angle), point.y() + m_height / 2.0 *  sin(angle));

        sad::Rect2D rect(
            middle.x() - m_height / 2.0, middle.y() - m_sprite_width / 2.0,
            middle.x() + m_height / 2.0, middle.y() + m_sprite_width / 2.0
        );
        m_sprite->setArea(rect);
        m_sprite->setAngle(angle);

        static_cast<sad::p2d::Rectangle*>(m_body->currentShape())->setRect(weapons::Laser::rect(middle, m_physical_width, m_height, angle));
        m_body->initPosition(m_sprite->middle());


        m_timer.stop();
        double elapsed = m_timer.elapsed();
        if (elapsed > m_max_time)
        {
            m_game->killProjectile(this);
        }
    }
}

// ============================================= PRIVATE METHODS =============================================

sad::Rect2D weapons::Laser::rect(const sad::Point2D& middle, double width, double height, double angle)
{
    sad::Rect2D result(
        middle.x() - height / 2.0, middle.y() - width / 2.0,
        middle.x() + height / 2.0, middle.y() + width / 2.0
    );
    sad::rotate(result, angle);
    return result;
}