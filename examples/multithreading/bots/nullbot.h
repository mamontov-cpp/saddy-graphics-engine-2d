/*! \file nullbot.h

    A null bot does nothing
 */
#pragma once
#include "abstractbot.h"

namespace bots
{

/*! A bot, that does nothing
 */
class NullBot : public bots::AbstractBot
{
public:
    /*! A new empty bot
     */
    NullBot();
    /*! Frees all data
     */
    virtual ~NullBot() override;
    /*! Copies bot's state and returns a new copy
       \return clone of current bot
     */
    virtual bots::AbstractBot* clone() const override;
    /*! Performs actors actions on each bot
        \param[in] game a game
        \param[in] actor an actor to be called
     */
    virtual void perform(Game* game, game::Actor* actor) override;
};

}