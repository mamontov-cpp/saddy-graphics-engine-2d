/*! \file world.h
    

    Defines world simulation as set of items and callbacks
 */
#pragma once
#include <functional>
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
#include "../sadmutex.h"

namespace sad
{

namespace p2d
{
class Body;

/*! A world is defined as a set if simulated items, stored in groups and a set of
    callbacks used to define interactions between objects in a different groups.
 */
class World: public sad::Object
{
SAD_OBJECT
public:
    /*! A pair of body with it's activity flag with comparison
     */
    struct BodyWithActivityFlag
    {
        /*! A stored body 
         */
        sad::p2d::Body* Body;
        /*! An activity flag
         */
        bool Active;
        /*! Makes new pair
            \param[in] b body
         */
        inline BodyWithActivityFlag(sad::p2d::Body* b) : Body(b), Active(true)
        {
            
        }
        /*! Marks body as inactive
         */
        inline void markAsInactive()
        {
            this->Active = false;
        }
    };
    /*! A body location defines, how body is stored in all bodies vector and in groups
     */
    struct BodyLocation
    {
        /*! Offset of body in a list of all bodies
         */
        size_t OffsetInAllBodies;
        /*! A list of positions of body in group, where value is position
            of group in group list
         */
        sad::Vector<size_t> PositionInGroups;
    };
    /*! A global container for bodies
     */
    struct GlobalBodyContainer
    {
        /*! A storage for storing a body to location positions 
         */
        sad::Hash<sad::p2d::Body*, BodyLocation> BodyToLocation;
        /*! A vector of all bodies. Adding body to container will store an item here
         */
        sad::Vector<sad::p2d::World::BodyWithActivityFlag> AllBodies;
        /*! A vector of free positions in array above to make sure, that we could store some objects here
            in near O(1)
         */
        sad::Vector<size_t> FreePositions;
        /*! Performs action on container
            \param[in] f function
         */
        void performAction(const std::function<void(sad::p2d::Body*)>& f);
        /*! Performs action on all active bodies with specified time step
            \param time_step a time step
         */
        void performActionWithTimeStep(
            void (sad::p2d::Body::*action)(double),
            double time_step
        );
        /*! Sets sampling count for all active bodies
            \param[in] sample_count a sample count
         */
        void setSamplingCount(int sample_count);
        /*! Tries to set transformer for all active bodies
         */
        void trySetTransformer();
        /*! Builds bodys' inner caches required for shapes and acceleration
            \param[in] time_step a current time step for a world
         */
        void buildBodyCaches(double time_step);
        /*! Steps all body options, like ghost options and body
            \param[in] time_step a time step size
         */
        void stepDiscreteChangingValues(double time_step);
        /*! Steps a position and velocities
            \param[in] time_step a time step size
         */
        void stepPositionsAndVelocities(double time_step);
        /*! Adds a body to list
            \param[in] b body
            \return a position of body in list
         */
        BodyLocation& add(sad::p2d::Body* b);
        /*! Removes a body from a container
            \param[in] b body
         */
        void remove(sad::p2d::Body* b);
        /*! Get group locations for a body from a world
            \param[in] b body
            \return result
         */
        const sad::Vector<size_t>& getGroupLocations(sad::p2d::Body* b);
        /*! Clears a container
         */
        void clear();
    };
    /*! A group container for bodies
     */
    struct Group
    {
        /*! A hash table for storing body to location
         */
        sad::Hash<sad::p2d::Body*, size_t> BodyToLocation;
        /*! A linear list of bodies
         */
        sad::Vector<sad::p2d::World::BodyWithActivityFlag> Bodies;
        /*! A vector of free positions in array above to make sure, that we could store some objects here
            in near O(1)
         */
        sad::Vector<size_t> FreePositions;
        /*! Adds body to group
            \param[in] b body
            \return location of body in group
         */
        size_t add(sad::p2d::Body* b);
        /*! Removes body from group
            \param[in] b body
         */
        void remove(sad::p2d::Body* b);
        /*! Returns list of bodies
            \return list of bodies
         */
        const sad::Vector<sad::p2d::World::BodyWithActivityFlag>& bodies() const;
        /*! Returns location for a body in group
           \param[in] b body
           \return  a location of body in vector
         */
        sad::Maybe<size_t> getLocation(sad::p2d::Body* b) const;
        /*! Clears a group
         */
        void clear();
    };
    /*! A pair of group with it's activity flag with comparison
     */
    struct GroupWithActivityFlag
    {
        /*! A stored group
        */
        sad::p2d::World::Group Group;
        /*! An activity flag
        */
        bool Active;
        /*! Makes new group
        */
        inline GroupWithActivityFlag() : Active(true)
        {

        }
        /*! Marks group as inactive
        */
        inline void markAsInactive()
        {
            this->Active = false;
        }
    };
    /*! A group container for groups
     */
    struct GroupContainer
    {
        /*! A group name to location
         */
        sad::Hash<sad::String, size_t> GroupToLocation;
        /*! A list of groups with activity flags
         */
        sad::Vector<sad::p2d::World::GroupWithActivityFlag> Groups;
        /*! A vector of free positions in array above to make sure, that we could store some objects here
           in near O(1)
        */
        sad::Vector<size_t> FreePositions;

