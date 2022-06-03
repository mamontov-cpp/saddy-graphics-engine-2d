#include "pipeline/pipelinepausabledelayedtasklist.h"


void sad::pipeline::PausableDelayedTaskList::pause()
{
    for(auto* item : m_list)
    {
        item->pause();
    }
}

void sad::pipeline::PausableDelayedTaskList::resume()
{
    for (auto* item : m_list)
    {
        item->resume();
    }
}

void sad::pipeline::PausableDelayedTaskList::remove(sad::pipeline::PausableDelayedTask* task)
{
    m_list.removeAll(task);
}

size_t sad::pipeline::PausableDelayedTaskList::count() const
{
    return m_list.size();
}