#include "delayedtasks.h"

DelayedTasks::DelayedTasks()
{

}

void DelayedTasks::clear()
{
    m_tasks.clear();
}

void DelayedTasks::add(const DelayedTask& task)
{
    m_tasks << task;
}

void DelayedTasks::tryExecute()
{
    for (size_t i = 0; i < m_tasks.size(); i++)
    {
        if (m_tasks[i].tryExecute())
        {
            m_tasks.removeAt(i);
            --i;
        }
    }
}

void DelayedTasks::pause()
{
    for (size_t i = 0; i < m_tasks.size(); i++)
    {
        m_tasks[i].pause();
    }
}

void DelayedTasks::resume()
{
    for (size_t i = 0; i < m_tasks.size(); i++)
    {
        m_tasks[i].resume();
    }
}
