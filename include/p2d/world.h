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
class World: public sad::Object  // NOLINT(cppcoreguidelines-special-member-functions)
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
    struct BodyLocation // NOLINT(cppcoreguidelines-pro-type-member-init)
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
        /*! Removes body from group. If all groups are removed, removes it from a container
            \param[in] b body
            \param[in] group_offset an offset for group
         */
        void removeFromGroup(sad::p2d::Body* b, size_t group_offset);
        /*! Returns amount of active bodies in container
            \return body count
         */
        size_t bodyCount();
        /*! Returns a list of active bodies in container
            \return a list of bodies
         */
        sad::Vector<sad::p2d::Body*> activeBodies();
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
        /*! Returns amount of bodies in group
            \return body count
         */
        size_t bodyCount();
        /*! Returns a list of active bodies in container
            \return a list of bodies
         */
        sad::Vector<sad::p2d::Body*> activeBodies();
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
        /*! Clear all bodies from a group
         */
        void clearBodies();
        /*! Returns location for a body in a group
           \param[in] name a group name
           \return name of group
         */
        sad::Maybe<size_t> getLocation(const sad::String& name) const;
        /*! Makes group and adds body to it, updating body location
            \param[in] group_name a name of group
            \param[in] body a body
            \param[out] loc updated location
         */
        void makeGroupAndAddBody(const sad::String& group_name, sad::p2d::Body* body, sad::p2d::World::BodyLocation& loc);
        /*! Returns amount of active groups
            \return amount of active groups
         */
        size_t groupCount();
        /*! Returns list of existing groups
            \return list of existing groups
         */
        sad::Vector<sad::String> existingGroups() const;
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
        sad::Vector<sad::p2d::BasicCollisionHandler*>* List;
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
        /*! Removes a handler from list
            \param[in] i1 first index
            \param[in] i2 second index
            \param[in] h a handler
         */
        void remove(size_t i1, size_t i2, sad::p2d::BasicCollisionHandler* h);
        /*! Removes a handler from a list
            \param[in] h handler
         */
        void remove(sad::p2d::BasicCollisionHandler* h);
        /*! Clears a handler list
         */
        void clear();
        /*! Clears a list for groups
            \param[in] i1 a group index
            \param[in] i2 a second group index
         */
        void clearForGroups(size_t i1, size_t i2);
        /*! Removes a handler for a list
            \param[in] location a location
         */
        void removeHandlersFor(size_t location);
        /*! Returns total handler count
            \return total handler count
         */
        size_t totalHandlerCount();
        /*! Returns total handler count for groups
            \param[in] i1 first group index
            \param[in] i2 second group index
            \return handler count
         */
        size_t totalHandlerCount(size_t i1, size_t i2);
        /*! Returns amount of times, handler occurs in container
            \param[in] h handler
            \return amount of times
         */
        size_t totalHandlerOccurences(sad::p2d::BasicCollisionHandler* h);
        /*! Returns amount of times, handler occurs in container group pair
            \param[in] i1 first group index
            \param[in] i2 second group index
            \param[in] h handler
            \return amount of times
         */
        size_t totalHandlerOccurences(size_t i1, size_t i2, sad::p2d::BasicCollisionHandler* h);
        /*! Returns a handler list for all of groups 
            \return a handler list
         */
        sad::Vector<sad::p2d::BasicCollisionHandler*> handlers();
        /*! Returns a handler list for all of objects
            \param[in] i1 first group index
            \param[in] i2 second group index
            \return a handler list
         */
        sad::Vector<sad::p2d::BasicCollisionHandler*> handlers(size_t i1, size_t i2);
    };
    /*! A queued command type for queued commands
     */
    enum QueuedCommandType
    {
        P2D_WORLD_QCT_ADD_BODY = 0,     //!< Add body command
        P2D_WORLD_QCT_REMOVE_BODY = 1,  //!< Remove body command
        P2D_WORLD_QCT_CLEAR_BODIES = 2, //!< Clears all bodies

        P2D_WORLD_QCT_ADD_BODY_TO_GROUP = 3,      //!< Add body to group command
        P2D_WORLD_QCT_REMOVE_BODY_FROM_GROUP = 4, //!< Remove body from group command

        P2D_WORLD_QCT_ADD_GROUP = 5,    //!< Add group command
        P2D_WORLD_QCT_REMOVE_GROUP = 6, //!< Remove group command
        P2D_WORLD_QCT_CLEAR_GROUP = 7,  //!< Clear group command
        P2D_WORLD_QCT_CLEAR_GROUPS = 8, //!< Clears all groups, removing them from a world

        P2D_WORLD_QCT_ADD_HANDLER = 9,                //!< Add handler command
        P2D_WORLD_QCT_REMOVE_HANDLER_FROM_GROUPS = 10,//!< Removes handler from specified groups 
        P2D_WORLD_QCT_REMOVE_HANDLER = 11,            //!< Remove handler command
        P2D_WORLD_QCT_CLEAR_HANDLERS = 12,            //!< Clear handlers command
        P2D_WORLD_QCT_CLEAR_HANDLERS_FOR_GROUPS = 13, //!< Clears handlers for specified groups

        P2D_WORLD_QCT_CLEAR = 14,                     //!<  A global clearing command
        P2D_WORLD_QCT_STEP = 15,                      //!<  A stepping command for a world
    };
    /*! A queued command as a set of parameters
     */
    struct QueuedCommand  // NOLINT(cppcoreguidelines-pro-type-member-init)
    {
        /*! A type of command
         */
        sad::p2d::World::QueuedCommandType Type;
        /*! A handler for adding or removing one
         */
        sad::p2d::BasicCollisionHandler* Handler;
        /*! A body to add or remove from command
         */
        sad::p2d::Body* Body;
        /*! A group name to be interpreted in queued command or first group name
            for handler
         */
        sad::String GroupName;
        /*! A second group name for a handler
         */
        sad::String SecondGroupName;
        /*! A step value for stepping or setting step
         */
        double StepValue;
    };
    /*! A collision event with  a callback, that it can be performed on.
        Used to store all the collision event's, that were occured in world
     */
    struct EventWithCallback
    {
        /*! An event
         */
        sad::p2d::BasicCollisionEvent Event;
        /*! A callback, that should be invoked with event
         */
        sad::Vector<sad::p2d::BasicCollisionHandler*>* CallbackList;
        /*! Makes empty structure for event with callback
         */
        inline EventWithCallback() : CallbackList(nullptr)
        {

        }
        /*! Makes new event with callback
            \param ev event
            \param cblst callback list
         */
        inline EventWithCallback(const  sad::p2d::BasicCollisionEvent& ev, sad::Vector<sad::p2d::BasicCollisionHandler*>* cblst) : Event(ev), CallbackList(cblst)
        {

        }
        /*! A comparison operator, that only will compare time
            \param[in] ewc other object
            \return comparison results
         */
        inline bool operator<(const sad::p2d::World::EventWithCallback& ewc) const
        {
            return this->Event.m_time < ewc.Event.m_time;
        }
        /*! A comparison operator, that only will compare time
            \param[in] ewc other object
            \return comparison results
         */
        inline bool operator>(const sad::p2d::World::EventWithCallback& ewc) const
        {
            return this->Event.m_time > ewc.Event.m_time;
        }
        /*! A comparison operator, that only will compare time. A precision
            is set to 0.001 to ensure, that most events will differ from each other more than this value.
            In practice, world has detection precision no more than this value.
            \param[in] ewc other object
            \return comparison results
         */
        inline bool operator==(const sad::p2d::World::EventWithCallback& ewc) const
        {
            return sad::is_fuzzy_equal(this->Event.m_time, ewc.Event.m_time, 0.001);
        }

        /*! A comparison operator, that only will compare time. A precision
            is set to 0.001 to ensure, that most events will differ from each other more than this value.
            In practice, world has detection precision no more than this value.

            \param[in] ewc other object
            \return comparison results
         */
        inline bool operator!=(const sad::p2d::World::EventWithCallback& ewc) const
        {
            return !((*this) == ewc);
        }

        /*! A comparison operator, that only will compare time
            \param[in] ewc other object
            \return comparison results
         */
        inline bool operator>=(const sad::p2d::World::EventWithCallback& ewc) const
        {
            return ((*this) >= ewc) || ((*this) == ewc);
        }

        /*! A comparison operator, that only will compare time
            \param[in] ewc other object
            \return comparison results
         */
        inline bool operator<=(const sad::p2d::World::EventWithCallback& ewc) const
        {
            return ((*this) <= ewc) || ((*this) == ewc);
        }
        /*! Invokes a callback
         */
        inline void operator()() const
        {
            if (CallbackList)
            {
                sad::Vector<sad::p2d::BasicCollisionHandler*>& lst = *CallbackList;
                for(size_t i = 0; i < lst.size(); i++)
                {
                    lst[i]->invoke(Event);
                }
            }
        }
    };
    /*! A list pf events with callbacks
     */
    typedef sad::Vector<EventWithCallback> EventsWithCallbacks;
