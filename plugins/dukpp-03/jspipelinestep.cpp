#include "dukpp-03/jspipelinestep.h"
#include <renderer.h>

sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_FRAME>::JSPipelineStep(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f)
: m_ctx(ctx), m_function(f)
{
    m_ctx->addRef();    
}


sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_FRAME>::~JSPipelineStep()
{
    m_ctx->delRef();
}

bool sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_FRAME>::shouldBeDestroyedAfterProcessing()
{
    return false;    
}

void sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_FRAME>::_process()
{
    m_function.call(m_ctx);
    ::dukpp03::Maybe<std::string>  maybe_error = m_ctx->errorOnStack(-1);
    if (maybe_error.exists())
    {
        m_ctx->renderer()->log()->critical(maybe_error.value().c_str(), __FILE__, __LINE__);
    }
    m_ctx->cleanStack();
}

sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_MS>::JSPipelineStep(double interval, sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f)
: m_interval(interval),  m_ctx(ctx), m_function(f)
{
    m_ctx->addRef();
    m_timer.start();
}

sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_MS>::~JSPipelineStep()
{
    m_ctx->delRef();
}

bool sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_MS>::shouldBeDestroyedAfterProcessing()
{
    return false;
}

void sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_MS>::_process()
{
    m_timer.stop();
    if (m_timer.elapsed() >= m_interval)
    {
        m_function.call(m_ctx);
        ::dukpp03::Maybe<std::string>  maybe_error = m_ctx->errorOnStack(-1);
        if (maybe_error.exists())
        {
            m_ctx->renderer()->log()->critical(maybe_error.value().c_str(), __FILE__, __LINE__);
        }
        m_ctx->cleanStack();
        m_timer.start();
    }
}



sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_ONE_SHOT>::JSPipelineStep(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f)
: m_ctx(ctx), m_function(f), m_should_be_removed(false)
{
    m_ctx->addRef();    
}


sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_ONE_SHOT>::~JSPipelineStep()
{
    m_ctx->delRef();
}

bool sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_ONE_SHOT>::shouldBeDestroyedAfterProcessing()
{
    return m_should_be_removed;    
}

void sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_ONE_SHOT>::_process()
{
    m_function.call(m_ctx);
    ::dukpp03::Maybe<std::string>  maybe_error = m_ctx->errorOnStack(-1);
    if (maybe_error.exists())
    {
        m_ctx->renderer()->log()->critical(maybe_error.value().c_str(), __FILE__, __LINE__);
    }
    m_ctx->cleanStack();
    m_should_be_removed = true;
}

