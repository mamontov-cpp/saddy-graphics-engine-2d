#include <periodicalevent.h>
#include <log/log.h>

sad::PeriodicalEvent::PeriodicalEvent()
{
	m_interval = 50.0;
	m_enabled = true;
	m_timer.start();
}

sad::PeriodicalEvent::~PeriodicalEvent()
{
}

void sad::PeriodicalEvent::tryPerform()
{
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
	m_interval = interval;
}

void sad::PeriodicalEvent::disable()
{
	m_enabled = false;
}

void sad::PeriodicalEvent::enable()
{
	m_enabled = true;
}


sad::PeriodicalEventPollProcess::PeriodicalEventPollProcess(PeriodicalEvent * e) : m_event(e)
{

}

sad::PeriodicalEvent * sad::PeriodicalEventPollProcess::e()
{
	return m_event;
}

void sad::PeriodicalEventPollProcess::_process()
{
	if (m_event)
	{
		m_event->tryPerform();
	}
}

void sad::PeriodicalEventPollProcess::setEvent(PeriodicalEvent * e)
{
	delete m_event;
	m_event = e;
}

sad::PeriodicalEventPollProcess::~PeriodicalEventPollProcess()
{
	delete m_event;
}
