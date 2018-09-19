/*!  \file actors.h
 *
 *   Defines an actors, that will be processed in game time, every tick
 */
#pragma once
#include <dukpp-03/context.h>

#include <db/dbvariant.h>

#include <sadhash.h>
#include <sadstring.h>

#include "actor.h"


class Game;

namespace game
{

/*! A list of actors
 */
class Actors
{
public:
/*! A data state, holds info on actors
 */
struct Data
{
    game::Actor* Actor;                                     //!< A stored actor in-game
    sad::dukpp03::CompiledFunction Function;          //!< A compiled function
    sad::Hash<sad::String, sad::db::Variant> State;   //!< An inner variant for state
};
public:
    /*! Inits default actors list
     */
    inline Actors() = default;
    /*! Constructs new actors list
        \param[in] o other actor
     */
    Actors(const game::Actors& o);
    /*! Copies other actor
     *  \param[in] o other actor
     *  \return data
     */
    game::Actors& operator=(const game::Actors& o);
    /*! Destroys new list
     */
    ~Actors();
    /*! Tries to add new actor in list
     *  \param[in] actor an actor
     *  \param[in] fn function (actor, state) -> (state), which performs an action. Can use other global stuff
     *  \param[in] state a state for working
     *  \return if actor is already here, returns false. Also returns false, if actor is NULL
     */
    bool add(game::Actor* actor, const sad::dukpp03::CompiledFunction& fn, const  sad::Hash<sad::String, sad::db::Variant>& state);
    /*! Removes an actor from list, erasing all data
     *  \param[in] actor an actor
     */
    void remove(game::Actor* actor);
    /*! Processes an actors, performing actions
     *  \param[in] game a game
     *  \param[in] ctx context
     */
    void process(Game* game, sad::dukpp03::Context* ctx);
    /*! Checks boundary collision for horizontal collision
        \param[in] left_bound a left bound
        \param[in] right_bound a right bound
        \param[in] up_bound an upper bound for actor
        \param[in] bottom_bound a bottom bound for actor
     */
    void checkBoundaryCollision(double left_bound, double right_bound, double up_bound, double bottom_bound);
    /*! Clear fixed flags
     */
    void clearFixedFlags();
    /*! Performs forces if velocity were changed
     */
    void performForceActionIfVelocityWereChanged();
    /*! Clears actors list
     */
    void clear();
private:
    /*! Copies state of actors from other actor
     * \param[in] o object
     */
    void copyState(const game::Actors& o);
    /*! Clears actors list
     */
    void destroy();
    /*! A list of actors for actors
     */
    sad::Vector<game::Actors::Data*> m_actors;
};

}

