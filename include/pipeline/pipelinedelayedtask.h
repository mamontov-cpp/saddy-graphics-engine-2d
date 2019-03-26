/*! \file pipelinetask.h


Describes a task, which will be performed once, and then removed from process
*/
#pragma once
#include "pipelinetask.h"
#include "../timer.h"

namespace sad
{

namespace pipeline
{

/*! A delayed task, that will be performed after certain time delay
 */
class DelayedTask : public sad::pipeline::AbstractTask
{
public:
	/*! Default constructor with empty delegate
		\param[in] interval an interval for task
	*/
	DelayedTask(double interval) : m_interval(interval), m_delegate(NULL), m_performed(false)
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
    DelayedTask(_Callable f, double interval) : m_interval(interval), m_performed(false)
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
    DelayedTask(_Object * o, _Method f, double interval) : m_interval(interval), m_performed(false)
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
    DelayedTask(_Object * o, _FirstMethod f, _SecondMethod g, double interval) : m_interval(interval), m_performed(false)
    {
        m_delegate = new sad::pipeline::ComposedMethodCall<_Object, _FirstMethod, _SecondMethod>(o, f, g);
        m_timer.start();
    }
    /*! Changes object for all method call. Object is casted down to method
        \param[in] o object for method call
     */
    virtual void changeObject(void * o);
    /*! An abstract process should never be destroyed after processing
        \return false
     */
    virtual bool shouldBeDestroyedAfterProcessing();
    /*! Destroys a delegate in process
     */
    virtual ~DelayedTask();
protected:
    /*! Invokes a delegate inside of process
     */
    virtual void _process();
    /*!  A linked delegate for process
     */
    sad::pipeline::Delegate * m_delegate;
    /*! An interval, after which task will be performed in milliseconds
     */
    double m_interval;
    /*! Whether task is performed
     */
    bool m_performed;
    /*! A timer
     */
    sad::Timer m_timer;
private:
    /*! Disable copying for class
       \param[in] o other process
     */
    DelayedTask(const DelayedTask & o);
    /*! Disable copying for class
        \param[in] o other process
        \return self-reference
     */
    DelayedTask & operator=(const DelayedTask & o);
};

}

}