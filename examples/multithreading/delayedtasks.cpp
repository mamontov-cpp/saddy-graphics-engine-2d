#include "delayedtasks.h"

DelayedTasks::DelayedTasks() : m_paused(false)
{

}

void DelayedTasks::clear()
{
    m_tasks.clear();
}

void DelayedTasks::add(const DelayedTask& task)
{
    m_tasks << task;
    if (m_paused)
    {
        m_tasks[m_tasks.size() - 1].pause();
    }
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
    m_paused = true;
    for (size_t i = 0; i < m_tasks.size(); i++)
    {
        m_tasks[i].pause();
    }
}

void DelayedTasks::resume()
{
    m_paused = false;
    for (size_t i = 0; i < m_tasks.size(); i++)
    {
        m_tasks[i].resume();
    }
}
