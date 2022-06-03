#include "pipeline/pipelinepausabledelayedtask.h"
#include "pipeline/pipelinepausabledelayedtasklist.h"

void sad::pipeline::PausableDelayedTask::changeObject(void * o)
{
    m_delegate->changeObject(o);
}

bool sad::pipeline::PausableDelayedTask::shouldBeDestroyedAfterProcessing()
{
    return m_performed;
}

sad::pipeline::PausableDelayedTask::~PausableDelayedTask()
{
    delete m_delegate;
}

void sad::pipeline::PausableDelayedTask::_process()
{
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
    m_timer.pause();
}

void sad::pipeline::PausableDelayedTask::resume()
{
    m_timer.resume();
}


void sad::pipeline::PausableDelayedTask::setParent(sad::pipeline::PausableDelayedTaskList* list)
{
    m_parent = list;
}