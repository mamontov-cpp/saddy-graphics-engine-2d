#include "unanimatedcoins.h"

#include <animations/animationsanimation.h>
#include <animations/animationsinstance.h>

#include <db/dbtable.h>


const double game::UnanimatedCoins::MaxDistance = 1600;

game::UnanimatedCoins::UnanimatedCoins() : m_database(NULL),  m_renderer(NULL), m_animation(NULL)
{

}

sad::Vector<sad::Sprite2D*> game::UnanimatedCoins::fetchCoinSprites(sad::db::Database* db)
{
    sad::Vector<sad::Sprite2D*> result;
    sad::db::Table* tbl = db->table("scenenodes");
    if (tbl)
    {
        sad::Vector<sad::db::Object*> objs = tbl->objectListOfType("sad::Sprite2D");
        for(size_t i = 0; i < objs.size(); i++)
        {
            sad::String name = objs[i]->objectName();
            name.toUpper();
            if (name.getOccurence("COIN") != -1)
            {
                result << static_cast<sad::Sprite2D*>(objs[i]);
            }
        }
    }
    return result;
}

void  game::UnanimatedCoins::init(const sad::Vector<sad::Sprite2D*>& sprites, sad::db::Database* db, sad::Renderer* r)
{
    m_sprites = sprites;
    m_database = db;
    m_renderer = r;
    m_animation = NULL;
}

void game::UnanimatedCoins::animateNearestCoins(const sad::Point2D& middle)
{
    if (m_database && m_renderer)
    {
        if (!m_animation)
        { 
            m_animation = m_database->objectByName<sad::animations::Animation>("coin_flip");
        }
        if (m_animation)
        { 
            for (size_t i = 0; i < m_sprites.size(); i++)
            {
                m_sprites[i]->scene()->setLayer(m_sprites[i], 0);
                double dist = middle.distance(m_sprites[i]->middle());
                if (dist <= game::UnanimatedCoins::MaxDistance)
                {
                    sad::animations::Instance* instance = new sad::animations::Instance();
                    instance->setAnimation(m_animation);
                    instance->setObject(m_sprites[i]);
                    m_renderer->animations()->add(instance);
                    m_sprites.removeAt(i);
                    --i;
                }
            }
        }
    }
}
