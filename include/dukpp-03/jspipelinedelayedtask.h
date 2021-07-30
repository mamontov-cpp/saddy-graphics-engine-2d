/*! \file jspipelinedelayedtask.h

A simple pipeline delayed task for performing compiled function calls
*/
#pragma once

#include "../pipeline/pipelinedelayedtask.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "../timer.h"
#include "context.h"

namespace sad
{

namespace dukpp03
{

	class JSPipelineDelayedTask : public sad::pipeline::DelayedTask
	{
	public:
		/*! Makes new handler
		\param[in] interval an interval for launch
		\param[in] ctx context
		\param[in] f function
		*/
		JSPipelineDelayedTask(double interval, sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f);
		/*! Destroys a task
		*/
		virtual ~JSPipelineDelayedTask();

	protected:
		/*! A context
		*/
		sad::dukpp03::Context* m_ctx;
		/*! A function
		*/
		sad::dukpp03::CompiledFunction m_function;

	};

}

}