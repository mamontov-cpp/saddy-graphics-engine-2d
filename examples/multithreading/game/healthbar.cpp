#include "healthbar.h"
#include "../game.h"

#include <camera.h>

#include <db/dbdatabase.h>

#define MAX_VISIBLE_HEARTS (10)

DECLARE_SOBJ_INHERITANCE(game::HealthBar, sad::SceneNode)

const sad::Point2D game::HealthBar::TopLeftAngle(56, 593);

const sad::Point2D game::HealthBar::HeartSize(30, 24);

const double game::HealthBar::Padding = 2;

game::HealthBar::HealthBar(Game* game) : m_game(game)
{
    sad::db::Database* db = game->rendererForMainThread()->database("gamescreen");

    m_scene = db->objectByName<sad::Scene>("gui");

    m_life_label = db->objectByName<sad::Label>("LifeLabel");

    m_multiplier_label = db->objectByName<sad::Label>("LifeMultiplier");
    m_multiplier_label->setVisible(false);

    m_multiplier_label_point = m_multiplier_label->point();
    m_life_label_point = m_life_label->point();

    m_heart  = new sad::Sprite2D();
    m_heart->addRef();
    m_heart->setTreeName(game->rendererForMainThread(), "");
    m_heart->set("heart/heart");
}


game::HealthBar::~HealthBar()
{
    m_heart->delRef();
}

void game::HealthBar::setTreeName(sad::Renderer* renderer, const sad::String& tree_name)
{
    this->sad::SceneNode::setTreeName(renderer, tree_name);
    m_heart->setTreeName(renderer, tree_name);
}

void game::HealthBar::setScene(sad::Scene* scene)
{
    this->sad::SceneNode::setScene(scene);
    m_heart->setScene(scene);
}

void game::HealthBar::rendererChanged()
{
    this->sad::SceneNode::rendererChanged();
    m_heart->rendererChanged();
}

void game::HealthBar::render()
{
    int lives = m_game->player()->lives();
    sad::Point2D gox = m_game->rendererForMainThread()->globalTranslationOffset();
    double x = gox.x() * (-1);
    double y = gox.y();
    m_multiplier_label->setVisible(lives > MAX_VISIBLE_HEARTS);

    m_multiplier_label->setPoint(m_multiplier_label_point + sad::Point2D(x, y));
    m_life_label->setPoint(m_life_label_point + sad::Point2D(x, y));

    if (lives > 0)
    {
        if (lives > MAX_VISIBLE_HEARTS)
        {
            m_heart->setArea(sad::Rect2D(
               x + game::HealthBar::TopLeftAngle.x()                                 , y + game::HealthBar::TopLeftAngle.y() - game::HealthBar::HeartSize.y(),
               x + game::HealthBar::TopLeftAngle.x() + game::HealthBar::HeartSize.x(), y + game::HealthBar::TopLeftAngle.y()
            ));
            m_multiplier_label->setString(sad::String("x ") + sad::String::number(lives));
            m_heart->render();
        }
        else
        {
            double xstep = game::HealthBar::HeartSize.x() + game::HealthBar::Padding;
            for(int i = 1; i <= lives; i++)
            {
                int ii = i -1;
                double xleft = game::HealthBar::TopLeftAngle.x() + xstep * ii;
                m_heart->setArea(sad::Rect2D(
                    x + xleft,  y + game::HealthBar::TopLeftAngle.y() - game::HealthBar::HeartSize.y(),
                    x + xleft + game::HealthBar::HeartSize.x(), y + game::HealthBar::TopLeftAngle.y()
                ));
                m_heart->render();
            }
        }
    }
}