#include "os/semaphore.h"
using namespace os;

#ifdef WIN32

semaphore::semaphore(unsigned int beginvalue, unsigned int maxvalue)
{ 
 m_v=maxvalue;
 m_s=CreateSemaphoreA(NULL,beginvalue,maxvalue,NULL);
}
semaphore::~semaphore()
{
  CloseHandle(m_s);
}
int    semaphore::value()
{
	LONG val=4568;
	ReleaseSemaphore(m_s,1,&val);
	if (val!=m_v) WaitForSingleObject(m_s,INFINITE);
	return (int)val;
}
void semaphore::consume(unsigned int v)
{
	for (int i=0;i<v;i++)   
    {
       while(WaitForSingleObject(m_s,1)!=WAIT_OBJECT_0); 
    }
}

void semaphore::release(unsigned int v)
{
 ReleaseSemaphore(m_s,(LONG)v,NULL);
}

#else

semaphore::semaphore(unsigned int beginvalue, unsigned int maxvalue)
{ 
 //printf("Creating semaphore\n");
  int rs=sem_init(&m_s,0,beginvalue);
 //printf("Semaphore created %d\n",rs);
}


semaphore::~semaphore()
{
  sem_close(&m_s);
}
int    semaphore::value()
{
	int n=0;
	sem_getvalue(&m_s,&n);
        return n;
}
void semaphore::release(unsigned int v)
{
	for (int i=0;i<v;i++)  
       {
       	  //printf("Attempting to post sth\n");
       	  sem_post(&m_s);
          //printf("Posted\n");
       }
}
void semaphore::consume(unsigned int v)
{
	for (int i=0;i<v;i++)   
        {
        	sem_wait(&m_s); 
        }
}

#endif
