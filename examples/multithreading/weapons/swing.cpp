#include "swing.h"

#include <db/dbdatabase.h>

#include <p2d/rectangle.h>

#include <scene.h>

#include "../game.h"

DECLARE_SOBJ_INHERITANCE(weapons::Swing, weapons::Projectile)

const double weapons::Swing::LeftAngleStart = 0;
const double weapons::Swing::LeftAngleEnd = 101;
const double weapons::Swing::RightAngleStart = 180;
const double weapons::Swing::RightAngleEnd = 79;


weapons::Swing::Swing(Game* game, game::Actor* actor, const weapons::SwingSettings& settings)
: m_swing_sprite(NULL),
m_body(NULL),
m_attached_actor(actor),
m_game(game),  
m_max_time(settings.DecayTime)
{
    m_left = actor->isLastMovedLeft();
    bool is_player = game->player()->actor() == actor;
    sad::Renderer* r  = game->rendererForMainThread();
    sad::Sprite2D::Options* opts =  r->tree()->get<sad::Sprite2D::Options>(settings.IconName);
    if (opts)
    {
        sad::db::Database* db = r->database("gamescreen");
        sad::Scene* main_scene = db->objectByName<sad::Scene>("main");

        sad::Sprite2D* sprite = new sad::Sprite2D();
        sprite->setScene(main_scene);
        sprite->setTreeName(r, "");
        sprite->set(*opts);

        sad::Rect2D area = actor->sprite()->area();
        m_local_width = opts->Rectangle.width() * settings.ScaleFactor;
        m_local_height = opts->Rectangle.height() * settings.ScaleFactor;
        if (!m_left)
        {
            sad::Rect2D rect(area[2].x(), area[2].y(), area[2].x() + m_local_width, area[2].y() + m_local_height);
            sprite->setArea(rect);
            sprite->setFlipY(true);
            sprite->setAngle(weapons::Swing::RightAngleStart / 180.0 * M_PI);
        }
        else
        {
            sad::Rect2D rect(area[0].x() - m_local_width, area[2].y(), area[0].x(), area[2].y() + m_local_height);
            sprite->setArea(rect);
            sprite->setAngle(weapons::Swing::LeftAngleStart / 180.0 * M_PI);
        }

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

        m_swing_sprite = sprite;
        m_body = body;
    }
    m_timer.start();
}


weapons::Swing::~Swing()
{
}

game::Actor* weapons::Swing::actor() const
{
    return m_attached_actor;
}


void weapons::Swing::kill()
{
    if (m_body)
    { 
        m_game->physicsWorld()->removeBody(m_body);
    }
    if (m_swing_sprite)
    {
        m_swing_sprite->scene()->removeNode(m_swing_sprite);
    }
}

void weapons::Swing::update()
{
    if (m_swing_sprite)
    {
        if (m_game->isDead(m_attached_actor))
        {
            m_game->killProjectile(this);
            return;
        }
        m_timer.stop();
        double elapsed = m_timer.elapsed();
        if (elapsed > m_max_time)
        {
            m_game->killProjectile(this);
            return;
        }

        double angle;
        if (m_left)
        {
            angle = weapons::Swing::LeftAngleStart + (weapons::Swing::LeftAngleEnd  - weapons::Swing::LeftAngleStart) / m_max_time * elapsed;
        }
        else
        {
            angle = weapons::Swing::RightAngleStart + (weapons::Swing::RightAngleEnd - weapons::Swing::RightAngleStart) / m_max_time * elapsed;
        }
        angle = angle / 180.0 * M_PI;
        double highest_y = m_attached_actor->sprite()->area()[2].y();;
        double lowest_y = m_attached_actor->sprite()->area()[0].y() - m_local_height;
        double lowy = highest_y + (lowest_y - highest_y) / m_max_time * elapsed;
        double lowx = (m_left) ? (m_attached_actor->sprite()->area()[0].x() - m_local_width)  : m_attached_actor->sprite()->area()[2].x();
        
        sad::Rect2D area(lowx, lowy, lowx + m_local_width, lowy + m_local_height);
        m_swing_sprite->setArea(area);
        m_swing_sprite->setAngle(angle);

        static_cast<sad::p2d::Rectangle*>(m_body->currentShape())->setRect(m_swing_sprite->renderableArea());
    }
}
