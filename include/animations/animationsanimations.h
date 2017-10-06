/*! \file animationsanimations.h
    

    Defines an animations as pipeline step
 */
#pragma once

#include "../temporarilyimmutablecontainer.h"
#include "../sadvector.h"

#include "../pipeline/pipelineprocess.h"

#include "../sadmutex.h"
#include "../sadvector.h"

#include "animationsprocess.h"
#include "animationssavedobjectstatecache.h"


namespace sad
{
    
namespace animations
{

/*! An animations, as a pipeline rendering step
 */
class Animations: public sad::pipeline::AbstractProcess
{
public:
    /*! A queued command for storing all of data
     */
    struct QueueCommand
    {
        /*! A command type for storing commands for animations
         */
        enum CommandType
        {
            CT_ADD,     //!< This command adds new animation process to pipeline
            CT_REMOVE,  //!< This command removes animation process from pipeline
            CT_CLEAR,   //!< This command clears pipeline

            CT_INSERT_AT,     //!< This command insert animation at specified place
            CT_REMOVE_AT,     //!< This command removes animation at specified place
            CT_INSERT_BEFORE, //!< This command inserts process before specified
            CT_INSERT_AFTER,  //!< This command inserts process after specified

            CT_SWAP           //!< This command swaps two command
        };

        CommandType Type;
        sad::animations::Process* Process; //!< A process to be handled in command
        size_t Position1; //!< A position for insertions or removal
        size_t Position2; //!< A second position for swap
    };

    /*! Creates new empty animations list
     */
    Animations();
    /*! Removes links to processes, destroying object
     */
    virtual ~Animations();

    /*! Adds new process to container
        \param[in] o an animation process
     */
    virtual void add(sad::animations::Process * o);
    /*! Removes process from container
        \param[in] o an animation process
     */
    virtual void remove(sad::animations::Process* o);
    /*! Clears a container, removing all processes
     */
    virtual void clear();

    /*! A cache, for saving object state
        \return cache for saving state
     */
    sad::animations::SavedObjectStateCache& cache();
    /*! Queries processes, relates to specified object
        \param[in] o specified object
        \return list of related processes
     */
    sad::Vector<sad::animations::Process*> queryProcessesRelatedToObject(sad::db::Object* o);
    /*! Stops processes, related to object
        \param[in] o object
     */
    void stopProcessesRelatedToObject(sad::db::Object* o);
protected:
    /*! Runs every process
     */ 
    virtual void _process();
    /*! Adds an animation process to list
        \param[in] o object
     */
    void addNow(sad::animations::Process* o);
    /*! Removes an animation process from a list
        \param[in] o process
     */
    void removeNow(sad::animations::Process* o);
    /*! Clears an animation list
     */
    void clearNow();
    /*! Pushes new command to queue
        \param[in] c queued command
     */
    void pushCommand(const QueueCommand & c);
    /*! Locks a changes inside an inner container of animations
     */
    void lockChanges();
    /*! Unlocks a changes inside  of container
     */
    void unlockChanges();
    /*! Returns a value, indicating, whether container is locked
        \return  a value, indicating, whether container is locked
     */
    bool containerLocked();
    /*! Performs all queued actions, clearing command queue
     */
    void performQueuedActions();

    /*! A list of animations processes
     */
    sad::Vector<sad::animations::Process*> m_list;
    /*! A cache, for saving an animations
     */
    sad::animations::SavedObjectStateCache m_cache;
    /*! A lock for locking operations on container
     */
    sad::Mutex m_lock;
    /*! A queued commands container
     */
    sad::Vector<QueueCommand> m_command_queue;
    /*! A lock, for adding a commands into lock
     */
    sad::Mutex                  m_command_queue_lock;
    /*! A flag, whether actions is locked
     */
    bool                       m_lock_changes;
    /*! A lock for protected m_lock_changed flag
     */
    sad::Mutex                 m_lock_changes_lock;
};

}

}
