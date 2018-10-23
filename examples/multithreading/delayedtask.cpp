#include <utility>

#include "delayedtask.h"


DelayedTask::DelayedTask(double time, std::function<void()>  f)
: m_fn(std::move(f)),
m_elapsed_time(0),
m_time(time),
m_is_paused(false)
{
    m_timer.start();
}

bool DelayedTask::tryExecute()
{
     if (m_is_paused)
     {
         return false;
     }
     m_timer.stop();
     if (m_elapsed_time + m_timer.elapsed() >= m_time)
     {
         m_fn();
         return true;
     }
     return false;
}

void DelayedTask::pause()
{
    m_is_paused = true;
    m_paused_timer.start();
}

void DelayedTask::resume()
{
    m_is_paused = false;
    m_paused_timer.stop();
    m_elapsed_time += m_paused_timer.elapsed();
    m_timer.start();
}

