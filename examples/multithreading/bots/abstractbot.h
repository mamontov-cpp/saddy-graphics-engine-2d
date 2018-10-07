/*! \file abstractbot.h
 
    An abstract bot definition
 */
#pragma once

class Game;

namespace game
{
class Actor;
}

namespace bots
{

/*! An abstract bot, which can control actors
 */
class AbstractBot
{
public:
    /*! Destroys bot as function
     */
    virtual ~AbstractBot();
    /*! Copies bot's state and returns a new copy
       \return clone of current bot
     */
    virtual bots::AbstractBot* clone() const = 0;
    /*! Performs actors actions on each bot
        \param[in] game a game
        \param[in] actor an actor to be called
     */
    virtual void perform(Game* game, game::Actor* actor) = 0;
};

}