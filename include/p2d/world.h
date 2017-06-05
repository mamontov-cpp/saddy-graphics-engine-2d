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
#include "../sadvector.h"
#include "../object.h"

#include "../temporarilyimmutablecontainer.h"

namespace sad
{

namespace p2d
{
class Body;
/*! A world is a set if simulated items, and callbacks used to define
    behaviour between them
 */
class World: public sad::Object, public sad::TemporarilyImmutableContainer<p2d::Body>
{
SAD_OBJECT
public:
    /*! A body location defines, how body is stored in all bodies vector and in groups
     */
    struct BodyLocation
    {
        /*! Offset of body in a list of all bodies
         */
        size_t OffsetInAllBodies;
        /*! A list of positions of body in group
         */
        sad::Vector<size_t> PositionInGroups;
    };
    /*! A global container for bodies
     */
    struct GlobalBodyContainer
    {
        /*! A storage for storing a body to location positions 
         */
        sad::Hash<sad::p2d::Body*, BodyLocation> m_body_to_location;
        /*! A vector of all bodies. Adding body to container will store an item here
         */
        sad::Vector<sad::p2d::Body*> m_all_bodies;
    };
    /*! A group container for bodies
     */
    struct Group
    {
        /*! A hash table for storing body to location
         */
        sad::Hash<sad::p2d::Body*, int> m_body_to_location;
        /*! A linear list of bodies
         */
        sad::Vector<sad::p2d::Body*> m_bodies;
    };
    /*! A handler list for a group pair
     */
    struct HandlerList
    {
        /*! An index of first group
         */
        size_t TypeIndex1;
        /*! An index of second grop
         */
        size_t TypeIndex2;
        /*! A list of handlers to be invoked
         */
        sad::Vector<sad::p2d::BasicCollisionHandler*> List;
    };
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
     sad::p2d::BasicCollisionHandler*
     addHandler( void (*p)(const sad::p2d::CollisionEvent<T1, T2> &))
     {
         sad::p2d::BasicCollisionHandler * h = 
             new sad::p2d::FunctionCollisionHandler<T1, T2>(p);
         sad::String t1 = T1::globalMetaData()->name();
         sad::String t2 = T2::globalMetaData()->name();
         this->addHandler(h, t1, t2);
         return h;
     }
     /*! Adds new collision handler with specified callbacks
         \param[in] g1 first group for collision
         \param[in] g2 first group for collision
         \param[in] p specified handler
         \return created handler
      */
     template<typename T1, typename T2>
     sad::p2d::BasicCollisionHandler*
     addHandler(const sad::String& g1, const sad::String& g2, void (*p)(const sad::p2d::CollisionEvent<T1, T2> &))
     {
         sad::p2d::BasicCollisionHandler * h = 
             new sad::p2d::FunctionCollisionHandler<T1, T2>(p, false);
         this->addHandler(h, g1, g2);
         return h;
     }
     /*! Adds new handler   
         \param[in] p handler
         \return created handler
      */
     sad::p2d::BasicCollisionHandler*
     addHandler( void (*p)(const sad::p2d::BasicCollisionEvent &));
     /*! Adds new handler   
        \param[in] g1 first group for handler
        \param[in] g2 second group for handler
        \param[in] p handler
        \return created handler
      */
     sad::p2d::BasicCollisionHandler*
     addHandler(const sad::String& g1, const sad::String& g2, void (*p)(const sad::p2d::BasicCollisionEvent &));
     /*! Adds new handler
         \param[in] o binded object
         \param[in] p new handler
         \return created inner handler
      */
     template<typename _Class, typename T1, typename T2>
     sad::p2d::BasicCollisionHandler*
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
         \param[in] g1 first group for collisions
         \param[in] g2 second group for collisions
         \param[in] o binded object
         \param[in] p new handler
         \return created inner handler
      */
     template<typename _Class, typename T1, typename T2>
     sad::p2d::BasicCollisionHandler*
         addHandler(const sad::String& g1, const sad::String& g2, _Class * o, void (_Class::*p)(const sad::p2d::CollisionEvent<T1, T2> &))
     {
         sad::p2d::BasicCollisionHandler * h = 
             new sad::p2d::MethodCollisionHandler<_Class,T1, T2>(o, p, false);
         this->addHandler(h, g1, g2);
         return h;
     }
     /*! Adds new handler
         \param[in] o class
         \param[in] p new handler
         \return created inner handler
      */
     template<typename _Class>
     sad::p2d::BasicCollisionHandler*
         addHandler(_Class * o, void (_Class::*p)(const sad::p2d::BasicCollisionEvent &))
     {
         sad::p2d::BasicCollisionHandler * h = 
             new sad::p2d::MethodCollisionHandler<_Class,p2d::Body, p2d::Body>(o, p);
         sad::String b = "p2d::Body";
         this->addHandler(h, b, b);
         return h;
     }
     /*! Adds new handler
         \param[in] g1 first group
         \param[in] g2 second group
         \param[in] o class
         \param[in] p new handler
         \return created inner handler
      */
     template<typename _Class>
     sad::p2d::BasicCollisionHandler *
     addHandler(
        const sad::String& g1, 
        const sad::String& g2,
        _Class * o, 
        void (_Class::*p)(const sad::p2d::BasicCollisionEvent &)
     )
     {
         sad::p2d::BasicCollisionHandler * h = 
             new sad::p2d::MethodCollisionHandler<_Class,p2d::Body, p2d::Body>(o, p);
         this->addHandler(h, g1, g2);
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
     /*! Adds new body to group by code
         \param[in] group_code a group code for bodies
         \param[in] b body
         \param[in] to_common whether we should add a body to group for bodies
      */ 
     void addToGroup(unsigned int group_code, p2d::Body* b, bool to_common = false);
     /*! Adds new body to group
         \param[in] group a group for bodies
         \param[in] b body
         \param[in] to_common whether we should add a body to group for bodies
      */ 
     void addToGroup(const sad::String& group, p2d::Body* b, bool to_common = false);
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
     /*! A queue for adding bodies with specified groups as a list of triplets - first argument of triplet
         is whether we should add body to common group, second is a name of group and a third is a body
         itself
      */ 
     sad::Vector<sad::Triplet<bool, sad::String, p2d::Body* > > m_specified_groups_queue;
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
     /*! Adds body to group immediately
         \param[in] b body
         \param[in] g group for body
         \param[in] to_common whether we should add body to common group
      */
     virtual void addBodyToGroupNow(p2d::Body* b, const sad::String& g, bool to_common);
     /*! Performs queued actions for manipulating with container
      */
     virtual void performQueuedActions();
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::World)
