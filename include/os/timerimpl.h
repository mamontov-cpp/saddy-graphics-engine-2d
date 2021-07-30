/*! \file      timer.h
    \author    mamontov-cpp
    
    Describes a crossplatform high-resolution timer.
 */
#pragma once
#include <cstdio>
#include <ctime>
#ifdef WIN32
    #ifndef NOMINMAX
    #define NOMINMAX 
    #endif
    #include  <windows.h>
#else
    #include  <sys/time.h>
    #define SADDY_USED_CLOCK_TYPE CLOCK_MONOTONIC
#endif


namespace sad
{

namespace os
{

#ifdef WIN32
/*! Puts last error from calls of Window API functions
 */
void put_last_error();
#endif


/*! A high-resolution timer to measure performance (sad::Renderer's fps and other places).
    An approach taken from http://stackoverflow.com/questions/2150291/how-do-i-measure-a-time-interval-in-c
    Linux implementation taken from http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/.

    Note that this is platform dependent version. In any common case you should use @see sad::Timer
 */
class TimerImpl
{
private:
#ifdef WIN32
    LARGE_INTEGER m_frequency1;
    LARGE_INTEGER m_frequency2;
    LARGE_INTEGER m_frequency3;

    LARGE_INTEGER m_start;         
    LARGE_INTEGER m_end;
#else
    timespec      m_start;         
    timespec     m_end;         
    timespec     m_frequency;
#endif
public:
    /*! Creates a new timer and starts it immediately
     */
    TimerImpl();
    /*! Starts a timer
     */
    void start();
    /*! Stops a timer. After stopping, you can measure time, using elapsed
     */
    void stop();
    /*! Returns elapsed time between sad::os::TimerImpl::start() and 
        sad::TimerImpl::stop()
        in milliseconds
        \return elapsed time in milliseconds
     */
    double elapsed() const;
};

}

}
