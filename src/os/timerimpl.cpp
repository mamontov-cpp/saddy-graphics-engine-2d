#include <os/timerimpl.h>
#include <log/log.h>

#ifdef WIN32

 
void sad::os::put_last_error() {
    DWORD errCode = GetLastError();
    char *err;
    if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                       nullptr,
                       errCode,
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
                       (LPTSTR) &err,
                       0,
                       nullptr))
    {
        SL_CRITICAL("Can't format error message!");
    }

    SL_CRITICAL(err);
    LocalFree(err);
}

#endif

sad::os::TimerImpl::TimerImpl() //-V730
{
#ifdef WIN32

    typedef LONG (__stdcall  *PTR) (ULONG,BOOLEAN,PULONG);

    HMODULE hDll = LoadLibraryA("ntdll.dll");

    PTR lpfnDLLProc = (PTR)GetProcAddress (hDll, "NtSetTimerResolution");
    ULONG DesiredResolution = 50000; // 1ms
    BOOLEAN SetResolution = TRUE;
    ULONG CurrentResolution = 0 ;
    LONG ret = lpfnDLLProc(DesiredResolution, SetResolution,&CurrentResolution);

    if(!QueryPerformanceFrequency(&m_frequency1))
    {
        sad::os::put_last_error();
    }
#else
        clock_getres(SADDY_USED_CLOCK_TYPE, &m_frequency);
#endif
        start();
}


void sad::os::TimerImpl::start()
{
#ifdef WIN32
    QueryPerformanceFrequency(&m_frequency1);
    BOOL result = QueryPerformanceCounter(&m_start); 
    if (!result)
    {
        sad::os::put_last_error();
    }
#else
        clock_gettime(SADDY_USED_CLOCK_TYPE, &m_start);
#endif
}

void sad::os::TimerImpl::stop()
{
#ifdef WIN32
    QueryPerformanceFrequency(&m_frequency2);
    BOOL result = QueryPerformanceCounter(&m_end); 
    if (!result)
    {
        sad::os::put_last_error();
    }
#else
        clock_gettime(SADDY_USED_CLOCK_TYPE, &m_end);
#endif
}



double sad::os::TimerImpl::elapsed() const
{
#ifdef WIN32
    sad::os::TimerImpl * me = const_cast<sad::os::TimerImpl *>(this);
    QueryPerformanceFrequency(&(me->m_frequency3));
    double frequency = (double)(std::max(std::max(m_frequency1.QuadPart, m_frequency2.QuadPart), m_frequency3.QuadPart));
    double millisecondsofhpc = (m_end.QuadPart - m_start.QuadPart) / frequency * 1000.0;

    return millisecondsofhpc;
#else
    double starttime = m_start.tv_sec * 1.0E+3 + m_start.tv_nsec * 1.0E-6;
    double endtime = m_end.tv_sec * 1.0E+3 + m_end.tv_nsec * 1.0E-6;
    return endtime - starttime;
#endif
}
