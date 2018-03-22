/*! \file animationsanimations.h
    

    Defines an animations as pipeline step
 */
#pragma once

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
    /*! Creates new empty animations list
     */
    Animations();
    /*! Removes links to processes, destroying object
     */
    virtual ~Animations();

    /*! Adds new process to container
        \param[in] o an animation process
     */
    void add(sad::animations::Process * o);
    /*! Removes process from container
        \param[in] o an animation process
     */
    void remove(sad::animations::Process* o);
    /*! Removes a process by name
        \param[in] name a name of process
     */
    void removeByName(const sad::String& name);
    /*! Removes a process by name and type
        \param[in] type a type of object
        \param[in] name a name of process
     */
    void removeByNameAndType(const sad::String& type, const sad::String& name);
    /*! Removes a process by major id
        \param[in] major_id a major id for process
     */
    void removeByMajorId(unsigned long long major_id);
    /*! Clears a container, removing all processes
     */
    void clear();
    /*! Inserts process at specified position
        \param[in] position a position for process
        \param[in] o process
     */
    void insertAt(unsigned int position, sad::animations::Process* o);
    /*! Removes process at specified position
        \param[in] position  a position for removal of object
     */
    void removeAt(unsigned int position);
    /*! Returs amount of processes of list
        \return amount of processes in list
     */
    unsigned int count() const;
    /*! Returns list, copying all of stored animations
        \return list
     */
    sad::Vector<sad::animations::Process*> list() const;
    /*! Returns position of process in container
        \param[in] o object
        \return a position of process
     */
    int find(sad::animations::Process* o);

    /*! Inserts one process before another
        \param[in] before a pivot process
        \param[in] o another process
     */
    void insertBefore(sad::animations::Process* before, sad::animations::Process* o);
    /*! Inserts one process before another
        \param[in] name a name of another process
        \param[in] o another process
    */
    void insertBefore(const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process before another
        \param[in] type a type of process
        \param[in] name a name of another process
        \param[in] o another process
    */
    void insertBefore(const sad::String& type, const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process before another
        \param[in] major_id a major id of pivot process
        \param[in] o another process
    */
    void insertBefore(unsigned long long major_id, sad::animations::Process* o);
    /*! Inserts one process before another animation related to object
        \param[in] before a pivot object
        \param[in] o another process
    */
    void insertBeforeInstanceWithObject(sad::db::Object* before, sad::animations::Process* o);
    /*! Inserts one process before another animation related to object
        \param[in] name a name
        \param[in] o another process
    */
    void insertBeforeInstanceWithObject(const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process before another animation related to object
        \param[in] type a type
        \param[in] name a name
        \param[in] o another process
    */
    void insertBeforeInstanceWithObject(const sad::String& type, const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process before another animation related to object
        \param[in] major_id a major id
        \param[in] o another process
    */
    void insertBeforeInstanceWithObject(unsigned long long major_id, sad::animations::Process* o);
    /*! Inserts one process before another animation related to object
        \param[in] before a pivot process
        \param[in] o another process
    */
    void insertBeforeInstanceWithAnimation(sad::animations::Animation* before, sad::animations::Process* o);
    /*! Inserts one process before another animation related to object
        \param[in] name a name
        \param[in] o another process
    */
    void insertBeforeInstanceWithAnimation(const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process before another animation related to object
        \param[in] type a type
        \param[in] name a name
        \param[in] o another process
    */
    void insertBeforeInstanceWithAnimation(const sad::String& type, const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process before another animation related to object
        \param[in] major_id a major id
        \param[in] o another process
    */
    void insertBeforeInstanceWithAnimation(unsigned long long major_id, sad::animations::Process* o);

    /*! Inserts one process after another
        \param[in] after a pivot process
        \param[in] o another process
    */
    void insertAfter(sad::animations::Process* after, sad::animations::Process* o);
    /*! Inserts one process after another
        \param[in] name a name of another process
        \param[in] o another process
    */
    void insertAfter(const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process after another
        \param[in] type a type of process
        \param[in] name a name of another process
        \param[in] o another process
    */
    void insertAfter(const sad::String& type, const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process after another
        \param[in] major_id a major id of pivot process
        \param[in] o another process
    */
    void insertAfter(unsigned long long major_id, sad::animations::Process* o);
    /*! Inserts one process after another animation related to object
        \param[in] after a pivot object
        \param[in] o another process
    */
    void insertAfterInstanceWithObject(sad::db::Object* after, sad::animations::Process* o);
    /*! Inserts one process after another animation related to object
        \param[in] name a name
        \param[in] o another process
    */
    void insertAfterInstanceWithObject(const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process after another animation related to object
        \param[in] type a type
        \param[in] name a name
        \param[in] o another process
    */
    void insertAfterInstanceWithObject(const sad::String& type, const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process after another animation related to object
        \param[in] major_id a major id
        \param[in] o another process
    */
    void insertAfterInstanceWithObject(unsigned long long major_id, sad::animations::Process* o);
    /*! Inserts one process after another animation related to object
        \param[in] after a pivot process
        \param[in] o another process
    */
    void insertAfterInstanceWithAnimation(sad::animations::Animation* after, sad::animations::Process* o);
    /*! Inserts one process after another animation related to object
        \param[in] name a name
        \param[in] o another process
    */
    void insertAfterInstanceWithAnimation(const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process after another animation related to object
        \param[in] type a type
        \param[in] name a name
        \param[in] o another process
    */
    void insertAfterInstanceWithAnimation(const sad::String& type, const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process after another animation related to object
        \param[in] major_id a major id
        \param[in] o another process
    */
    void insertAfterInstanceWithAnimation(unsigned long long major_id, sad::animations::Process* o);
    /*! Swaps first process position with second process
        \param[in] first a first process
        \param[in] second a second process
     */
    void swap(sad::animations::Process* first, sad::animations::Process* second);
    /*! Swaps first process named name1 with position of name2
        \param[in] name1 first name
        \param[in] name2 second name
     */
    void swap(const sad::String& name1, const sad::String name2);
    /*! Swaps first process named name1 with type1 position of name2 with type2
        \param[in] type1 first type
        \param[in] name1 first name
        \param[in] type2 second type
        \param[in] name2 second name
    */
    void swap(const sad::String& type1, const sad::String& name1, const sad::String& type2, const sad::String name2);
    /*! Swaps first process named major_id1 with position of major_id2
        \param[in] major_id1 first major id
        \param[in] major_id2 second major id
    */
    void swap(unsigned long long major_id1, unsigned long long major_id2);

    /*! A cache, for saving object state
        \return cache for saving state
     */
    sad::animations::SavedObjectStateCache& cache();
    
    /*! Queries processes by name
        \param[in] name a name of process
     */
    sad::Vector<sad::animations::Process*> queryProcessesByName(const sad::String& name);
    /*! Queries processes by type and name
        \param[in] type a type of process
        \param[in] name a name of process
     */
    sad::Vector<sad::animations::Process*> queryProcessesByTypeAndName(const sad::String& type, const sad::String& name);
    /*! Queries processes by major id
        \param[in] major_id a major id for process
     */
    sad::Vector<sad::animations::Process*> queryProcessesByMajorId(unsigned long long major_id);
    /*! Queries processes, related to specified object
        \param[in] o specified object
        \return list of related processes
     */
    sad::Vector<sad::animations::Process*> queryProcessesRelatedToObject(sad::db::Object* o);
    /*! Queries processes, related to specified object by name
        \param[in] name a name of object
        \return list of related processes
    */
    sad::Vector<sad::animations::Process*> queryProcessesRelatedToObjectByName(const sad::String& name);
    /*! Queries processes, related to specified object by type and name
        \param[in] type a type of object
        \param[in] name a name of object
        \return list of related processes
     */
    sad::Vector<sad::animations::Process*> queryProcessesRelatedToObjectByTypeAndName(const sad::String& type, const sad::String& name);
    /*! Queries processes, related to specified object by major id
        \param[in] major_id a major id for object
        \return list of related processes
    */
    sad::Vector<sad::animations::Process*> queryProcessesRelatedToObjectByMajorId(unsigned long long major_id);
    /*! Queries processes, related to specified animation
        \param[in] o specified object
        \return list of related processes
    */
    sad::Vector<sad::animations::Process*> queryProcessesRelatedToAnimation(sad::animations::Animation* o);
    /*! Queries processes, related to specified animation by name
        \param[in] name a name of animation
        \return list of related processes
     */
    sad::Vector<sad::animations::Process*> queryProcessesRelatedToAnimationByName(const sad::String& name);
    /*! Queries processes, related to specified animation by type and name
        \param[in] type a type of animation
        \param[in] name a name of animation
        \return list of related processes
     */
    sad::Vector<sad::animations::Process*> queryProcessesRelatedToAnimationByTypeAndName(const sad::String& type, const sad::String& name);
    /*! Queries processes, related to specified animation by major id
        \param[in] major_id a major id for animation
        \return list of related processes
     */
    sad::Vector<sad::animations::Process*> queryProcessesRelatedToAnimationByMajorId(unsigned long long major_id);

    /*! Stops processes or instances, nested in group
        \param[in] o process
     */
    void stopProcess(sad::animations::Process* o);
    /*! Stops processes by their name
        \param[in] name a name of process
     */
    void stopProcessByName(const sad::String& name);
    /*! Stops processes by their name and type
        \param[in] type a type of process
        \param[in] name a name of process
     */
    void stopProcessByTypeAndName(const sad::String& type, const sad::String& name);
    /*! Stops processes by their major id
        \param[in] major_id a major id
     */
    void stopProcessByMajorId(unsigned long long major_id);
    /*! Stops processes, related to object
        \param[in] o object
     */
    void stopProcessesRelatedToObject(sad::db::Object* o);
    /*! Stops processes, related to object
        \param[in] name a name of object
     */
    void stopProcessesRelatedToObjectWithName(const sad::String& name);
    /*! Stops processes, related to object with type
        \param[in] type an object with type
        \param[in] name a name of object
     */
    void stopProcessesRelatedToObjectWithTypeAndName(const sad::String& type, const sad::String& name);
    /*! Stops processes, related to object with specified major id
        \param[in] major_id a major id of object
     */
    void stopProcessesRelatedToObjectWithMajorId(unsigned long long major_id);

    /*! Stops processes, related to object
        \param[in] o object
    */
    void stopProcessesRelatedToAnimation(sad::animations::Animation* o);
    /*! Stops processes, related to animation
        \param[in] name a name of object
    */
    void stopProcessesRelatedToAnimationWithName(const sad::String& name);
    /*! Stops processes, related to animation with type
        \param[in] type an object with type
        \param[in] name a name of object
    */
    void stopProcessesRelatedToAnimationWithTypeAndName(const sad::String& type, const sad::String& name);
    /*! Stops processes, related to animation with specified major id
        \param[in] major_id a major id of object
    */
    void stopProcessesRelatedToAnimationWithMajorId(unsigned long long major_id);

    /*! Count processes, related for passed by pointer
        \param[in] o process
        \return amount of processes
     */
    size_t countProcesses(sad::animations::Process* o);
    /*! Count processes, related to one with name
        \param[in] name a name of process
        \return amount of processes
     */
    size_t countProcessesByName(const sad::String& name);
    /*! Count processes, related to one with type and name
        \param[in] type a type of process
        \param[in] name a name of process
        \return amount of processes
    */
    size_t countProcessesByTypeAndName(const sad::String& type, const sad::String& name);
    /*! Count processes, related to one with major id
        \param[in] major_id a major id of process
        \return amount of processes
    */
    size_t countProcessesByMajorId(unsigned long long major_id);
    /*! Count processes, related to object
        \param[in] o process
        \return amount of processes
     */
    size_t countProcessesRelatedToObject(sad::db::Object* o);
    /*! Count processes, related to object with name
        \param[in] name a name of process
        \return amount of processes
     */
    size_t countProcessesRelatedToObjectByName(const sad::String& name);
    /*! Count processes, related to object with type and name
        \param[in] type a type of process
        \param[in] name a name of process
        \return amount of processes
     */
    size_t countProcessesRelatedToObjectByTypeAndName(const sad::String& type, const sad::String& name);
    /*! Count processes, related to object with major id
        \param[in] major_id a major id of process
        \return amount of processes
     */
    size_t countProcessesRelatedToObjectByMajorId(unsigned long long major_id);
    /*! Count processes, related to animation
        \param[in] o process
        \return amount of processes
     */
    size_t countProcessesRelatedToAnimation(sad::animations::Animation* o);
    /*! Count processes, related to animation with name
        \param[in] name a name of process
        \return amount of processes
     */
    size_t countProcessesRelatedToAnimationByName(const sad::String& name);
    /*! Count processes, related to animation with type and name
        \param[in] type a type of process
        \param[in] name a name of process
        \return amount of processes
     */
    size_t countProcessesRelatedToAnimationByTypeAndName(const sad::String& type, const sad::String& name);
    /*! Count processes, related to animation with major id
        \param[in] major_id a major id of process
        \return amount of processes
     */
    size_t countProcessesRelatedToAnimationByMajorId(unsigned long long major_id);

    /*! Returns, whether there are  processes, related for passed by pointer
        \param[in] o process
        \return whether there are reated processes
     */
    bool hasProcesses(sad::animations::Process* o);
    /*! Returns, whether there are  processes, related to one with name
        \param[in] name a name of process
        \return whether there are reated processes
     */
    bool hasProcessesByName(const sad::String& name);
    /*! Returns, whether there are  processes, related to one with type and name
        \param[in] type a type of process
        \param[in] name a name of process
        \return whether there are reated processes
     */
    bool hasProcessesByTypeAndName(const sad::String& type, const sad::String& name);
    /*! Returns, whether there are  processes, related to one with major id
        \param[in] major_id a major id of process
        \return whether there are reated processes
     */
    bool hasProcessesByMajorId(unsigned long long major_id);
    /*! Returns, whether there are  processes, related to object
        \param[in] o process
        \return whether there are reated processes
     */
    bool hasProcessesRelatedToObject(sad::db::Object* o);
    /*! Returns, whether there are  processes, related to object with name
        \param[in] name a name of process
        \return whether there are reated processes
     */
    bool hasProcessesRelatedToObjectByName(const sad::String& name);
    /*! Returns, whether there are  processes, related to object with type and name
        \param[in] type a type of process
        \param[in] name a name of process
        \return whether there are reated processes
     */
    bool hasProcessesRelatedToObjectByTypeAndName(const sad::String& type, const sad::String& name);
    /*! Returns, whether there are  processes, related to object with major id
        \param[in] major_id a major id of process
        \return whether there are reated processes
     */
    bool hasProcessesRelatedToObjectByMajorId(unsigned long long major_id);
    /*! Returns, whether there are  processes, related to animation
        \param[in] o process
        \return whether there are reated processes
     */
    bool hasProcessesRelatedToAnimation(sad::animations::Animation* o);
    /*! Returns, whether there are  processes, related to animation with name
        \param[in] name a name of process
        \return whether there are reated processes
     */
    bool hasProcessesRelatedToAnimationByName(const sad::String& name);
    /*! Returns, whether there are  processes, related to animation with type and name
        \param[in] type a type of process
        \param[in] name a name of process
        \return whether there are reated processes
     */
    bool hasProcessesRelatedToAnimationByTypeAndName(const sad::String& type, const sad::String& name);
    /*! Returns, whether there are  processes, related to animation with major id
        \param[in] major_id a major id of process
        \return whether there are reated processes
     */
    bool hasProcessesRelatedToAnimationByMajorId(unsigned long long major_id);

    /*! Notifies, that process should be removed. This is inner function, DO NOT CALL IT
        \param[in] process a process
     */
    void notifyProcessRemoval(sad::animations::Process* process);
protected:
    /*! Throws exception, as this object is non-copyable
        \param[in] o object
     */
    Animations(const sad::animations::Animations& o);
    /*! Throws exception, as this object is non-copyable
        \param[in] o object
        \return self-reference
     */
    Animations& operator=(const sad::animations::Animations& o);
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
    /*! Inserts an animation process to list at position
        \param[in] position a position of object
        \param[in] o an object
     */
    void insertAtNow(unsigned int position, sad::animations::Process* o);
    /*! Removes an animation process at specified position
        \param[in] position a position
     */
    void removeAtNow(unsigned int position);
    /*! Returns position of first matched process
        \param[in] f a matching function
        \return a position or -1 if not found
     */
    int findFirst(const std::function<bool(sad::animations::Process*)>& f);
    /*! Returns position of last matched process
        \param[in] f a matching function
        \return a position or -1 if not found
    */
    int findLast(const std::function<bool(sad::animations::Process*)>& f);
    /*! Inserts a process before first matched
        \param[in] f matching function
        \param[in] o a process
     */
    void insertBeforeFirstMatched(const std::function<bool(sad::animations::Process*)>& f, sad::animations::Process* o);
    /*! Inserts a process after last matched
        \param[in] f matching function
         \param[in] o a process
     */
    void insertAfterLastMatched(const std::function<bool(sad::animations::Process*)>& f, sad::animations::Process* o);
    /*! Inserts one process before another
        \param[in] before a pivot process
        \param[in] o another process
    */
    void insertBeforeNow(sad::animations::Process* before, sad::animations::Process* o);
    /*! Inserts one process before another
       \param[in] name a name of another process
       \param[in] o another process
    */
    void insertBeforeNow(const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process before another
       \param[in] type a type of process
       \param[in] name a name of another process
       \param[in] o another process
    */
    void insertBeforeNow(const sad::String& type, const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process before another
        \param[in] major_id a major id of pivot process
        \param[in] o another process
    */
    void insertBeforeNow(unsigned long long major_id, sad::animations::Process* o);


    /*! Inserts one process after another
        \param[in] after a pivot process
        \param[in] o another process
    */
    void insertAfterNow(sad::animations::Process* after, sad::animations::Process* o);
    /*! Inserts one process after another
        \param[in] name a name of another process
        \param[in] o another process
    */
    void insertAfterNow(const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process after another
        \param[in] type a type of process
        \param[in] name a name of another process
        \param[in] o another process
    */
    void insertAfterNow(const sad::String& type, const sad::String& name, sad::animations::Process* o);
    /*! Inserts one process after another
        \param[in] major_id a major id of pivot process
        \param[in] o another process
    */
    void insertAfterNow(unsigned long long major_id, sad::animations::Process* o);

    /*! Pushes new command to queue
        \param[in] f a function
     */
    void pushCommand(const std::function<void()>& f);
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
    /*! Performs an action or queues it
        \param[in] f function
     */
    void performOrQueue(const std::function<void()>& f);
    /*! Querys processes
        \param[in] f a functional object for predicate
    */
    sad::Vector<sad::animations::Process*> queryProcesses(const std::function<bool(sad::animations::Process*)>& f);
    /*! Querys processes, related to matched object
        \param[in] f a functional object for predicate
     */
    sad::Vector<sad::animations::Process*> queryProcessesRelatedToMatchedObject(const std::function<bool(sad::db::Object*)>& f);
    /*! Querys processes, related to matched animation
        \param[in] f a functional object for predicate
    */
    sad::Vector<sad::animations::Process*> queryProcessesRelatedToMatchedAnimation(const std::function<bool(sad::animations::Animation*)>& f);

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
    sad::Vector<std::function<void()> > m_command_queue;
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
