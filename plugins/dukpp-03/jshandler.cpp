#include "dukpp-03/jshandler.h"

sad::dukpp03::JSHandler::JSHandler(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f)
: m_ctx(ctx), m_function(f) 
{
    m_ctx->addRef();
}

sad::dukpp03::JSHandler::~JSHandler()
{
    m_ctx->delRef();    
}
