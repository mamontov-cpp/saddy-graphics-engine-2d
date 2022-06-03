/*! \file pipelinepausabledelayedtasklist.h
 *
 *  A list of delayed tasks, that can be paused
 */
#pragma once
#include "pipelinepausabledelayedtask.h"
#include "../sadvector.h"

namespace sad
{

namespace pipeline
{

/*! A list of delayed tasks, that can be paused
 */
class PausableDelayedTaskList
{
public:
    /*! Default constructor
     */
    PausableDelayedTaskList() = default;
    /*! Creates task in list for specified callable
        \param[in] f first callable object
        \param[in] interval an interval, after which function will be called
        \return result
     */
    template<
        typename _Callable
    >
    sad::pipeline::AbstractTask* makeTask(_Callable f, double interval)
    {
        sad::pipeline::PausableDelayedTask* task = new sad::pipeline::PausableDelayedTask(f, interval);
        task->setParent(this);
        m_list.push_back(task);
        return task;
    }
    /*! Creates task in list specified method call
        \param[in] o object, which method call os performed on
        \param[in] f called method
        \param[in] interval an interval, after which function will be called
        \return result
     */
    template<
        typename _Object,
        typename _Method
    >
    sad::pipeline::AbstractTask* makeTask(_Object * o, _Method f, double interval)
    {
        sad::pipeline::PausableDelayedTask* task = new sad::pipeline::PausableDelayedTask(o, f, interval);
        task->setParent(this);
        m_list.push_back(task);
        return task;
    }
    /*! Creates task in list specified chained method call
        \param[in] o object, which method call os performed on
        \param[in] f first called method
        \param[in] g second called method
        \param[in] interval an interval, after which function will be called
        \return result
     */
    template<
        typename _Object,
        typename _FirstMethod,
        typename _SecondMethod
    >
    sad::pipeline::AbstractTask* makeTask(_Object * o, _FirstMethod f, _SecondMethod g, double interval)
    {
        sad::pipeline::PausableDelayedTask* task = new sad::pipeline::PausableDelayedTask(o, f, g, interval);
        task->setParent(this);
        m_list.push_back(task);
        return task;
    }
    /*! Pauses all tasks' timers
     */
    void pause();
    /*! Resumes all tasks' timers
     */
    void resume();
    /*! Removes task from queue
     *  \param[in] task task
     */
    void remove(sad::pipeline::PausableDelayedTask* task);
    /*! Returns amount of tasks in list
     *  \return amount of tasks in list
     */
    size_t count() const;
private:
    /*! A list of tasks
     */
    sad::Vector<sad::pipeline::PausableDelayedTask*> m_list;
};


}

}