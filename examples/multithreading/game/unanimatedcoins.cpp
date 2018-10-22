#include "unanimatedcoins.h"

#include <db/dbtable.h>


const double game::UnanimatedCoins::MaxDistance = 1600;

game::UnanimatedCoins::UnanimatedCoins() : m_database(NULL),  m_renderer(NULL)
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
            if (name.toUpper().getOccurence("COIN") != -1)
            {
                result << static_cast<sad::Sprite2D*>(objs[i]);
            }
        }
    }
}
