#include <sadthread.h>

#include <os/threadimpl.h>

#include <limits>

const int sad::Thread::Cancelled = std::numeric_limits<int>::min();

sad::Thread::Thread(const sad::Thread & o)
: m_dptr( new sad::os::ThreadImpl(*(o.m_dptr)) )
{

}

sad::Thread& sad::Thread::operator=(const sad::Thread & o)
{
    if (this == &o)
    {
        return *this;
    }
    if (running())
    {
        this->stop();
    }
    delete m_dptr;
    m_dptr  = new sad::os::ThreadImpl(*(o.m_dptr));
    return *this;
}

sad::Thread::~Thread()
{
    delete m_dptr;
}


void sad::Thread::initialize(sad::AbstractThreadExecutableFunction * f)
{
    m_dptr = new sad::os::ThreadImpl(f);
}

bool sad::Thread::run()
{
    return m_dptr->run();
}

void sad::Thread::stop()
{
    m_dptr->stop();
}

int sad::Thread::exitCode() const
{
    return m_dptr->exitCode();
}

void sad::Thread::wait()
{
    m_dptr->wait();
}

void sad::Thread::wait(int milliseconds)
{
    m_dptr->wait(milliseconds);
}

bool sad::Thread::running() const
{
    return m_dptr->running();
}