        /*! Adds new group into container
            \param[in] name name of a group
         */
        size_t add(const sad::String& name);
        /*! Removes a group from a container
            \param[in] name a name of group
         */
        void remove(const sad::String& name);
        /*! Clears a container from a world
         */
        void clear();
        /*! Returns location for a grouo
           \param[in] name a group name
           \return name of group
         */
        sad::Maybe<size_t> getLocation(const sad::String& name) const;
    };
    /*! A handler list for a group pair
     */
    struct HandlerList
    {
        /*! An index of first group
         */
        size_t TypeIndex1;
        /*! An index of second group
         */
        size_t TypeIndex2;
        /*! A list of handlers to be invoked
         */
        sad::Vector<sad::p2d::BasicCollisionHandler*> List;
    };
    /*! A global handler list 
     */
    struct GlobalHandlerList
    {
        /*! An inner list of handlers
         */
        sad::Vector<sad::p2d::World::HandlerList> List;
        /*! Adds new handler to list
            \param[in] i1 first index
            \param[in] i2 second index
            \param[in] h a handler
         */
        void add(size_t i1, size_t i2, sad::p2d::BasicCollisionHandler* h);
        /*! Removes a handler from a list
            \param[in] h handler
         */
        void remove(sad::p2d::BasicCollisionHandler* h);
        /*! Clears a handler list
         */
        void clear();
    };
    /*! A queued command type for queued commands
     */
    enum QueuedCommandType
    {
        P2D_WORLD_QCT_ADD_BODY = 0,    //!< Add body command
        P2D_WORLD_QCT_REMOVE_BODY = 1, //!< Remove body command

        P2D_WORLD_QCT_ADD_BODY_TO_GROUP = 2,      //!< Add body to group command
        P2D_WORLD_QCT_REMOVE_BODY_FROM_GROUP = 3, //!< Remove body from group command

        P2D_WORLD_QCT_ADD_GROUP = 4,    //!< Add group command
        P2D_WORLD_QCT_REMOVE_GROUP = 5, //!< Remove group command
        P2D_WORLD_QCT_CLEAR_GROUP = 6,  //!< Clear group command

        P2D_WORLD_QCT_ADD_HANDLER = 7,    //!< Add handler command
        P2D_WORLD_QCT_REMOVE_HANDLER = 8, //!< Remove handler command
        P2D_WORLD_QCT_CLEAR_HANDLERS = 9, //!< Clear handlers command

        P2D_WORLD_CLEAR = 10,             //!<  A global clearing command
        P2D_WORLD_STEP = 11,              //!<  A stepping command for a world
        P2D_WORLD_SET_TIME_STEP = 12      //!<  A command for setting a time step
    };
    /*! A queued command as a set of parameters
     */
    struct QueuedCommand
    {
        /*! A type of command
         */
        sad::p2d::World::QueuedCommandType Type;
        /*! A body to add or remove from command
         */
        sad::p2d::Body* Body;
        /*! A group name to be interpreted in queued command
         */
        sad::String GroupName;
        /*! A step value for stepping or setting step
         */
        double StepValue;
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

     /*! TODO: Comment here
      *  \param b
      */
     void add(sad::p2d::Body* b);
     /*! TODO: Comment here
      */
     void clear();
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
    /*! A common transformer for all shapes
     */
    p2d::CircleToHullTransformer * m_transformer;
    /*! A collision dispatcher for testing an items for collision
     */
    p2d::CollisionDetector * m_detector;
    /*! A global body container for storing body references
     */
    sad::p2d::World::GlobalBodyContainer m_global_body_container;
    /*! A group container for storing groups
     */
    sad::p2d::World::GroupContainer m_group_container;
    /*! A global handler list for storing handlers
     */
    sad::p2d::World::GlobalHandlerList m_global_handler_list;
    /*! A command queue for locking a world
     */
    sad::Vector<sad::p2d::World::QueuedCommand> m_command_queue;
    /*! A world lock to support multithreading at least patially
     */
    sad::Mutex m_world_lock;
    /*! Whether world could be changed (not in step)
     */
    bool m_is_locked;

    //! TODO: Redo other parts to support queue

    /*! A splitted time step
      */
     sad::Maybe<double> m_splitted_time_step;
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
