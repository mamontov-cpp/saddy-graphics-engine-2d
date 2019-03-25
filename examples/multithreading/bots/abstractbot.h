/*! \file abstractbot.h
 
    An abstract bot definition
 */
#pragma once
#include "shootingstrategies/shootingstrategy.h"

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
    /*! Default abstract bot
     */
    AbstractBot();
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
    /*! Sets strategy for bot
     *  \param[in] strategy a strategy for bot
     */
    void setStrategy(bots::shootingstrategies::ShootingStrategy* strategy);
    /*! Returns strategy for bot
     *  \return strategy
     */
    bots::shootingstrategies::ShootingStrategy* strategy() const;
protected:
    /*! A local strategy for bot
     */
    bots::shootingstrategies::ShootingStrategy* m_strategy;
};

}