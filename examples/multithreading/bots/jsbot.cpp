#include "jsbot.h"
#include <utility>

#include "../game.h"

#include "../game/actor.h"

bots::JSBot::JSBot(
    const sad::dukpp03::CompiledFunction& fn,
    sad::Hash<sad::String, sad::db::Variant> state, 
    sad::dukpp03::Context* context
) : m_context(context), m_function(fn), m_state(std::move(state))
{
    
}

bots::JSBot::~JSBot()
{
    
}

bots::AbstractBot* bots::JSBot::clone() const
{
    return new bots::JSBot(m_function, m_state, m_context);
}

void bots::JSBot::perform(Game* game, game::Actor* actor)
{
    ::dukpp03::PushValue<game::Actor*, sad::dukpp03::BasicContext>::perform(m_context, actor);
    ::dukpp03::PushValue<sad::Hash<sad::String, sad::db::Variant>, sad::dukpp03::BasicContext>::perform(m_context, m_state);
    m_function.call(m_context);

    ::dukpp03::Maybe<std::string> data = m_context->errorOnStack(-1);
    if (data.exists())
    {
        sad::String log_entry = data.value() + "\n";
        SL_LOCAL_CRITICAL(log_entry, *(game->rendererForMainThread()));
    }
    else
    {
        ::dukpp03::Maybe<sad::Hash<sad::String, sad::db::Variant> > maybe_state = ::dukpp03::GetValue<sad::Hash<sad::String, sad::db::Variant>, sad::dukpp03::BasicContext>::perform(m_context, -1);
        if (maybe_state.exists())
        {
            m_state = maybe_state.value();
        }
        else
        {
            sad::Renderer* r = actor->game()->rendererForMainThread();
            SL_LOCAL_DEBUG("No state is left, does it exists?", *r);
        }
    }

    m_context->cleanStack();
}