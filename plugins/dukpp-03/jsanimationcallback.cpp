#include "dukpp-03/jsanimationcallback.h"

#include "db/dbtypename.h"

#include <renderer.h>

DECLARE_COMMON_TYPE(sad::dukpp03::JSAnimationCallback)

sad::dukpp03::JSAnimationCallback::JSAnimationCallback() : m_ctx(NULL)
{
    
}

sad::dukpp03::JSAnimationCallback::JSAnimationCallback(const sad::dukpp03::JSAnimationCallback& cb) : m_ctx(cb.m_ctx), m_function(cb.m_function)
{
    if (m_ctx)
    {
        m_ctx->addRef();
    }
}

sad::dukpp03::JSAnimationCallback::JSAnimationCallback(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f) : m_ctx(ctx), m_function(f)
{
    if (m_ctx)
    { 
        m_ctx->addRef();
    }
}

sad::dukpp03::JSAnimationCallback& sad::dukpp03::JSAnimationCallback::operator=(const sad::dukpp03::JSAnimationCallback& cb)
{
    if (m_ctx)
    {
        m_ctx->delRef();
    }
    m_ctx = cb.m_ctx;
    m_function=  cb.m_function;
    if (m_ctx)
    {
        m_ctx->addRef();
    }
    return *this;
}

void sad::dukpp03::JSAnimationCallback::invoke()
{
    if (!m_ctx)
    {
        return;
    }
    m_function.call(m_ctx);
    ::dukpp03::Maybe<std::string>  maybe_error = m_ctx->errorOnStack(-1);
    if (maybe_error.exists())
    {
        m_ctx->renderer()->log()->critical(maybe_error.value().c_str(), __FILE__, __LINE__);
    }
    m_ctx->cleanStack();
}

sad::animations::Callback* sad::dukpp03::JSAnimationCallback::clone() const
{
    return new sad::dukpp03::JSAnimationCallback(m_ctx, m_function);
}

sad::dukpp03::JSAnimationCallback::~JSAnimationCallback()
{
    if (m_ctx)
    {
        m_ctx->delRef();
    }
}
