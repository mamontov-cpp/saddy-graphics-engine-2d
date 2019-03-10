/*! \file divingfloaterbot.h


    A bot for floaters, which makes them dive
 */
#pragma once
#include "abstractbot.h"

namespace bots
{


/*! A bot for diving floater, which makes them go next or stop
 */
class DivingFloaterBot: public bots::AbstractBot
{
public:
    /*! Creates new floater, which periodically dives, moving on lie
     * \param from a left offset
     * \param to  a right offset
     * \param top a top offset
     * \param bottom a bottom offset
     */
    DivingFloaterBot(double from, double to, double top, double bottom);
    /*! A destructors
     */
    ~DivingFloaterBot();
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
    /*! A left bound for bot
     */
    double m_left_bound;
    /*! A right bound for bot
     */
    double m_right_bound;
    /*! A top bound for bot
     */
    double m_top_bound;
    /*! A bottom bound for bot
     */
    double m_bottom_bound;

    /*! Is moving from right to left
     */
    bool m_is_left;
    /*! Whether we are diving
     */
    bool m_is_diving;
    /*! Whether we are returning from diving
     */
    bool m_is_returning;
};

}
