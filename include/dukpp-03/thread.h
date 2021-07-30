/*! \file thread.h
    
    Defines a scriptable ref-countable thread
 */
#pragma once
#include "../sadthread.h"
#include "../refcountable.h"
#include "context.h"

namespace sad
{

namespace dukpp03
{
    
class Thread: public sad::Thread, public sad::RefCountable
{
public:
    /*! Makes new thread
        \param[in] ctx context
        \param[in] f a function
     */
    Thread(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f);
    /*! Analogue of wait() call in parent class
     */
    void join();
    /*! Analogue of wait(ms) call in parent class
        \param[in] ms milliseconds
     */
    void waitFor(int ms);
    /*! Could be inherites
     */
    virtual ~Thread();
protected:
class CompiledFunctionExecutor: public sad::AbstractThreadExecutableFunction 
{
public:	
    /*! Makes new instance of object
        \param[in] ctx context
        \param[in] f a function
     */
    CompiledFunctionExecutor(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f);
    /*! Executes a function in thread
        \return code, which will be returned in thread
     */
    virtual int execute() override;
    /*! Cretes a clone of executable function
        \returns exact copy of current thread executable function
     */
    virtual sad::AbstractThreadExecutableFunction * clone() const override;
    /*! Kept for purpose of inheritance
     */
    virtual ~CompiledFunctionExecutor() override;
protected:
    /*! A context for function
     */
    sad::dukpp03::Context* m_ctx;
    /*! A compiled function to be run
     */
    sad::dukpp03::CompiledFunction m_function;
};

};

}

}