#include "dukpp-03/jspipelinedelayedtask.h"
#include <renderer.h>


sad::dukpp03::JSPipelineDelayedTask::JSPipelineDelayedTask(double interval, sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f)
: DelayedTask(interval)
{
	m_function = f;
	m_ctx = ctx;
	m_ctx->addRef();
	std::function<void()> invoke_function = [ctx, f]() -> void {
		const_cast<sad::dukpp03::CompiledFunction&>(f).call(ctx);
		::dukpp03::Maybe<std::string>  maybe_error = ctx->errorOnStack(-1);
		if (maybe_error.exists())
		{
			ctx->renderer()->log()->critical(maybe_error.value().c_str(), __FILE__, __LINE__);
		}
		ctx->cleanStack();
	};
	m_delegate = new sad::pipeline::Function<std::function<void()> >(invoke_function);
}

sad::dukpp03::JSPipelineDelayedTask::~JSPipelineDelayedTask()
{
	m_ctx->delRef();
}