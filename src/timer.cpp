#include <timer.h>
#include <os/timerimpl.h>

sad::Timer::Timer() : m_dptr(new sad::os::TimerImpl())
{

}

sad::Timer::Timer(const sad::Timer &o) 
: m_dptr(new sad::os::TimerImpl(*(o.m_dptr)))
{

}

sad::Timer& sad::Timer::operator=(const sad::Timer & o)
{
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
    m_dptr->start();
}

void sad::Timer::stop()
{
    m_dptr->stop();
}

double sad::Timer::elapsed() const
{
    return m_dptr->elapsed();
}

sad::Timer::~Timer()
{
    delete m_dptr;
}
