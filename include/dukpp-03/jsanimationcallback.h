/*! \file jsanimationcallback.h
     
    A callback for invoking when animation is started or finished
 */
#pragma once
#include "../animations/animationscallback.h"
#include "context.h"

namespace sad
{

namespace dukpp03
{


/*! A js animation handler, used to place invokable functions in groups or instances
 */
class JSAnimationCallback : public sad::animations::Callback
{
public:
    /*! Makes new handler
        \param[in] ctx context
        \param[in] f function
    */
    JSAnimationCallback(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f);
    /*! Invokes a handler
     */
    virtual void invoke();
    /*! Frees a handler
     */
    virtual ~JSAnimationCallback();
protected:
    /*! A context to be called with handler
     */
    sad::dukpp03::Context* m_ctx;
    /*! A function to be called
     */
    sad::dukpp03::CompiledFunction m_function;
};

}

}