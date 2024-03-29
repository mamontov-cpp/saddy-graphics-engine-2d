/*!  \file triggers.h

     A simple triggers, that can be used to perform in-game actions
 */
#pragma once
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
    /*! Clears list of triggers
     */
    void clear();
    /*! Tries to run triggers
     *  \param[in] p player, whose location we should check
     *  \param[in] ctx context to be launched in
     */
    void tryRun(game::Player* p, sad::dukpp03::Context* ctx);
private:
    /*! A list of triggers to check
     */
    sad::Vector<game::Triggers::Data> m_triggers;
};

}
