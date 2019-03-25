/*! \file wayfloater.h


    A bot for floaters, which makes them float on a way
 */
#pragma once
#include "abstractbot.h"

#include <p2d/app/way.h>

namespace bots
{


/*! A bot for floater, that moves on way
 */
class WayFloater : public bots::AbstractBot
{
public:
    /*! Creates new floater 
     *  \param[in] start_time a starting time for floater
     * \param[in] name for way, that floater is bound to
     */
    WayFloater(double start_time, const sad::String& name);
    /*! A destructors
     */
    ~WayFloater();
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
    /*! A local time for floater
     */
    double m_time;
    /*! A way data
     */
    sad::p2d::app::Way* m_way;
    /*! A name for floater
     */
    sad::String m_way_name;
};

}
