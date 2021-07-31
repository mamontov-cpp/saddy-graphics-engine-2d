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
#include "../bots/abstractbot.h"


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
    bots::AbstractBot* Bot;                                 //!< An AI for it
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
     *  \param[in] bot a bot to be added
     *  \return if actor is already here, returns false. Also returns false, if actor is nullptr
     */
    bool add(game::Actor* actor, bots::AbstractBot* bot);
    /*! Removes an actor from list, erasing all data
     *  \param[in] actor an actor
     */
    void remove(game::Actor* actor);
    /*! Removes an actor by body from list 
     *  \param[in] body a body
     */
    void remove(sad::p2d::Body* body);
    /*! Tests if actor is absent in list
     *  \param[in] actor an actor
     *  \return true if dead (defined as absent as list)
     */
    bool isDead(game::Actor* actor) const;
    /*! Sets shooting strategy for bot
     *  \param[in] actor an actor
     *  \param[in] s strategy
     */
    void setStrategyForBot(game::Actor* actor, bots::shootingstrategies::ShootingStrategy* s);
    /*! Processes an actors, performing actions
     *  \param[in] game a game
     */
    void process(Game* game);
    /*! Tests resting
     */
    void testResting();
    /*! Clears actors list
     */
    void clear();
    /*! Pauses weapon reloading for actor
     */
    void pause();
    /*! Resumes weapon reloading for actor
     */
    void resume();
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

