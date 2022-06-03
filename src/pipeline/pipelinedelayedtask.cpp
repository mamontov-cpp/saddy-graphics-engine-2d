#include "pipeline/pipelinedelayedtask.h"

void sad::pipeline::DelayedTask::changeObject(void * o)
{
    m_delegate->changeObject(o);
}

bool sad::pipeline::DelayedTask::shouldBeDestroyedAfterProcessing()
{
    return m_performed;
}

sad::pipeline::DelayedTask::~DelayedTask()
{
    delete m_delegate;
}

void sad::pipeline::DelayedTask::_process()
{
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



