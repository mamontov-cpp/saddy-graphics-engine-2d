#include <utility>

#include "delayedtask.h"


DelayedTask::DelayedTask(double time, std::function<void()>  f)
: m_fn(std::move(f)),
m_time(time)
{
    m_timer.start();
}

// ReSharper disable once CppMemberFunctionMayBeConst
bool DelayedTask::tryExecute()
{
     if (m_timer.paused())
     {
         return false;
     }

     if (m_timer.elapsed() >= m_time)
     {
         m_fn();
         return true;
     }
     return false;
}

void DelayedTask::pause()
{
    m_timer.pause();
}

void DelayedTask::resume()
{
    m_timer.resume();
}

