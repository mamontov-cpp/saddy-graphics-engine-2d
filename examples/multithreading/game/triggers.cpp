#include "triggers.h"
#include "player.h"

void game::Triggers::add(double x, const sad::dukpp03::CompiledFunction& func, bool once)
{
    m_triggers << game::Triggers::Data(x, func, once);
}

void game::Triggers::tryRun(game::Player* p, sad::dukpp03::Context *ctx)
{
    double localx = p->area()[0].x();
    for(size_t i = 0; i < m_triggers.size(); i++)
    {
        if (localx > m_triggers[i]._1())
        {
            m_triggers[i]._2().call(ctx);
            if (m_triggers[i]._3())
            {
                m_triggers.removeAt(i);
                --i;
            }
        }
    }
}
