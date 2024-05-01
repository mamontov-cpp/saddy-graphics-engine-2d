#include "pipeline/pipelinetask.h"
#include "opticksupport.h"

bool sad::pipeline::AbstractTask::shouldBeDestroyedAfterProcessing()
{
    PROFILER_EVENT;
    return true;
}

sad::pipeline::Task::~Task()
{
    PROFILER_EVENT;
    delete m_delegate;
}

void sad::pipeline::Task::_process()
{
    PROFILER_EVENT;
    m_delegate->call();
}

void sad::pipeline::Task::changeObject(void * o)
{
    PROFILER_EVENT;
    m_delegate->changeObject(o);
}
