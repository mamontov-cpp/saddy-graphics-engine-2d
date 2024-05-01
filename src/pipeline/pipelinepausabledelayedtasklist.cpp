#include "pipeline/pipelinepausabledelayedtasklist.h"
#include "opticksupport.h"


void sad::pipeline::PausableDelayedTaskList::pause()
{
    PROFILER_EVENT;
    for(auto* item : m_list)
    {
        item->pause();
    }
}

void sad::pipeline::PausableDelayedTaskList::resume()
{
    PROFILER_EVENT;
    for (auto* item : m_list)
    {
        item->resume();
    }
}

void sad::pipeline::PausableDelayedTaskList::remove(sad::pipeline::PausableDelayedTask* task)
{
    PROFILER_EVENT;
    m_list.removeAll(task);
}

void sad::pipeline::PausableDelayedTaskList::clear()
{
    PROFILER_EVENT;
    m_list.clear();
}

size_t sad::pipeline::PausableDelayedTaskList::count() const
{
    PROFILER_EVENT;
    return m_list.size();
}
