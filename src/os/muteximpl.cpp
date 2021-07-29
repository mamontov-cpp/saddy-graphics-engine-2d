#include <os/muteximpl.h>

sad::os::MutexImpl::MutexImpl() 
#ifndef WIN32 
: m_locked(false)
#endif
{
#ifdef WIN32
    InitializeCriticalSection(&m_m);
#else
    pthread_mutex_init(&m_m,nullptr);
    pthread_mutex_init(&m_guard,nullptr);
#endif
}

sad::os::MutexImpl::~MutexImpl()
{
#ifdef WIN32
    DeleteCriticalSection(&m_m);
#else
    pthread_mutex_destroy(&m_m);
    pthread_mutex_destroy(&m_guard);
#endif
}

void sad::os::MutexImpl::lock()
{
#ifdef WIN32
    EnterCriticalSection(&m_m);
#else
    bool should_call_lock = false;
    pthread_mutex_lock(&m_guard);
    if (m_locked)
    {
        if (pthread_equal(m_locked_thread, pthread_self()) == 0)
        {
            should_call_lock = true;
        }
    }
    else
    {       
        should_call_lock = true;
    }
    pthread_mutex_unlock(&m_guard);
    
    if (should_call_lock) {
        pthread_mutex_lock(&m_m);
        
        pthread_mutex_lock(&m_guard);
        m_locked = true;
        m_locked_thread = pthread_self();
        pthread_mutex_unlock(&m_guard);
    }
#endif
}

void sad::os::MutexImpl::unlock()
{
#ifdef WIN32
    LeaveCriticalSection(&m_m);
#else
    bool should_call_unlock = false;
    pthread_mutex_lock(&m_guard);
    if (m_locked)
    {
        if (pthread_equal(m_locked_thread, pthread_self()) != 0)
        {
            should_call_unlock = true;
        }
    }
    pthread_mutex_unlock(&m_guard);
    
    if (should_call_unlock)
    {
        pthread_mutex_unlock(&m_m);
        
        pthread_mutex_lock(&m_guard);
        m_locked = false;
        pthread_mutex_unlock(&m_guard);
    }
#endif
}
