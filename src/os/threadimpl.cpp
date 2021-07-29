#include "os/threadimpl.h"
#include "sadmutex.h"
#include "sadsleep.h"
#include "sadthreadexecutablefunction.h"
#include "sadhash.h"
#include "sadthread.h"

#ifdef WIN32

#include <process.h>

sad::os::ThreadId sad::os::current_thread_id()
{
    return 	GetCurrentThread();
}

#endif

#ifdef LINUX

 #include <linux/unistd.h>
 #include <sys/types.h>
 
sad::os::ThreadId sad::os::current_thread_id()
{
    return  syscall(SYS_gettid);
}

#endif


#ifdef LINUX
#include <sys/time.h>

/*! A special status structure, which allows us to do timed waits for thread, via
    table of threas statuses
 */
struct ThreadStatus
{
    /*! Contains, whether thread is running at a time
     */
    bool Running;
    /*! Returns current status of thread
     */
    int  ResultCode;
    /*! Returns status, when current thread is running
     */
    static ThreadStatus makeRunning()
    {
        ThreadStatus result;
        result.Running = true;
        result.ResultCode = 0; 
        return result;
    }
    /*! Returns a starus, when current thread is running
        \return whether is finished
     */
    static ThreadStatus makeFinished(int code = sad::Thread::Cancelled)
    {
        ThreadStatus result;
        result.Running = false;
        result.ResultCode = code; 
        return result;
    }
};
/*! A mutex for synchronized access to thread
 */ 
::sad::Mutex  m_thread_table_mtx;
/*! A thread table, for getting a status
 */
static sad::Hash<pthread_t, ThreadStatus> m_thread_table;
/*! Write thread status to a thread table
    \param[in] thread a thread
    \param[in] status a status for thread
 */
static void write_thread_status(pthread_t thread, const ThreadStatus & status)
{
    m_thread_table_mtx.lock();
    if (m_thread_table.contains(thread) == false)
    {
        m_thread_table.insert(thread, status);
    }
    else
    {
        m_thread_table[thread] = status;
    }
    m_thread_table_mtx.unlock();
}
/*! Reads status from a thread table
    \param[in] thread a thread
    \return a thread status
 */
static ThreadStatus read_thread_status(pthread_t thread)
{
    ThreadStatus result;
    result.Running = false;
    result.ResultCode  = 0;
    m_thread_table_mtx.lock();
    if (m_thread_table.contains(thread))
    {
        result = m_thread_table[thread];
    }
    m_thread_table_mtx.unlock();
    return result;
}
/*! Registers new thread, which is already running
    \param[in] thread registered thread
 */
static void register_running_thread(pthread_t thread)
{
    write_thread_status(thread, ThreadStatus::makeRunning());
}
/*! Registers cancelled thread
 */
static void register_cancelled_thread(void *)
{
    write_thread_status(pthread_self(), ThreadStatus::makeFinished());
}
/*! Registers finished thread
    \param[in] code exit code
 */
static void register_finished_thread(int code)
{
    write_thread_status(pthread_self(), ThreadStatus::makeFinished(code));
}

#endif

sad::os::ThreadImpl::ThreadImpl(sad::AbstractThreadExecutableFunction * f)
: m_function(f),
#ifdef WIN32
  m_handle(INVALID_HANDLE_VALUE)
#else
  m_handle(0)
#endif
{

}

sad::os::ThreadImpl::ThreadImpl(const sad::os::ThreadImpl & o)
: m_function(o.m_function->clone()), m_handle(o.m_handle)
{
#ifdef WIN32
    // Duplicate handle, for handle closing handle
    DuplicateHandle(GetCurrentProcess(), 
                    o.m_handle, 
                    GetCurrentProcess(),
                    &m_handle,
                    0,
                    TRUE,
                    DUPLICATE_SAME_ACCESS
                   );
#endif
}

sad::os::ThreadImpl::~ThreadImpl()
{
    delete m_function;
#ifdef WIN32
    if (m_handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_handle);
    }
#endif
}


#ifdef WIN32

