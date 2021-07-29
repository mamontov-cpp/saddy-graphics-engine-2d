#include <os/semaphoreimpl.h>
#include <cassert>

#ifndef WIN32

#include <limits.h>
#include <unistd.h>
#include <sys/param.h>

#endif

#ifdef WIN32

sad::os::SemaphoreImpl::SemaphoreImpl(unsigned int currentvalue, unsigned int maxvalue)
: m_max_value(maxvalue)
{ 
    m_s = CreateSemaphoreA(nullptr,currentvalue,maxvalue,nullptr);
}

#else

sad::os::SemaphoreImpl::SemaphoreImpl(unsigned int currentvalue, unsigned int maxvalue)
{ 
    assert(currentvalue <= SEM_VALUE_MAX);
    assert(maxvalue  <= SEM_VALUE_MAX);
    // printf("Creating semaphore\n");
    int result = sem_init(&m_s,0,currentvalue);
    // printf("Semaphore created %d\n", result);
}

#endif


sad::os::SemaphoreImpl::~SemaphoreImpl()
{
#ifdef WIN32
    CloseHandle(m_s);
#else
    sem_close(&m_s);
#endif
}

void sad::os::SemaphoreImpl::consume(unsigned int v)
{
    for (unsigned int i=0;i < v;i++)   
    {
#ifdef WIN32
        WaitForSingleObject(m_s, INFINITE); 
#else
        sem_wait(&m_s); 
#endif
    }
}

void sad::os::SemaphoreImpl::release(unsigned int v)
{
#ifdef WIN32
    ReleaseSemaphore(m_s,(LONG)v,nullptr);
#else
    for (unsigned int i = 0; i < v; i++)  
    {
        //printf("Attempting to post sth\n");
        sem_post(&m_s);
        //printf("Posted\n");
    }
#endif
}

int sad::os::SemaphoreImpl::value() const
{
#ifdef WIN32
    LONG val=4568;
    if (ReleaseSemaphore(m_s,1,&val) == FALSE)
    {
        // Maximum value is exceeded, so just false
        return m_max_value;
    }
    WaitForSingleObject(m_s, INFINITE);
    return (int)val;
#else
    int n=0;
    sem_t *  s = const_cast<sem_t*>(&m_s);
    sem_getvalue(s,&n);
        return n;
#endif
}
