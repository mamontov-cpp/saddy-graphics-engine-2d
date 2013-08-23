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
		m_last_shot = 0;
		perform();
	}
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
	m_event->tryPerform();
}


TimePeriodicalTask::~TimePeriodicalTask()
{
	delete m_event;
}
