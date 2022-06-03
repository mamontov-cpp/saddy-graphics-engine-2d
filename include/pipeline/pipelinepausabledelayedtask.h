/*! \file pipelinepausabledelayedtask.h
 *
 *
 *  Describes a delayed task, task that can be paused, if time delay is not reached
 */
#pragma once
#include "pipelinetask.h"
#include <functional>
#include "../pausabletimer.h"

namespace sad
{

namespace pipeline
{

class PausableDelayedTaskList;
/*! A delayed task, that will be performed after certain time delay, but before it is executed, in can be paused
 */
class PausableDelayedTask : public sad::pipeline::AbstractTask
{
public:
    /*! Disable copying for class
        \param[in] o other process
     */
    PausableDelayedTask(const sad::pipeline::PausableDelayedTask & o) = delete;
    /*! Disable copying for class
        \param[in] o other process
        \return self-reference
     */
    sad::pipeline::PausableDelayedTask & operator=(const sad::pipeline::PausableDelayedTask & o) = delete;
    /*! Default constructor with empty delegate
        \param[in] interval an interval for task
    */
    inline PausableDelayedTask(double interval) : m_delegate(nullptr), m_interval(interval), m_performed(false), m_parent(nullptr)
    {
        m_timer.start();
    }
    /*! Creates process for specified callable
        \param[in] f first callable object
        \param[in] interval an interval, after which function will be called
     */
    template<
        typename _Callable
    >
    PausableDelayedTask(_Callable f, double interval) : m_interval(interval), m_performed(false), m_parent(nullptr)
    {
        m_delegate = new sad::pipeline::Function<_Callable>(f);
        m_timer.start();
    }
    /*! Creates process for specified method call
        \param[in] o object, which method call os performed on
        \param[in] f called method
        \param[in] interval an interval, after which function will be called
     */
    template<
        typename _Object,
        typename _Method
    >
    PausableDelayedTask(_Object * o, _Method f, double interval) : m_interval(interval), m_performed(false), m_parent(nullptr)
    {
        m_delegate = new sad::pipeline::MethodCall<_Object, _Method>(o, f);
        m_timer.start();
    }
    /*! Creates process for specified chained method call
        \param[in] o object, which method call os performed on
        \param[in] f first called method
        \param[in] g second called method
        \param[in] interval an interval, after which function will be called
     */
    template<
        typename _Object,
        typename _FirstMethod,
        typename _SecondMethod
    >
    PausableDelayedTask(_Object * o, _FirstMethod f, _SecondMethod g, double interval) : m_interval(interval), m_performed(false), m_parent(nullptr)
    {
        m_delegate = new sad::pipeline::ComposedMethodCall<_Object, _FirstMethod, _SecondMethod>(o, f, g);
        m_timer.start();
    }
    /*! Changes object for all method call. Object is casted down to method
        \param[in] o object for method call
     */
    virtual void changeObject(void * o) override;
    /*! An abstract process should never be destroyed after processing
        \return false
     */
    virtual bool shouldBeDestroyedAfterProcessing() override;
    /*! Destroys a delegate in process
     */
    virtual ~PausableDelayedTask() override;
    /*! Pauses task's timer
     */
    void pause();
    /*! Resumes task's timer
     */
    void resume();
    /*! Sets parent list
     *  \param[in] list a parent data
     */
    void setParent(sad::pipeline::PausableDelayedTaskList* list);
protected:
    /*! Invokes a delegate inside of process
     */
    virtual void _process() override;
    /*!  A linked delegate for process
     */
    sad::pipeline::Delegate* m_delegate;
    /*! An interval, after which task will be performed in milliseconds
     */
    double m_interval;
    /*! Whether task is performed
     */
    bool m_performed;
    /*! A timer
     */
    sad::PausableTimer m_timer;
    /*! A parent task queue
     */
    sad::pipeline::PausableDelayedTaskList* m_parent;
};

}

}