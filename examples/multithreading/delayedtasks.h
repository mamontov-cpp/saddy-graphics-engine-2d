/*! \file delayedtasks.h

     A list of delayed tasks
 */
#pragma once
#include "delayedtask.h"
#include <sadvector.h>

/*! An empty delayed tasks
 */
class DelayedTasks
{
public:
    /*! Constructs new empty list of delayed tasks
     */
    DelayedTasks();
    /*! Clears list of delayed tasks
     */
    void clear();
    /*! Adds new task into list
        \param[in] task a task
     */
    void add(const DelayedTask& task);
    /*! Tries to execute list of tasks, removing executed
     */
    void tryExecute();
    /*! Pauses execution of tasks
     */
    void pause();
    /*! Resumes execution of tasks
     */
    void resume();
private:
    /*! A list of delayed tasks
     */
    sad::Vector<DelayedTask> m_tasks;
};
