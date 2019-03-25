/*! \file platformpatrolbot.h

    A bot, which patrols a platform
 */
#pragma once
#include "abstractbot.h"

namespace bots
{

/*! A bot, that does nothing
 */
class PlatformPatrolBot : public bots::AbstractBot
{
public:
    /*! A new empty bot
     */
    PlatformPatrolBot();
    /*! Frees all data
     */
    virtual ~PlatformPatrolBot();
    /*! Copies bot's state and returns a new copy
       \return clone of current bot
     */
    virtual bots::AbstractBot* clone() const;
    /*! Performs actors actions on each bot
        \param[in] game a game
        \param[in] actor an actor to be called
     */
    virtual void perform(Game* game, game::Actor* actor);
private:
    /*! Whether bot should walk left
     */
    bool m_is_left;
};

}
