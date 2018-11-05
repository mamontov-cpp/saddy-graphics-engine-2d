#include "pausabletimer.h"

sad::PausableTimer::PausableTimer() 
: m_running(false), m_elapsed(0)
{

}

sad::PausableTimer::~PausableTimer() 
{

}

void sad::PausableTimer::start()
{
    m_elapsed = 0;
    m_timer.start();
    m_running = true;
}

void sad::PausableTimer::stop()
{
    m_timer.stop();
    m_running = false;
}


void sad::PausableTimer::pause()
{
    m_timer.stop();
    m_elapsed += m_timer.elapsed();
    m_running = false;
}

void sad::PausableTimer::resume()
{
    m_timer.start();
    m_running = true;
}

double sad::PausableTimer::elapsed() const
{
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
