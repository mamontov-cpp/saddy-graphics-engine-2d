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
    /*! Default handler
     */
    JSAnimationCallback();
    /*! Copies a callback
        \param[in] cb other callback
     */
    JSAnimationCallback(const sad::dukpp03::JSAnimationCallback& cb);
    /*! Makes new handler
        \param[in] ctx context
        \param[in] f function
    */
    JSAnimationCallback(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f);
    /*! Copies a callback
        \param[in] cb a callback
        \return self-reference
     */
    sad::dukpp03::JSAnimationCallback& operator=(const sad::dukpp03::JSAnimationCallback& cb);
    /*! Invokes a handler
     */
    virtual void invoke();
    /*! Clones a callback
        \return copy of callback
     */
    virtual sad::animations::Callback* clone() const;
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