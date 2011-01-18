#ifndef WIN32
#include <sys/time.h>
#endif
#include "os/thread.h"

using namespace os;

#ifdef WIN32

thread::info::info(thread_function f, void * p)
{
  running=false;
  fun=f;
  parameter=p;
}       

thread::thread(thread_function fun, void * param, bool waitable)
{
  m_thread=NULL;
  m_info=os::thread::info(fun,param);
}

thread::~thread()
{
  if (m_thread!=NULL) CloseHandle(m_thread);	
}

DWORD WINAPI thread::proxy(LPVOID info)
{
   thread::info * inf=(thread::info *)info;
   void * (*thread_f)(void*)=inf->fun;
   void * p=inf->parameter;
   inf->running=true;
   void * result=thread_f(p);
   return (DWORD)result;	
}

void thread::detach()
{
	
}
void thread::makeWaitable()
{
	
}

void thread::stop()
{
	if (m_info.running && m_thread!=NULL)  TerminateThread(m_thread,13);
}

bool thread::wait(unsigned int time)
{
  if (m_info.running)
  {
  	if (time==INFINITE) 
        { 
             while (WaitForSingleObject(m_thread,1)!=WAIT_OBJECT_0);
			 return true;  
        }
        else 
        {
		    return WaitForSingleObject(m_thread,1)==WAIT_OBJECT_0;
        }  
  }
  return false;	
}

void * thread::result()
{
  if (m_info.running)
  {
     while (WaitForSingleObject(m_thread,1)!=WAIT_OBJECT_0);
	DWORD res;
	GetExitCodeThread(m_thread,&res);
	return (void*)res;
  }
  else return NULL;
}

bool thread::run()
{
  m_thread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)thread::proxy,(LPVOID)&m_info,0,NULL);
  if (m_thread!=NULL)  while (!(m_info.running));
  return m_thread!=NULL;
}


#else

thread::info::info(bool _waitable)
{
	    running=false;
        waitable=_waitable;
        result=NULL;
        fun=NULL;
        parameter=NULL;
}

thread::thread(thread_function fun, void * param, bool waitable)
{
   m_thread=(pthread_t)0;
   pthread_attr_init(&m_attr);
   m_info=thread::info::info(waitable);
   m_info.fun=fun;
   m_info.parameter=param;	
}

thread::~thread()
{
  pthread_attr_destroy(&m_attr);	
}

void * thread::proxy(void * info)
{
   thread::info * inf=(thread::info *)info;
   void * (*thread_f)(void*)=inf->fun;
   void * p=inf->parameter;
   bool waitable=inf->waitable;
   inf->running=true;
   void * result=thread_f(p);
    if (waitable)
    {
    	inf->running=false;
        inf->result=result;
    }
    return result;	
}

void thread::detach()
{
	m_info.waitable=false;
}
void thread::makeWaitable()
{
	m_info.waitable=true;
}
void thread::stop()
{
	if (m_info.running)  pthread_cancel(m_thread);
}
bool thread::wait(unsigned int time)
{
  if (m_info.running)
  {
  	if (time==INFINITE) 
        { 
             pthread_join(m_thread,&(m_info.result));
             m_info.running=false; 
             return true;  
        }
        else 
        {
            if (m_info.waitable)
            {
              clock_t start=clock();
              while ( (float)(clock()-time)/CLOCKS_PER_SEC*1.0E+3 < time && m_info.running  );
              return m_info.running;  
            }
            else return false;	
        }  
  }
  return false;	
}
void * thread::result()
{
  if (!m_info.waitable)  return m_info.result;
  pthread_join(m_thread,&(m_info.result));
  return m_info.result;
}
bool thread::run()
{
  bool r=pthread_create(&m_thread,&m_attr,thread::proxy,(void*)&m_info)==0;
  if (r)  while (!(m_info.running));
  return r;
}
#endif

