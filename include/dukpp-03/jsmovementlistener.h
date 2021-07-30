/*! \file jsmovementlistener.h
    
    A listener for movement
 */
#pragma once
#include "../p2d/movement.h"
#include "../renderer.h"

namespace sad
{

namespace dukpp03
{

namespace internal_for_movement
{

/*! An internal push helper for overriding pushing sad::p2d::Vector
 */
template<typename _Value>
struct Push
{
    /*! Pushes value on stack
        \param[in] ctx context
        \param[in] e value
     */
    inline static void push(sad::dukpp03::BasicContext* ctx, const _Value& e)
    {
        ::dukpp03::PushValue<_Value, sad::dukpp03::BasicContext>::perform(ctx, e);
    }

};
/*! An internal push helper for overriding pushing sad::p2d::Vector
 */
template<>
struct Push<sad::p2d::Vector>
{
    /*! Pushes value on stack
        \param[in] ctx context
        \param[in] e value
     */
    inline static void push(sad::dukpp03::BasicContext* ctx, const sad::p2d::Vector& e)
    {
        ::dukpp03::PushValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(ctx, e);
    }
};


}


/*! A js handler to be used as movement listener
 */    
template<
    typename _Value
>
class JSMovementListener: public sad::p2d::AbstractMovementDeltaListener<_Value>
{
public:
    /*! Makes new listener
        \param[in] ctx context
        \param[in] f function
     */
    JSMovementListener(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f) : m_ctx(ctx), m_function(f)
    {
        ctx->addRef();
    }
    /*! Calls a handler with event
     */
    virtual void notify(const _Value& e) override
    {
        sad::dukpp03::internal_for_movement::Push<_Value>::push(m_ctx, e);
        m_function.call(m_ctx);
        const ::dukpp03::Maybe<std::string>  maybe_error = m_ctx->errorOnStack(-1);
        if (maybe_error.exists())
        {
            m_ctx->renderer()->log()->critical(maybe_error.value().c_str(), __FILE__, __LINE__);
        }
        m_ctx->cleanStack();
    }
    /*! Frees a handler
     */
    virtual ~JSMovementListener()
    {
        m_ctx->delRef();
    }
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
