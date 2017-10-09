#include "dukpp-03/jsanimationcallback.h"

#include <renderer.h>

sad::dukpp03::JSAnimationCallback::JSAnimationCallback(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f) : m_ctx(ctx), m_function(f)
{
    m_ctx->addRef();
}

void sad::dukpp03::JSAnimationCallback::invoke()
{
    m_function.call(m_ctx);
    ::dukpp03::Maybe<std::string>  maybe_error = m_ctx->errorOnStack(-1);
    if (maybe_error.exists())
    {
        m_ctx->renderer()->log()->critical(maybe_error.value().c_str(), __FILE__, __LINE__);
    }
    m_ctx->cleanStack();
}

sad::dukpp03::JSAnimationCallback::~JSAnimationCallback()
{

}
