/*! \file pipelineprocess.h
    

    Describes a process, which should be performed every time inside of pipeline loop
 */
#pragma once
#include "pipelinestep.h"
#include "pipelinedelegate.h"

namespace sad
{

namespace pipeline
{

/*! Defines a basic pipeline process, which should be performed every time
    inside of pipeline loop
 */
class AbstractProcess: public sad::pipeline::Step
{
public:
    inline AbstractProcess() : sad::pipeline::Step()
    {
    }
    /*! An abstract process should never be destroyed after processing
        \return false
     */
    virtual bool shouldBeDestroyedAfterProcessing() override;
};


class Process: public sad::pipeline::AbstractProcess
{
public:
    /*! Creates process for specified callable
        \param[in] f first callable object
     */
    template<
        typename _Callable
    >
    Process(_Callable f)
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
    Process(_Object * o, _Method f)
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
    Process(_Object * o, _FirstMethod f, _SecondMethod g)
    {
        m_delegate = new sad::pipeline::ComposedMethodCall<_Object, _FirstMethod, _SecondMethod>(o, f, g);
    }

    /*! Destroys a delegate in process
     */
    virtual ~Process() override;
    /*! Changes object for all method call. Object is casted down to method
        \param[in] o object for method call
     */
    virtual void changeObject(void * o) override;
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
    Process(const Process & o);
    /*! Disable copying for class
        \param[in] o other process
        \return self-reference
     */
    Process & operator=(const Process & o);
};

}

}
