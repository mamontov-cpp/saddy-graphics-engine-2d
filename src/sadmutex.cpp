#include <sadmutex.h>
#include <os/muteximpl.h>

sad::Mutex::Mutex()
: m_dptr(new sad::os::MutexImpl())
{

}

sad::Mutex::Mutex(const sad::Mutex &)
: m_dptr(new sad::os::MutexImpl())
{

}

sad::Mutex & sad::Mutex::operator=(const sad::Mutex & o)
{
	return *this;
}

sad::Mutex::~Mutex()
{
	delete m_dptr;
}

void sad::Mutex::lock()
{
	m_dptr->lock();
}

void sad::Mutex::unlock()
{
	m_dptr->unlock();
}
