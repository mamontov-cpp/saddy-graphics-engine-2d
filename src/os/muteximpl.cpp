#include <os/muteximpl.h>

sad::os::MutexImpl::MutexImpl()
{
#ifdef WIN32
	m_m = CreateMutexA(NULL,FALSE,NULL);
#else
	pthread_mutex_init(&m_m,NULL);
#endif
}

sad::os::MutexImpl::~MutexImpl()
{
#ifdef WIN32
	CloseHandle(m_m);
#else
	pthread_mutex_destroy(&m_m);
#endif
}

void sad::os::MutexImpl::lock()
{
#ifdef WIN32
	WaitForSingleObject(m_m,INFINITE);
#else
	pthread_mutex_lock(&m_m);
#endif
}

void sad::os::MutexImpl::unlock()
{
#ifdef WIN32
	ReleaseMutex(m_m);
#else
	pthread_mutex_unlock(&m_m);
#endif
}
