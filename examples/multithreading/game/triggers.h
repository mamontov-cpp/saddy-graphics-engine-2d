/*!  \file triggers.h

     A simple triggers, that can be used to perform in-game actions
 */
#include <dukpp-03/context.h>
#include <sadvector.h>
#include <sadpair.h>

#pragma once

namespace game
{

class Player;

/*! A triggers list, used to perform actions
 */
class Triggers
{
public:
    /*! An inner data for storing triggers
     */
    typedef sad::Triplet<double, sad::dukpp03::CompiledFunction, bool> Data;
    /*! We use default constructor
     */
    inline Triggers() = default;
    /*! Adds new trigger on specified x value
     *  \param[in] x horizontal location value
     *  \param[in] func a function
     *  \param[in] once whether trigger should work once
     */
    void add(double x, const sad::dukpp03::CompiledFunction& func, bool once);
    /*! Tries to run triggers
     *  \param[in] player, whose location we should check
     *  \param[in] ctx context ot be runned in
     */
    void tryRun(game::Player* p, sad::dukpp03::Context* ctx);
private:
    /*! A list of triggers to chec
     */
    sad::Vector<game::Triggers::Data> m_triggers;
};

}
