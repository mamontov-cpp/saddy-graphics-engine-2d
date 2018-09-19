#include "actors.h"
#include "../game.h"

// ============================================ PUBLIC METHODS ============================================

game::Actors::Actors(const game::Actors& o)
{
    this->copyState(o);
}

game::Actors& game::Actors::operator=(const game::Actors& o)
{
    this->destroy();
    this->copyState(o);
    return *this;
}

game::Actors::~Actors()
{
    this->destroy();
}


bool game::Actors::add(game::Actor* actor, sad::dukpp03::CompiledFunction fn, const  sad::Hash<sad::String, sad::db::Variant>& state)
{
    if (!actor)
    {
        return false;
    }
    for(size_t i = 0 ; i < m_actors.size(); i++)
    {
        if (m_actors[i]->Actor == actor)
        {
            return false;
        }
    }

    actor->addRef();
    m_actors << new game::Actors::Data{actor, fn, state};
}

void game::Actors::remove(game::Actor* actor)
{
    for(size_t i = 0 ; i < m_actors.size(); i++)
    {
        if (m_actors[i]->Actor == actor)
        {
            actor->delRef();
            delete m_actors[i];
            m_actors.removeAt(i);
            --i;
        }
    }
}

void game::Actors::process(Game* game, sad::dukpp03::Context* ctx)
{
    for(size_t i = 0 ; i < m_actors.size(); i++)
    {
        if (m_actors[i]->Actor->isDying() == false)
        {
            ::dukpp03::PushValue<game::Actor*,sad::dukpp03::BasicContext>::perform(ctx, m_actors[i]->Actor);
            ::dukpp03::PushValue<sad::Hash<sad::String, sad::db::Variant>,sad::dukpp03::BasicContext>::perform(ctx, m_actors[i]->State);
            m_actors[i]->Function.call(ctx);

            ::dukpp03::Maybe<std::string> data = ctx->errorOnStack(-1);
            if (data.exists())
            {
                sad::String log_entry = data.value() + "\n";
                SL_LOCAL_CRITICAL(log_entry, *(game->rendererForMainThread()));
            }
            else
            {
                ::dukpp03::Maybe<sad::Hash<sad::String, sad::db::Variant> > maybe_state = ::dukpp03::GetValue<sad::Hash<sad::String, sad::db::Variant>,sad::dukpp03::BasicContext>::perform(ctx, -1);
                if (maybe_state.exists())
                {
                    m_actors[i]->State = maybe_state.value();
                }
                else
                {
                    printf("No state is left, does it exists");
                }
            }

            ctx->cleanStack();
        }
    }
}

void game::Actors::checkBoundaryCollision(double left_bound, double right_bound, double up_bound, double bottom_bound)
{
    for(size_t i = 0 ; i < m_actors.size(); i++)
    {
        m_actors[i]->Actor->checkBoundaryCollision(left_bound, right_bound, up_bound, bottom_bound);
    }
}

void game::Actors::clear()
{
    this->destroy();
}

// ============================================ PRIVATE METHODS ============================================

void game::Actors::copyState(const game::Actors& o)
{
    for(size_t i = 0; i < o.m_actors.size(); i++)
    {
        game::Actors::Data* data = o.m_actors[i];
        data->Actor->addRef();
        this->m_actors << new game::Actors::Data(*data);
    }
}

void game::Actors::destroy()
{
    for(size_t i = 0; i < m_actors.size(); i++)
    {
        game::Actors::Data* data = m_actors[i];
        data->Actor->delRef();
        delete data;
    }
    m_actors.clear();
}
