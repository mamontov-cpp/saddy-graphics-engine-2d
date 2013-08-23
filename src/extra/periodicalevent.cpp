#include <extra/periodicalevent.h>

PeriodicalEvent::PeriodicalEvent()
{
	m_last_shot = 0;
	m_interval = 50.0;
	m_enabled = true;
}

PeriodicalEvent::~PeriodicalEvent()
{
}

void PeriodicalEvent::tryPerform()
{
	double deltainsec = (clock() - m_last_shot) / CLOCKS_PER_SEC;
	double deltainms = deltainsec * 1000.0;
	if (deltainms >= m_interval && m_enabled)
	{
		m_last_shot = clock();
		perform();
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
