/*! \file tasklock.h
 *
 *  A lock for tasks, that used to synchronize pipeline tasks with transition in game
 */
#pragma once
#include <sadmutex.h>

namespace threads
{

class TaskLock
{
public:
    /*! Defines default task lock
     */
    TaskLock();
    /*! Acquires lock, incrementing amount of tasks
     */
    void acquire();
    /*! Releases lock, decrementing amount of tasks
     */
    void release();
    /*! Waits for all of tasks
     */
    void waitForTasks();
private:
    /*! Amount of running tasks in pipeline at the moment
     */
    int m_running_tasks;
    /*! A lock for running tasks counter
     */
    sad::Mutex m_running_tasks_lock;
};

}
