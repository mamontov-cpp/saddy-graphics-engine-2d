#include <os/muteximpl.h>

sad::os::MutexImpl::MutexImpl()
{
#ifdef WIN32
	InitializeCriticalSection(&m_m);
#else
	pthread_mutex_init(&m_m,NULL);
#endif
}

sad::os::MutexImpl::~MutexImpl()
{
#ifdef WIN32
	DeleteCriticalSection(&m_m);
#else
	pthread_mutex_destroy(&m_m);
#endif
}

void sad::os::MutexImpl::lock()
{
#ifdef WIN32
	EnterCriticalSection(&m_m);
#else
	pthread_mutex_lock(&m_m);
#endif
}

void sad::os::MutexImpl::unlock()
{
#ifdef WIN32
	LeaveCriticalSection(&m_m);
#else
	pthread_mutex_unlock(&m_m);
#endif
}
