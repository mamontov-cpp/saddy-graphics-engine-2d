/*! \file world.h
    

    Defines world simulation as set of items and callbacks
 */
#pragma once
#include "circletohulltransformer.h"
#include "collisiondetector.h"
#include "simplecollisiondetector.h"
#include "broadcollisiondetector.h"
#include "multisamplingcollisiondetector.h"
#include "collisionhandler.h"

#include "../sadhash.h"
#include "../sadpair.h"

#include "../temporarilyimmutablecontainer.h"

namespace sad
{

namespace p2d
{
class Body;
/*! A world is a set if simulated items, and callbacks used to define
    behaviour between them
 */
class World: public sad::TemporarilyImmutableContainer<p2d::Body>
{
public:
     typedef sad::Pair<sad::String, sad::String> type_pair_t;
     typedef sad::Pair<type_pair_t, sad::p2d::BasicCollisionHandler *> types_with_handler_t;
     typedef sad::Hash<p2d::Body *, sad::Vector<sad::String> > bodies_to_types_t;
     typedef sad::Pair<sad::p2d::BasicCollisionEvent, sad::p2d::BasicCollisionHandler*> 
             reaction_t;
     typedef sad::Vector<reaction_t> reactions_t;
     /*! Compares two reactions, returns true if time of impact of first is lesser
         than second
         \param[in] r1 first reaction
         \param[in] r2 second reaction
         \return whether less
      */
     static bool compare(const reaction_t & r1, const reaction_t & r2);
     /*! Creates world with default transformer
      */
     World();
     /*! Destroys world
      */
     ~World();
     /*! Returns a transformer for all circles
         \return a transformer for all circles
      */
     p2d::CircleToHullTransformer * transformer();
     /*! Sets new detector for a world
         \param[in] d detector
      */
     void setDetector(p2d::CollisionDetector * d);
     /*! Set transformer for a circles
         \param[in] t transformer
      */
     void setTransformer(p2d::CircleToHullTransformer * t);
     /*! Adds new collision handler with specified callbacks
         \param[in] p specified handler
         \return created handler
      */
     template<typename T1, typename T2>
     sad::p2d::BasicCollisionHandler *
     addHandler( void (*p)(const sad::p2d::CollisionEvent<T1, T2> &))
     {
         sad::p2d::BasicCollisionHandler * h = 
             new sad::p2d::FunctionCollisionHandler<T1, T2>(p);
         sad::String t1 = T1::globalMetaData()->name();
         sad::String t2 = T2::globalMetaData()->name();
         this->addHandler(h, t1, t2);
         return h;
     }
     /*! Adds new handler	
         \param[in] p handler
         \return created handler
      */
     sad::p2d::BasicCollisionHandler *
     addHandler( void (*p)(const sad::p2d::BasicCollisionEvent &));
     /*! Adds new handler
         \param[in] o binded object
         \param[in] p new handler
         \return created inner handler
      */
     template<typename _Class, typename T1, typename T2>
     sad::p2d::BasicCollisionHandler *
         addHandler( _Class * o, void (_Class::*p)(const sad::p2d::CollisionEvent<T1, T2> &))
     {
         sad::p2d::BasicCollisionHandler * h = 
             new sad::p2d::MethodCollisionHandler<_Class,T1, T2>(o,p);
         sad::String t1 = T1::globalMetaData()->name();
         sad::String t2 = T2::globalMetaData()->name();
         this->addHandler(h, t1, t2);
         return h;
     }
     /*! Adds new handler
         \param[in] o class
         \param[in] p new handler
         \return created inner handler
      */
     template<typename _Class>
     sad::p2d::BasicCollisionHandler *
         addHandler(_Class * o, void (_Class::*p)(const sad::p2d::BasicCollisionEvent &))
     {
         sad::p2d::BasicCollisionHandler * h = 
             new sad::p2d::MethodCollisionHandler<_Class,p2d::Body, p2d::Body>(o, p);
         sad::String b = "p2d::Body";
         this->addHandler(h, b, b);
         return h;
     }
     /*! Removes handler from a world
         \param[in] h registered handler
      */
     void removeHandler(sad::p2d::BasicCollisionHandler * h);
     /*! When called inside of step() method, makes a world integrate
         velocities and positions to specified time, and restart step, not
         stepping through ghostoptions and forces
         \param[in] time time of item
      */
     void splitTimeStepAt(double time);
     /*! Returns current time step for a world
         \return a time step for a world
      */
     double timeStep() const;
     /*! Steps a world by specified time
         \param[in] time a size of time step
      */
     void step(double time);
     /*! Removes a body from a world. Also clears a user object and body listeners, to make sure,
         that if user object removed before body, it won't be harmed
         \param[in] body a body
     */
     virtual void remove(p2d::Body * body);
     /*! Returns hash code for group, inserting it if required
         \param[in] group a group name
         \param[in] preferred if hash code is not found, this will be returned
         \return hash code
      */
     unsigned int getGroupCode(const sad::String& group, unsigned int preferred = 0);
protected:
     /*! Current time step
      */
     double m_time_step;
     /*! A splitted time step
      */
     sad::Maybe<double> m_splitted_time_step;
     /*! A common transformer for all shapes
      */
     p2d::CircleToHullTransformer * m_transformer;
     /*! A collision dispatcher for testing an items for collision
      */
     p2d::CollisionDetector * m_detector;
     /*! A hash codes for body groups
      */
     sad::Hash<unsigned int, sad::String> m_group_hash_codes;
     /*! A callbacks, with related types
      */
     sad::Vector<types_with_handler_t>  m_callbacks;
     /*! Bodies by groups
      */
     sad::Hash<sad::String, sad::Vector<p2d::Body *> > m_groups;
     /*! All bodies for checking all information
      */
     bodies_to_types_t m_allbodies;
     /*! Builds body's inner caches for shapes and acceleration
      */
     void buildBodyCaches();
     /*! Steps all body options, like ghost options and body
         \param[in] time a time step size
      */
     virtual void stepDiscreteChangingValues(double time);
     /*! Steps a position and velocities
         \param[in] time a time step size
      */
     virtual void stepPositionsAndVelocities(double time);
     /*! Executes a reactions for a world
         \param[in] reactions found reactions
      */ 
     virtual void executeCallbacks(reactions_t & reactions);
     /*! Sorts callbacks. Ascending order of time of impact
         \param[in] reactions found reactions
      */
     virtual void sortCallbacks(reactions_t & reactions);
     /*! Find specific collision events and populates reactions
         \param[in] reactions a reactions
      */
     virtual void findEvents(reactions_t & reactions);
     /*! Finds a specifif collision event and populates reaction
         \param[in] reactions a reactions
         \param[in] twh  types and handlers
      */
     virtual void findEvent(reactions_t & reactions, const types_with_handler_t & twh);
     /*! Finds and executes callbacks
      */
     virtual void findAndExecuteCollisionCallbacks();
     /*! Adds new handler
         \param[in] h handler
         \param[in] t1 first type
         \param[in] t2 second type
      */
     virtual void addHandler(
         sad::p2d::BasicCollisionHandler * h, 
         const sad::String & t1, 
         const sad::String & t2
      );
     /*! Peforms adding a body
         \param[in] o body
      */
     virtual void addNow(p2d::Body * o);
     /*! Peforms removing a body
         \param[in] o body
      */
     virtual void removeNow(p2d::Body * o);
     /*! Clears a world
      */
     virtual void clearNow();
};

}

}
