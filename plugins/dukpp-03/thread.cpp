#include "dukpp-03/thread.h"

sad::dukpp03::Thread::Thread(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f) : sad::Thread(new sad::dukpp03::Thread::CompiledFunctionExecutor(ctx, f))
{
    
}

void sad::dukpp03::Thread::join()
{
    this->wait();
}

void sad::dukpp03::Thread::waitFor(int ms)
{
    this->wait(ms);
}

sad::dukpp03::Thread::~Thread()
{
    
}

sad::dukpp03::Thread::CompiledFunctionExecutor::CompiledFunctionExecutor(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f) : m_ctx(ctx), m_function(f)
{
    m_ctx->addRef();
}

int sad::dukpp03::Thread::CompiledFunctionExecutor::execute()
{
    m_ctx->cleanStack();
    return m_function.call(m_ctx);
}

sad::AbstractThreadExecutableFunction * sad::dukpp03::Thread::CompiledFunctionExecutor::clone() const
{
    return new sad::dukpp03::Thread::CompiledFunctionExecutor(m_ctx, m_function);
}

sad::dukpp03::Thread::CompiledFunctionExecutor::~CompiledFunctionExecutor()
{
    m_ctx->delRef();
}

DECLARE_COMMON_TYPE(sad::dukpp03::Thread)
