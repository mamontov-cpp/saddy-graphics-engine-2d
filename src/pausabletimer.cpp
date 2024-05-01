#include "pausabletimer.h"
#include "opticksupport.h"

sad::PausableTimer::PausableTimer() 
: m_running(false), m_has_called_start(false), m_elapsed(0)
{
    PROFILER_EVENT;

}

sad::PausableTimer::~PausableTimer() = default;

void sad::PausableTimer::start()
{
    PROFILER_EVENT;
    m_elapsed = 0;
    m_timer.start();
    m_has_called_start = true;
    m_running = true;
}

void sad::PausableTimer::stop()
{
    PROFILER_EVENT;
    if (!m_has_called_start)
    {
        start();
    }
    m_timer.stop();
    m_running = false;
}


void sad::PausableTimer::pause()
{
    PROFILER_EVENT;
    if (!m_has_called_start)
    {
        start();
    }
    // If we are not running, no reason to pause at all
    if (!m_running)
    {
        return;
    }
    m_timer.stop();
    m_elapsed += m_timer.elapsed();
    m_running = false;
}

void sad::PausableTimer::resume()
{
    PROFILER_EVENT;
    // We are running already, no reason to restart timer, flushing the time
    if (m_running)
    {
        return;
    }
    m_timer.start();
    m_running = true;
}

double sad::PausableTimer::elapsed() const
{
    PROFILER_EVENT;
    double elapsed = 0;
    sad::PausableTimer* me = const_cast<sad::PausableTimer*>(this);
    if (m_running)
    {
        me->m_timer.stop();
        me->m_elapsed += m_timer.elapsed();
        me->m_timer.start();
    }
    else
    {
        elapsed = m_timer.elapsed();
    }
    return m_elapsed + elapsed;
}
