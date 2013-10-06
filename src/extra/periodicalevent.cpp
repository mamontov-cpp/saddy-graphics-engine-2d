#include <extra/periodicalevent.h>
#include <log/log.h>

using namespace sad;

PeriodicalEvent::PeriodicalEvent()
{
	m_interval = 50.0;
	m_enabled = true;
	m_timer.start();
}

PeriodicalEvent::~PeriodicalEvent()
{
}

void PeriodicalEvent::tryPerform()
{
	m_timer.stop();
	double deltainms = m_timer.elapsed();
	if (deltainms >= m_interval && m_enabled)
	{
		m_timer.start();
		this->perform();
	}
}

void PeriodicalEvent::setInterval(double interval)
{
	m_interval = interval;
}

void PeriodicalEvent::disable()
{
	m_enabled = false;
}

void PeriodicalEvent::enable()
{
	m_enabled = true;
}


TimePeriodicalTask::TimePeriodicalTask(PeriodicalEvent * e) : m_event(e)
{

}

PeriodicalEvent * TimePeriodicalTask::e()
{
	return m_event;
}

void TimePeriodicalTask::perform()
{
	if (m_event)
	{
		m_event->tryPerform();
	}
}

void TimePeriodicalTask::setEvent(PeriodicalEvent * e)
{
	delete m_event;
	m_event = e;
}

TimePeriodicalTask::~TimePeriodicalTask()
{
	delete m_event;
}
