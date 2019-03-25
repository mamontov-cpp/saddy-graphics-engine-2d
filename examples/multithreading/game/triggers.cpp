#include "triggers.h"
#include "player.h"
#include "../game.h"

#include <log/log.h>

void game::Triggers::add(double x, const sad::dukpp03::CompiledFunction& func, bool once)
{
    m_triggers << game::Triggers::Data(x, func, once);
}

void game::Triggers::clear()
{
    m_triggers.clear();
}

void game::Triggers::tryRun(game::Player* p, sad::dukpp03::Context *ctx)
{
    double localx = p->area()[0].x();
    for(size_t i = 0; i < m_triggers.size(); i++)
    {
        if (localx > m_triggers[i]._1())
        {
            m_triggers[i]._2().call(ctx);
            dukpp03::Maybe<std::string> data = ctx->errorOnStack(-1);
            if (data.exists())
            {
                sad::String log_entry = data.value() + "\n";
                SL_LOCAL_CRITICAL(log_entry, *(p->game()->rendererForMainThread()));
            }
            if (m_triggers[i]._3())
            {
                m_triggers.removeAt(i);
                --i;
            }
        }
    }
}
