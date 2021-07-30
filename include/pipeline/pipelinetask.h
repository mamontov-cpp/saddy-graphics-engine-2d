/*! \file pipelinetask.h
    

    Describes a task, which will be performed once, and then removed from process
 */
#pragma once
#include "pipelinestep.h"
#include "pipelinedelegate.h"

namespace sad
{

namespace pipeline
{

/*! Defines a basic pipeline task, which is being performed once and than removed from pipeline
 */
class AbstractTask: public sad::pipeline::Step
{
public:
    inline AbstractTask() : sad::pipeline::Step()
    {
    }
    /*! An abstract process should never be destroyed after processing
        \return false
     */
    virtual bool shouldBeDestroyedAfterProcessing() override;
};

/*! A task could be either a function call or method call, or chained method call, which is 
    being performed once and then removed from pipeline
 */
class Task: public sad::pipeline::AbstractTask
{
public:
    /*! Creates process for specified callable
        \param[in] f first callable object
     */
    template<
        typename _Callable
    >
    Task(_Callable f)
    {
        m_delegate = new sad::pipeline::Function<_Callable>(f);
    }
    /*! Creates process for specified method call
        \param[in] o object, which method call os performed on
        \param[in] f called method
     */
    template<
        typename _Object,
        typename _Method
    >
    Task(_Object * o, _Method f)
    {
        m_delegate = new sad::pipeline::MethodCall<_Object, _Method>(o, f);
    }
    /*! Creates process for specified chained method call
        \param[in] o object, which method call os performed on
        \param[in] f first called method
        \param[in] g second called method
     */
    template<
        typename _Object,
        typename _FirstMethod,
        typename _SecondMethod
    >
    Task(_Object * o, _FirstMethod f, _SecondMethod g)
    {
        m_delegate = new sad::pipeline::ComposedMethodCall<_Object, _FirstMethod, _SecondMethod>(o, f, g);
    }
    /*! Changes object for all method call. Object is casted down to method
        \param[in] o object for method call
     */
    virtual void changeObject(void * o) override;
    /*! Destroys a delegate in process
     */
    virtual ~Task() override;
protected:
    /*! Invokes a delegate inside of process
     */ 
    virtual void _process() override;
    /*!  A linked delegate for process
     */
    sad::pipeline::Delegate * m_delegate;
private:
    /*! Disable copying for class
        \param[in] o other process
     */
    Task(const Task & o);
    /*! Disable copying for class
        \param[in] o other process
        \return self-reference
     */
    Task & operator=(const Task & o);
};

}

}
