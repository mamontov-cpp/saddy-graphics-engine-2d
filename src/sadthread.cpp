#include <sadthread.h>

#include <os/threadimpl.h>

#include <limits>

const int sad::Thread::Cancelled = std::numeric_limits<int>::min();

sad::Thread::Thread(const sad::Thread & o)
: m_dptr( new sad::os::ThreadImpl(*(o.m_dptr)) )
{

}

sad::Thread::~Thread()
{
	delete m_dptr;
}


void sad::Thread::initialize(sad::AbsractThreadExecutableFunction * f)
{
	m_dptr = new sad::os::ThreadImpl(f);
}

sad::Thread & sad::Thread::operator=(const sad::Thread & o)
{
	delete m_dptr;
	m_dptr = new sad::os::ThreadImpl(*(o.m_dptr));
	return *this;
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


