/*! \file temporarilyimmutablecontainer.h
    

    Describes a temporarily immutable container, which is locked for actions, in between of 
    performing some actions, like inserting or deleting or clearing a container.
 */
#pragma once
#include "sadmutex.h"
#include "sadvector.h"


namespace sad
{

/*! A heterogeneous temporarily immutable container, where 
    added and removed objects can have different type
 */
template<
    typename _AddedObjectType,
    typename _RemovedObjectType
>
class TemporarilyImmutableContainerWithHeterogeneousCommands
{
public:
    /*! As a default, container is mutable
     */
    TemporarilyImmutableContainerWithHeterogeneousCommands()
    {
        m_lock_changes = false;
    }
    /*! Adds new object to container
        \param[in] o object
     */
    virtual void add(const _AddedObjectType & o)
    {
        if (m_lock_changes)
        {
            QueuedCommand  c;
            c.Type = CT_ADD;
            c.Added = o;
            pushCommand(c);
        }
        else
        {
            m_mutability_lock.lock();
            addNow(o);
            m_mutability_lock.unlock();
        }
    }
    /*! Removes an object
        \param[in] o object
     */
    virtual void remove(const _RemovedObjectType & o)
    {
        if (m_lock_changes)
        {
            QueuedCommand  c;
            c.Type = CT_REMOVE;
            c.Removed = o;
            pushCommand(c);
        }
        else
        {
            m_mutability_lock.lock();
            removeNow(o);
            m_mutability_lock.unlock();
        }
    }
    /*! Clears a container
     */
    virtual void clear()
    {
        if (m_lock_changes)
        {
            QueuedCommand  c;
            c.Type = CT_CLEAR;
            pushCommand(c);
        }
        else
        {
            m_mutability_lock.lock();
            clearNow();
            m_mutability_lock.unlock();
        }
    }
    /*! You should implement your destructor, to handle potential memory leak from queues
     */
    virtual ~TemporarilyImmutableContainerWithHeterogeneousCommands() 
    {
    
    }
protected:
    /*! Inserts new object to container. Hidden, because container should not expose this function by default
        \param[in] o object
        \param[in] position a position
    */
    virtual void insert(const _AddedObjectType & o, size_t position)
    {
        if (m_lock_changes)
        {
            QueuedCommand  c;
            c.Type = CT_INSERT;
            c.Added = o;
            c.Position = position;
            pushCommand(c);
        }
        else
        {
            m_mutability_lock.lock();
            insertNow(o, position);
            m_mutability_lock.unlock();
        }
    }

     /*! A described command type for queue of commands
      */
     enum CommandType
     {
        CT_ADD,
        CT_REMOVE,
        CT_CLEAR,
        CT_INSERT
     };
     /*! A command, which should be executed inside of queue
      */
     struct QueuedCommand
     {
         CommandType Type;              //!< An action, which should be performed
         _AddedObjectType  Added;      //!< An object, which is being added
         _RemovedObjectType Removed;    //!< An object, which is being removed
         size_t Position;               //!< A position for inserting object into queue
         /*! By default command is left uninitialized
          */
         inline QueuedCommand() : Type(CT_ADD), Position(0) //-V730
         {

         }
     };
     /*! A queued commands container
      */
     sad::Vector<QueuedCommand> m_command_queue;
     /*! A lock, for adding a commands into lock
      */
     ::sad::Mutex                  m_command_queue_lock;
     /*! A container changing lock
      */
     ::sad::Mutex                 m_mutability_lock;
     /*! A flag, whether actions is locked
      */
     bool                       m_lock_changes;
     /*! Pushes new command to queue with specified type and object
         \param[in] c queued command
      */
     void pushCommand(const QueuedCommand & c)
     {
        m_command_queue_lock.lock();
        m_command_queue << c;
        m_command_queue_lock.unlock();
     }
     /*! Locks a changes inside of container
      */
     inline void lockChanges() { m_lock_changes = true; }
     /*! Unlocks a changes inside  of container
      */
     inline void unlockChanges() { m_lock_changes = false; }
     /*! Tests, whether container is locked
         \return whether container is locked
      */
     inline bool containerLocked() const { return m_lock_changes; }
     /*! Immediately adds an object to container
          \param[in] o object
      */
     virtual void addNow(_AddedObjectType o) = 0;
     /*! Immediately removes an object from container
          \param[in] o object
      */
     virtual void removeNow(_RemovedObjectType o) = 0;
     /*! Immediately inserts an object to container.
         This function is hidden, since it should be used in some advanced containers
         \param[in] o object
         \param[in] position a position for object
      */
     virtual void insertNow(_AddedObjectType o, size_t position)
     {
     
     }
     /*! Immediately clears a container
      */
     virtual void clearNow() = 0;
     /*! Performs all queued actions
      */
     virtual void performQueuedActions()
     {
        m_command_queue_lock.lock();
        for(size_t i = 0; i < m_command_queue.count(); i++)
        {
            QueuedCommand & c = m_command_queue[i];
            switch(c.Type)
            {
                case CT_ADD : addNow(c.Added); break;
                case CT_REMOVE: removeNow(c.Removed); break;
                case CT_CLEAR:  clearNow(); break;
                case CT_INSERT: insertNow(c.Added, c.Position); break;
                default: break;
            };
        }
        m_command_queue.clear();
        m_command_queue_lock.unlock();
     }
};

/*! A common temporarily immutable container, where added and removed objects have the same type
 */
template< typename _Object >
class TemporarilyImmutableContainer
: public  TemporarilyImmutableContainerWithHeterogeneousCommands<_Object*, _Object *>
{
public:
    /*! Default container is mutable
     */
    inline TemporarilyImmutableContainer() 
    : TemporarilyImmutableContainerWithHeterogeneousCommands<_Object*, _Object *>()
    {

    }	   
    /*! Re-implement this, to handle potential memory leaks in event queue
     */
    virtual ~TemporarilyImmutableContainer() 
    {
    
    }
};

}
