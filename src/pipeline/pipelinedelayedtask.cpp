#include "pipeline/pipelinedelayedtask.h"
#include "opticksupport.h"

void sad::pipeline::DelayedTask::changeObject(void * o)
{
    PROFILER_EVENT;
    m_delegate->changeObject(o);
}

bool sad::pipeline::DelayedTask::shouldBeDestroyedAfterProcessing()
{
    PROFILER_EVENT;
    return m_performed;
}

sad::pipeline::DelayedTask::~DelayedTask()
{
    PROFILER_EVENT;
    delete m_delegate;
}

void sad::pipeline::DelayedTask::_process()
{
    PROFILER_EVENT;
    if (!m_performed) 
    {
        m_timer.stop();
        if (m_timer.elapsed() >= m_interval)
        {
            if (m_delegate)
            {
                m_delegate->call();
            }
            m_performed = true;
        }
    }
}


