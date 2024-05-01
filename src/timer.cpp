#include <timer.h>
#include <os/timerimpl.h>
#include "opticksupport.h"

sad::Timer::Timer() : m_dptr(new sad::os::TimerImpl())
{
    PROFILER_EVENT;

}

sad::Timer::Timer(const sad::Timer &o) 
: m_dptr(new sad::os::TimerImpl(*(o.m_dptr)))
{
    PROFILER_EVENT;

}

sad::Timer& sad::Timer::operator=(const sad::Timer & o)
{
    PROFILER_EVENT;
    if (this == &o)
    {
        return *this;
    }
    delete m_dptr;
    m_dptr = new sad::os::TimerImpl(*(o.m_dptr));
    return *this;
}

void sad::Timer::start()
{
    PROFILER_EVENT;
    m_dptr->start();
}

void sad::Timer::stop()
{
    PROFILER_EVENT;
    m_dptr->stop();
}

double sad::Timer::elapsed() const
{
    PROFILER_EVENT;
    return m_dptr->elapsed();
}

sad::Timer::~Timer()
{
    PROFILER_EVENT;
    delete m_dptr;
}
