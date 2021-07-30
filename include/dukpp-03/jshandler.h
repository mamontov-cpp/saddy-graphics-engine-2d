/*! \file jshandler.h
    
    Defines a js handler for input
 */
#pragma once
#include "context.h"
#include "../renderer.h"

namespace sad
{
    
namespace dukpp03
{


/*! A js handler to be used in controls
 */    
class JSHandler
{
public:
    /*! Makes new handler
        \param[in] ctx context
        \param[in] f function
     */
    JSHandler(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f);
    /*! Calls a handler with event
     */
    template<
        typename _EventType
    >
    void call(const _EventType& e)
    {
        ::dukpp03::PushValue<_EventType, sad::dukpp03::BasicContext>::perform(m_ctx, e);
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
    virtual ~JSHandler();
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
