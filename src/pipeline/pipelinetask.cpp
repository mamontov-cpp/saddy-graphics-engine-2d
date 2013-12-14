#include "pipeline/pipelinetask.h"

bool sad::pipeline::AbstractTask::shouldBeDestroyedAfterProcessing()
{
	return true;
}

sad::pipeline::Task::~Task()
{
	delete m_delegate;
}

void sad::pipeline::Task::_process()
{
	m_delegate->call();
}
