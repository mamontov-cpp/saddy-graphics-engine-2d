#include <periodicalevent.h>
#include <log/log.h>
#include "opticksupport.h"

sad::PeriodicalEvent::PeriodicalEvent()
{
    PROFILER_EVENT;
    m_interval = 50.0;
    m_enabled = true;
    m_timer.start();
}

sad::PeriodicalEvent::~PeriodicalEvent()
{
    PROFILER_EVENT;
}

void sad::PeriodicalEvent::tryPerform()
{
    PROFILER_EVENT;
    m_timer.stop();
    double deltainms = m_timer.elapsed();
    if (deltainms >= m_interval && m_enabled)
    {
        m_timer.start();
        this->perform();
    }
}

void sad::PeriodicalEvent::setInterval(double interval)
{
    PROFILER_EVENT;
    m_interval = interval;
}

void sad::PeriodicalEvent::disable()
{
    PROFILER_EVENT;
    m_enabled = false;
}

void sad::PeriodicalEvent::enable()
{
    PROFILER_EVENT;
    m_enabled = true;
}


sad::PeriodicalEventPollProcess::PeriodicalEventPollProcess(PeriodicalEvent * e) : m_event(e)
{
    PROFILER_EVENT;

}

sad::PeriodicalEvent * sad::PeriodicalEventPollProcess::e()
{
    PROFILER_EVENT;
    return m_event;
}

void sad::PeriodicalEventPollProcess::_process()
{
    PROFILER_EVENT;
    if (m_event)
    {
        m_event->tryPerform();
    }
}

void sad::PeriodicalEventPollProcess::setEvent(PeriodicalEvent * e)
{
    PROFILER_EVENT;
    delete m_event;
    m_event = e;
}

sad::PeriodicalEventPollProcess::~PeriodicalEventPollProcess()
{
    PROFILER_EVENT;
    delete m_event;
}
