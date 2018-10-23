#include "laser.h"

#include <db/dbdatabase.h>

#include <p2d/rectangle.h>

#include <scene.h>

#include "../game.h"
#include "../game/player.h"


DECLARE_SOBJ_INHERITANCE(weapons::Laser, weapons::Projectile)

weapons::Laser::Laser(Game* game, game::Actor* actor, double angle, const weapons::LaserSettings& settings)
: m_game(game), m_actor(actor), m_sprite(NULL), m_body(NULL), m_max_time(settings.Time)
{
    bool is_player =  game->player()->actor() == actor;
    m_last_actor_middle = actor->middle();
    sad::Point2D point = game->pointOnActorForBullet(actor, angle);
    sad::Renderer* r = game->rendererForMainThread();
    sad::Sprite2D::Options* opts = r->tree()->get<sad::Sprite2D::Options>(settings.IconName);
    double width = settings.Width;
    double height = settings.Height;
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
            middle.x() - height / 2.0, middle.y() - width / 2.0,
            middle.x() + height / 2.0, middle.y() + width / 2.0
        );
        sprite->setArea(rect);
        sprite->setAngle(angle);
        main_scene->addNode(sprite);

        sad::p2d::Rectangle* rectangle = new sad::p2d::Rectangle();
        rectangle->setRect(sprite->renderableArea());

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
    return NULL;
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
        sad::Point2D p = m_actor->middle();
        m_body->move(p - m_last_actor_middle);
        m_sprite->moveBy(p - m_last_actor_middle);
        m_last_actor_middle = p;
        m_timer.stop();
        double elapsed = m_timer.elapsed();
        if (elapsed > m_max_time)
        {
            m_game->killProjectile(this);
        }
    }
}
