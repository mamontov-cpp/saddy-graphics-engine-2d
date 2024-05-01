#include "pipeline/pipelinepausabledelayedtask.h"
#include "pipeline/pipelinepausabledelayedtasklist.h"
#include "opticksupport.h"

void sad::pipeline::PausableDelayedTask::changeObject(void * o)
{
    PROFILER_EVENT;
    m_delegate->changeObject(o);
}

bool sad::pipeline::PausableDelayedTask::shouldBeDestroyedAfterProcessing()
{
    PROFILER_EVENT;
    return m_performed;
}

sad::pipeline::PausableDelayedTask::~PausableDelayedTask()
{
    PROFILER_EVENT;
    delete m_delegate;
}

void sad::pipeline::PausableDelayedTask::_process()
{
    PROFILER_EVENT;
    if (!m_performed)
    {
        if (m_timer.elapsed() >= m_interval)
        {
            if (m_delegate)
            {
                m_delegate->call();
            }
            m_performed = true;
            if (m_parent)
            {
                m_parent->remove(this);
            }
        }
    }
}

void sad::pipeline::PausableDelayedTask::pause()
{
    PROFILER_EVENT;
    m_timer.pause();
}

void sad::pipeline::PausableDelayedTask::resume()
{
    PROFILER_EVENT;
    m_timer.resume();
}


void sad::pipeline::PausableDelayedTask::setParent(sad::pipeline::PausableDelayedTaskList* list)
{
    PROFILER_EVENT;
    m_parent = list;
}
