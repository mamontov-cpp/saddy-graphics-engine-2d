/*! \file jsbot.h
 
    A JSBot as function, that evals (actor, state) -> (state)
 */
#pragma once
#include "abstractbot.h"
#include <dukpp-03/context.h>

namespace bots
{ 

/*! A bot, which works as JS function, that evaluates (actor, state) -> (state)
 */
class JSBot: public bots::AbstractBot
{
public:
    /*! Computes bot
        \param[in] fn a function to be evaluated
        \param[in] state a state
        \param[in] context a context
     */
    JSBot(const sad::dukpp03::CompiledFunction& fn, sad::Hash<sad::String, sad::db::Variant> state, sad::dukpp03::Context* context);
    /*! Frees all data
     */
    virtual ~JSBot() override;
    /*! Copies bot's state and returns a new copy
       \return clone of current bot
     */
    virtual bots::AbstractBot* clone() const override;
    /*! Performs actors actions on each bot
        \param[in] game a game
        \param[in] actor an actor to be called
     */
    virtual void perform(Game* game, game::Actor* actor) override;
private:
    /*! A context for evaluation
     */
    sad::dukpp03::Context* m_context;
    /*! A compiled functions
     */
    sad::dukpp03::CompiledFunction m_function;
    /*! An inner state for function
     */
    sad::Hash<sad::String, sad::db::Variant> m_state;
};

}