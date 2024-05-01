#include "pipeline/pipelineprocess.h"
#include "opticksupport.h"

bool sad::pipeline::AbstractProcess::shouldBeDestroyedAfterProcessing()
{
    PROFILER_EVENT;
    return false;
}

sad::pipeline::Process::~Process()
{
    PROFILER_EVENT;
    delete m_delegate;
}

void sad::pipeline::Process::_process()
{
    PROFILER_EVENT;
    m_delegate->call();
}

void sad::pipeline::Process::changeObject(void * o)
{
    PROFILER_EVENT;
    m_delegate->changeObject(o);
}
