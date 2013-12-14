#include "pipeline/pipelineprocess.h"

bool sad::pipeline::AbstractProcess::shouldBeDestroyedAfterProcessing()
{
	return false;
}

sad::pipeline::Process::~Process()
{
	delete m_delegate;
}

void sad::pipeline::Process::_process()
{
	m_delegate->call();
}