public:
    /*! Creates world with default transformer
     */
    World();
    /*! Destroys world
     */
    virtual ~World();
    /*! Returns a transformer for all circles
        \return a transformer for all circles
     */
    sad::p2d::CircleToHullTransformer * transformer() const;
    /*! Sets new detector for a world
        \param[in] d detector
     */
    void setDetector(sad::p2d::CollisionDetector * d);
    /*! Returns a collision detector for a world
        \return detector
     */
    sad::p2d::CollisionDetector* detector() const;
    /*! Returns current time step for a world
        \return a time step for a world
     */
    double timeStep() const;
    /*! Set transformer for a circles
        \param[in] t transformer
     */
    void setTransformer(sad::p2d::CircleToHullTransformer * t);


    /*! Adds new body to a groups, defined by type of user object and common group, if
        any
        \param[in] b body
     */
    void addBody(sad::p2d::Body* b);
    /*! Removes a body from all groups, that bodies in in
        \param[in] b body
     */
    void removeBody(sad::p2d::Body* b);
    /*! Clear bodies list, removing all bodies from world
     */
    void clearBodies();
    /*! Adds new body to group, if group exists
        \param[in] group_name a group name for bodies
        \param[in] b body
     */
    void addBodyToGroup(const sad::String& group_name, sad::p2d::Body* b);
    /*! Removes a body from a group if group exists
        \param[in] group_name a group name for bodies
        \param[in] b body
     */
    void removeBodyFromGroup(const sad::String& group_name, sad::p2d::Body* b);
    /*! Clears a group by name
        \param[in] group_name a name of group
     */
    void clearGroup(const sad::String& group_name);

    /*! Adds new group by name
        \param[in] group_name a name of group
     */
    void addGroup(const sad::String& group_name);
    /*! Removes group by name
        \param[in] group_name a name of group
     */
    void removeGroup(const sad::String& group_name);
    /*! Clears all groups in world, removing all bodies
     */
    void clearGroups();

    /*! Adds new handler for handling collisions between groups.
        Handler is added only if two groups are here
        \param[in] first_group_name name of first group, object from which will be passed as first argument to callback.
        \param[in] second_group_name name of second group, object from which will be passed as first argument to callback
        \param[in] h handler
        \return a handler
    */
    sad::p2d::BasicCollisionHandler*  addHandler(
        const sad::String & first_group_name,
        const sad::String & second_group_name,
        sad::p2d::BasicCollisionHandler* h
    );
    /*! Removes a handler for handling collisions between groups.
        \param[in] first_group_name name of first group, object from which will be passed as first argument to callback.
        \param[in] second_group_name name of second group, object from which will be passed as first argument to callback
        \param[in] h handler
        \return a handler
    */
    void removeHandlerFromGroups(
        const sad::String & first_group_name,
        const sad::String & second_group_name,
        sad::p2d::BasicCollisionHandler* h
    );
    /*! Removes handler from a world if it is stored in world
        \param[in] h registered handler
     */
    void removeHandler(sad::p2d::BasicCollisionHandler* h);
    /*! Clears handlers, removing all handlers in world
     */
    void clearHandlers();
    /*! Clears handlers, specified for certain groups
        \param[in] first_group first group to specify handler's first argument
        \param[in] second_group a second group to specify handler's second argument
     */
    void clearHandlersForGroups(const sad::String& first_group, const sad::String& second_group);
    /*! Clears world, removing all objects, groups and handlers from it.
     */
    void clear();

    /*! Steps a world by specified time
        \param[in] time a size of time step
     */
    void step(double time);

    /*! Returns true, if body is in world
        \param[in] b body
        \return whether body is in world
     */
    bool isBodyInWorld(sad::p2d::Body* b);

    /*! Returns true if body is in group
        \param[in] group_name a name of group
        \param[in] b a body
        \return whether it exists
     */
    bool isInGroup(const sad::String& group_name, sad::p2d::Body* b);

    /*! Returns true if group with specified name exists in world
        \param[in] group_name a name of of group
        \return whether it exists
     */
    bool doesGroupExists(const sad::String& group_name);

    /*! Amount of total active bodies in world
        \return body count
     */
    size_t totalBodyCount();
    /*! Amount of total active groups in world
        \return body count
     */
    size_t totalGroupCount();
    /*! Returns amount of bodies in group or 0 if group does not exists
        \param[in] group_name a name of group
        \return amount of objects
     */
    size_t amountOfBodiesInGroup(const sad::String& group_name);

    /*! Returns list of all active bodies in world
        \return list of all active bodies in world 
     */
    sad::Vector<sad::p2d::Body*> allBodies();
    /*! Returns list of existing groups in world
        \return existing groups
     */
    sad::Vector<sad::String> existingGroups();
    /*! Returns list all bodies in group
        \return list all bodies in group
     */
    sad::Vector<sad::p2d::Body*> allBodiesInGroup(const sad::String& group_name);

    /*! Returns total amount of handlers in world
        \return total amount of handlers in world
     */
    size_t amountOfHandlers();
    /*! Returns total amount of handlers in pair of groups
        \param[in] s1 first group name
        \param[in] s2 second group name
        \return handler count
     */
    size_t amountOfHandlersForGroups(const sad::String& s1, const sad::String& s2);
    /*! Returns amount of times, handler occurs in container
        \param[in] h handler
        \return amount of times
     */
    size_t totalHandlerOccurences(sad::p2d::BasicCollisionHandler* h);
    /*! Returns amount of times, handler occurs in container group pair
        \param[in] s1 first group
        \param[in] s2 second group
        \param[in] h handler
        \return amount of times
     */
    size_t totalHandlerOccurencesInGroups(const sad::String& s1, const sad::String& s2, sad::p2d::BasicCollisionHandler* h);
    /*! Returns whether handler is in world
        \param[in] h handler
        \return whether handler is in world
     */
    bool isHandlerInWorld(sad::p2d::BasicCollisionHandler* h);
    /*! Returns amount of times, handler occurs in container group pair
        \param[in] s1 first group
        \param[in] s2 second group
        \param[in] h handler
        \return amount of times
     */
    bool isHandlerInGroups(const sad::String& s1, const sad::String& s2, sad::p2d::BasicCollisionHandler* h);

    /*! Returns all handlers, existing in a world
        \return all handlers, existing in a world
     */
    sad::Vector<sad::p2d::BasicCollisionHandler*> allHandlers();
    /*! Returns all handlers, that are binded to group pair in exact order
        \param[in] s1 first group
        \param[in] s2 second group
        \return all handlers, that are binded to group pair in exact order
    */
    sad::Vector<sad::p2d::BasicCollisionHandler*> allHandlersForGroups(const sad::String& s1, const sad::String& s2);

    /*! Adds new collision handler with specified typed callback
        \param[in] first_group a first group, where handler should be applied to
        \param[in] second_group a second group, where handler should be applied to
        \param[in] f a lambda function to be added
        \return created handler
     */
    template<typename T1, typename T2>
    sad::p2d::BasicCollisionHandler* addHandler(
        const sad::String& first_group,
        const sad::String& second_group,
        const std::function<void(const sad::p2d::CollisionEvent<T1, T2>&)>& f
    )
    {
        sad::p2d::BasicCollisionHandler* h = new sad::p2d::TypedCollisionHandler<T1, T2>(f);
        addHandler(first_group, second_group, h);
        return h;
    }

    /*! Adds new collision handler with specified typed callback
        \param[in] f a lambda function to be added
        \return a handler
     */
    template<typename T1, typename T2>
    sad::p2d::BasicCollisionHandler* addHandler(const std::function<void(const sad::p2d::CollisionEvent<T1, T2>&)>& f)
    {
        sad::String t1 = T1::globalMetaData()->name();
        sad::String t2 = T2::globalMetaData()->name();
        return this->addHandler<T1, T2>(t1, t2, f);
    }

    /*! Adds new collision handler with specified callbacks
        \param[in] p specified handler
        \return created handler
    */
    template<typename T1, typename T2>
    sad::p2d::BasicCollisionHandler*
    addHandler(void (*p)(const sad::p2d::CollisionEvent<T1, T2>&))
    {
        sad::String t1 = T1::globalMetaData()->name();
        sad::String t2 = T2::globalMetaData()->name();
        sad::p2d::BasicCollisionHandler* h = new sad::p2d::TypedCollisionHandler<T1, T2>(p);
        addHandler(t1, t2, h);
        return h;
    }
    /*! Adds new collision handler for grpoups with specified callback
        \param[in] g1 first group for collision
        \param[in] g2 first group for collision
        \param[in] p specified handler
        \return created handler
    */
    template<typename T1, typename T2>
    sad::p2d::BasicCollisionHandler*
    addHandler(const sad::String& g1, const sad::String& g2, void (*p)(const sad::p2d::CollisionEvent<T1, T2>&))
    {
        sad::p2d::BasicCollisionHandler* h = new sad::p2d::TypedCollisionHandler<T1, T2>(p);
        return addHandler(g1, g2, h);
    }

    /*! Adds new handler for group
        \param[in] first_group a first group
        \param[in] second_group a second group
        \param[in] f a function
        \return a handler
    */
    inline sad::p2d::BasicCollisionHandler*
    addHandler(const sad::String& first_group, const sad::String& second_group, const std::function<void(const sad::p2d::BasicCollisionEvent &)>& f)
    {
        sad::p2d::BasicCollisionHandler* h = new sad::p2d::UntypedCollisionHandler(f);
        return this->addHandler(first_group, second_group, h);
    }

    /*! Adds new handler for group
        \param[in] f a function
        \return a handler
    */
    inline sad::p2d::BasicCollisionHandler*
    addHandler(const std::function<void(const sad::p2d::BasicCollisionEvent &)>& f)
    {
        sad::p2d::BasicCollisionHandler* h = new sad::p2d::UntypedCollisionHandler(f);
        return this->addHandler("p2d::Body", "p2d::Body", h);
    }

    /*! Adds new handler
        \param[in] o object, that method will be called on
        \param[in] p new handler
        \return created inner handler
    */
    template<typename _Class, typename T1, typename T2>
    sad::p2d::BasicCollisionHandler*
    addHandler(_Class* o, void (_Class::*p)(const sad::p2d::CollisionEvent<T1, T2>&))
    {
        return this->addHandler<T1, T2>([o, p](const sad::p2d::CollisionEvent<T1, T2>& ev) {
            (o->*p)(ev);
        });
    }
    /*! Adds new handler
        \param[in] g1 first group for collisions
        \param[in] g2 second group for collisions
        \param[in] o object, used to call method on
        \param[in] p a handler method
        \return created inner handler
    */
    template<typename _Class, typename T1, typename T2>
    sad::p2d::BasicCollisionHandler*
    addHandler(const sad::String& g1, const sad::String& g2, _Class* o, void (_Class::*p)(const sad::p2d::CollisionEvent<T1, T2>&))
    {
        return this->addHandler<T1, T2>(g1, g2, [o, p](const sad::p2d::CollisionEvent<T1, T2>& ev) {
            (o->*p)(ev);
        });
    }
    /*! Adds new handler
        \param[in] o class
        \param[in] p new handler
        \return created inner handler
    */
    template<typename _Class>
    sad::p2d::BasicCollisionHandler*
    addHandler(_Class* o, void (_Class::*p)(const sad::p2d::BasicCollisionEvent&))
    {
        return this->addHandler("p2d::Body", "p2d::Body", [o, p](const sad::p2d::BasicCollisionEvent& ev) {
            (o->*p)(ev);
        });
    }
    /*! Adds new handler for specified groups
        \param[in] first_group a first group
        \param[in] second_group a second group
        \param[in] o an object to call method on
        \param[in] p a method to be called
        \return created inner handler
    */
    template<typename _Class>
    sad::p2d::BasicCollisionHandler *
    addHandler(
        const sad::String& first_group,
        const sad::String& second_group,
        _Class* o,
        void (_Class::*p)(const sad::p2d::BasicCollisionEvent &)
    )
    {
        return this->addHandler(first_group, second_group, [o, p](const sad::p2d::BasicCollisionEvent& ev) {
            (o->*p)(ev);
        });
    }
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
    sad::Vector<sad::p2d::World::QueuedCommand>* m_command_queue;
    /*! A lock for adding commands into command queue
     */
    sad::Mutex m_command_queue_lock;
    /*! A world lock to support multithreading at least patially
     */
    sad::Mutex m_world_lock;
    /*! A lock for lockes flag
     */
    sad::Mutex m_is_locked_lock;
    /*! Whether world could be changed (not in step)
     */
    bool m_is_locked;

    /*! Returns whether world is locked and cannot be changed, not in all
        kinds
        \return whether world is locked for changes
     */
    bool isLockedForChanges();
    /*! Sets is locked flag
        \param[in] is_locked a value for a flag
     */
    void setIsLockedFlag(bool is_locked);
    /*! Adds new command to a queue
        \param[in] c a command
     */
    void addCommand(const sad::p2d::World::QueuedCommand& c);
    /*! Extracts every command from a queue and tries to perform them,
        effectively changing container
     */
    void performQueuedCommands();

    /*! Peforms adding a body to a world
        \param[in] b body
     */
    void addBodyNow(sad::p2d::Body* b);
    /*! Peforms removing a body from all groups and a world
        \param[in] b body
     */
    void removeBodyNow(sad::p2d::Body* b);
    /*! Clears all bodies, erasing them from a world
     */
    void clearBodiesNow();
    /*! Adds a body to specified group
        \param[in] group_name a group name
        \param[in] o body
     */
    void addBodyToGroupNow(const sad::String& group_name, sad::p2d::Body* o);
    /*! Adds a body to specified group
        \param[in] group_name a group name
        \param[in] o body
     */
    void removeBodyFromGroupNow(const sad::String& group_name, sad::p2d::Body* o);
    /*! Add a group to a list
        \param group_name a name for group
     */
    void addGroupNow(const sad::String& group_name);
    /*! Removes a group from a list list
        \param group_name a name for group
     */
    void removeGroupNow(const sad::String& group_name);
    /*! Removes all bodies, from a specified group
        \param[in] group_name a group name
     */
    void clearGroupNow(const sad::String& group_name);
    /*! Erases all groups from a list
     */
    void clearGroupsNow();

    /*! Adds new handler for groups
        \param[in] group_name_1 a name for group, objects from which will be passed as first parameter to callback
        \param[in] group_name_2 a name for group, objects from which will be passed as second parameter to callback
        \param[in] h a handler
     */
    void addHandlerNow(
        const sad::String & group_name_1,
        const sad::String & group_name_2,
        sad::p2d::BasicCollisionHandler* h
    );
    /*! Removes a handler for handling collisions between groups.
        \param[in] first_group_name name of first group, object from which will be passed as first argument to callback.
        \param[in] second_group_name name of second group, object from which will be passed as first argument to callback
        \param[in] h handler
        \return a handler
    */
    void removeHandlerFromGroupsNow(
        const sad::String & first_group_name,
        const sad::String & second_group_name,
        sad::p2d::BasicCollisionHandler* h
    );
    /*! Removes a handler from a world
        \param h a handler
     */
    void removeHandlerNow(sad::p2d::BasicCollisionHandler* h);
    /*! Clears all handlers from a world
     */
    void clearHandlersNow();
    /*! Clears handlers, specified for certain groups
        \param[in] first_group first group to specify handler's first argument
        \param[in] second_group a second group to specify handler's second argument
     */
    void clearHandlersForGroupsNow(const sad::String& first_group, const sad::String& second_group);
    /*! Clears whole world at the moment
     */
    void clearNow();
    /*! Steps a world with specified time step
        \param time_step a time step
     */
    void stepNow(double time_step);
    /*! Find specific collision events and populates reactions
        \param[in] ewc events with callbacks
     */
    void findEvents(sad::p2d::World::EventsWithCallbacks& ewc);
    /*! Finds a specific collision event and populates reaction
        \param[in] ewc events with callbacks
        \param[in] lst a handler list to be used
     */
    void findEvent(sad::p2d::World::EventsWithCallbacks& ewc, sad::p2d::World::HandlerList& lst);
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::World)
