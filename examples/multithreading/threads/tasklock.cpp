#include "tasklock.h"

#include <sadsleep.h>

threads::TaskLock::TaskLock() : m_running_tasks(0)
{

}

void threads::TaskLock::acquire()
{
    m_running_tasks_lock.lock();
    ++m_running_tasks;
    m_running_tasks_lock.unlock();
}


void threads::TaskLock::release()
{
    m_running_tasks_lock.lock();
    --m_running_tasks;
    m_running_tasks_lock.unlock();
}

void threads::TaskLock::waitForTasks()
{
    // ReSharper disable once CppInitializedValueIsAlwaysRewritten
    int amount = 0;
    do
    {
        sad::sleep(100);
        m_running_tasks_lock.lock();
        amount = m_running_tasks;
        m_running_tasks_lock.unlock();
    }
    while(amount > 0);
}