static unsigned int WINAPI thread_implementation_function(LPVOID function)
{
    sad::AbstractThreadExecutableFunction * f = reinterpret_cast<
        sad::AbstractThreadExecutableFunction *
    >(function);
    int code = f->execute();
    _endthreadex(code);
    return static_cast<unsigned int>(code);
}

#else

static void * thread_implementation_function(void * function)
{
    int oldstate = 0;
        
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,&oldstate);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldstate);
    
    int code = sad::Thread::Cancelled;
    // Make thread set status on cancel
    pthread_cleanup_push(register_cancelled_thread, nullptr);
    // Register thread 	
    register_running_thread(pthread_self());	
   
    // Execute code
    sad::AbstractThreadExecutableFunction * f = reinterpret_cast<
        sad::AbstractThreadExecutableFunction *
    >(function);
    code = f->execute();
    // Register finished thread
    register_finished_thread(code);
    pthread_cleanup_pop(0);

    return  reinterpret_cast<void*>(code);
}

#endif


bool sad::os::ThreadImpl::run()
{
    // Do not start thread, if already running
    if (running())
        return false;
#ifdef WIN32    
    m_handle = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, thread_implementation_function, m_function,0,nullptr));
    return m_handle!=nullptr;
#else
    pthread_attr_t attrs;
    pthread_attr_init(&attrs);
    int result = pthread_create(&m_handle,&attrs,thread_implementation_function,(void*)m_function);
    bool running = (result == 0);
    if (running)
    {
        register_running_thread(m_handle);
    }
    return running;
#endif
}

void sad::os::ThreadImpl::stop()
{
#ifdef WIN32
    if (m_handle != INVALID_HANDLE_VALUE)
    {
        if (running())
        {
            TerminateThread(m_handle, sad::Thread::Cancelled);
        }
    }
#else
    if (m_handle != 0)
    {
        if (running())
        {
            pthread_cancel(m_handle);
        }
    }
#endif
}

int sad::os::ThreadImpl::exitCode() const
{
    bool isvalid = true;
#ifdef WIN32
    isvalid = m_handle != INVALID_HANDLE_VALUE;
#else
    isvalid = m_handle != 0;
#endif
    int result = sad::Thread::Cancelled;
    if (isvalid)
    {
        const_cast<sad::os::ThreadImpl *>(this)->wait();
#ifdef WIN32
        DWORD res;
        GetExitCodeThread(m_handle,&res);
        result = res;
#else
        ThreadStatus status = read_thread_status(m_handle);
        result = status.ResultCode;
#endif
    }
    return result;
}

void sad::os::ThreadImpl::wait()
{
    if (running())
    {
#ifdef WIN32
        WaitForSingleObject(m_handle, INFINITE);
#else
        void * result = nullptr;
        pthread_join(m_handle,&result);
#endif
    }
}

void sad::os::ThreadImpl::wait(unsigned int milliseconds)
{
    if (running())
    {
#ifdef WIN32
        WaitForSingleObject(m_handle, milliseconds);
#else

#ifdef _GNU_SOURCE
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += milliseconds / 1000;
        ts.tv_nsec  += (milliseconds % 1000) * 1000000;
        if (ts.tv_nsec > 1000000000)
        {
            ts.tv_sec += 1;
            ts.tv_nsec -= 1000000000;
        }
        void * result = nullptr;
        pthread_timedjoin_np(m_handle, &result, &ts);
#else
        if (milliseconds < 2)
        {
            sad::sleep(milliseconds);
        }
        else
        {
            unsigned int steps = 10;
            unsigned int step = milliseconds / 10;
            bool exit = false;
            for(int i = 0; i < steps && !exit; i++)
            {
                sad::sleep(step);
                exit = this->running();
            }
        }
#endif

#endif
    }
}

bool sad::os::ThreadImpl::running() const
{
    bool result = false;
#ifdef WIN32
    if (m_handle != INVALID_HANDLE_VALUE)
    {
        if (WaitForSingleObject(m_handle, 0) != WAIT_OBJECT_0)
        {
            result = true;
        }
    }
#else
    if (m_handle != 0)
    {
        ThreadStatus status = read_thread_status(m_handle);
        if (status.Running)
        {
            result = true;
        }
    }
#endif
    return result;
}