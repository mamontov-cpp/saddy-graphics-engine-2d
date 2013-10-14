#include "os/mutex.h"

using namespace os;

#ifdef WIN32

mutex::mutex(bool locked)
{
  m_m=CreateMutexA(NULL,(locked)?TRUE:FALSE,NULL);
}
mutex::~mutex()
{
 CloseHandle(m_m);
}
void mutex::lock()
{
  while(WaitForSingleObject(m_m,1)!=WAIT_OBJECT_0);
}
void mutex::unlock()
{
  ReleaseMutex(m_m);
}

#else

mutex::mutex(bool locked)
{
  pthread_mutex_init(&m_m,NULL);
  if (locked)
      pthread_mutex_lock(&m_m);
}
mutex::~mutex()
{
  pthread_mutex_destroy(&m_m);	
}
void mutex::lock()
{
  pthread_mutex_lock(&m_m);
}
void mutex::unlock()
{
  pthread_mutex_unlock(&m_m);
}

#endif
