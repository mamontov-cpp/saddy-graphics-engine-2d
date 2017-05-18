#include "dukpp-03/jscollisionhandler.h"

sad::dukpp03::JSCollisionHandler::JSCollisionHandler(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f) : m_ctx(ctx), m_function(f)
{
    ctx->addRef();
}

void sad::dukpp03::JSCollisionHandler::invoke(const sad::p2d::BasicCollisionEvent & ev)
{
    ::dukpp03::PushValue<sad::p2d::BasicCollisionEvent, sad::dukpp03::BasicContext>::perform(m_ctx, ev);
    m_function.call(m_ctx);
    ::dukpp03::Maybe<std::string>  maybe_error = m_ctx->errorOnStack(-1);
    if (maybe_error.exists())
    {
        m_ctx->renderer()->log()->critical(maybe_error.value().c_str(), __FILE__, __LINE__);
    }
    m_ctx->cleanStack();
}

sad::dukpp03::JSCollisionHandler::~JSCollisionHandler()
{
    m_ctx->delRef();
}