#include "dukpp-03-irrklang/jsstopcallback.h"


sad::dukpp03irrklang::JSStopCallback::JSStopCallback(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f)
: m_ctx(ctx), m_function(f) 
{
    m_ctx->addRef();
}

void sad::dukpp03irrklang::JSStopCallback::call(sad::irrklang::Sound* o)
{
    ::dukpp03::PushValue<sad::irrklang::Sound*, sad::dukpp03::Context>::perform(m_ctx, o);
    m_function.call(m_ctx);
    const ::dukpp03::Maybe<std::string>  maybe_error = m_ctx->errorOnStack(-1);
    if (maybe_error.exists())
    {
        m_ctx->renderer()->log()->critical(maybe_error.value().c_str(), __FILE__, __LINE__);
    }
    m_ctx->cleanStack();
}

sad::util::PointerCallback<sad::irrklang::Sound>* sad::dukpp03irrklang::JSStopCallback::copy()
{
    return new sad::dukpp03irrklang::JSStopCallback(m_ctx, m_function);
}

sad::dukpp03irrklang::JSStopCallback::~JSStopCallback()
{
    m_ctx->delRef();    
}
