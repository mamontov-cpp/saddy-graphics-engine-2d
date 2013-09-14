/*! \file      timer.h
    \author    HiddenSeeker
    
	Describes a crossplatform high-resolution timer.
 */
#pragma once
#ifdef WIN32
    #ifndef NOMINMAX
    #define NOMINMAX 
    #endif
    #include  <windows.h>
#else
    #include  <sys/time.h>
#endif


namespace os
{


/*! A high-resolution timer to measure performance (sad::Renderer's fps and other places).
    An approach taken from http://stackoverflow.com/questions/2150291/how-do-i-measure-a-time-interval-in-c
    Linux implementation taken from http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/ 
 */
class timer
{
private:
#ifdef WIN32
	LARGE_INTEGER m_frequency;    
    LARGE_INTEGER m_start;         
	LARGE_INTEGER m_end;         
#else
	timespec      m_start;         
	timespec     m_end;         
        timespec     m_frequency;
#endif
public:
	/*! Creates a new timer
	 */
	inline timer()
	{
#ifdef WIN32
	    QueryPerformanceFrequency(&m_frequency);
#else
		clock_getres(CLOCK_PROCESS_CPUTIME_ID, &m_frequency);
#endif
		start();
	}

	/*! Starts a timer
	 */
	inline void start()
	{
#ifdef WIN32
		QueryPerformanceCounter(&m_start);
#else
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &m_start);
#endif
	}
	/*! Stops a timer
	 */
	inline void stop()
	{
#ifdef WIN32
		QueryPerformanceCounter(&m_end);
#else
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &m_end);
#endif
	}
	/*! Returns elapsed time in milliseconds
		\return elapsed time in milliseconds
	 */
	inline double elapsed() const
	{
#ifdef WIN32
		return (m_end.QuadPart - m_start.QuadPart) * 1000.0 / m_frequency.QuadPart;
#else
		
		double elapsednsec;
		if (m_end.tv_nsec - m_start.tv_nsec < 0) 
		{
			elapsednsec = 1000000000.0+m_end.tv_nsec-m_start.tv_nsec;
		}
		else
		{
			elapsednsec = m_end.tv_nsec-m_start.tv_nsec;
		}
		//elapsednsec /= (double)(m_frequency.tv_nsec);
		return elapsednsec / 1000000.0  + (m_end.tv_sec - m_start.tv_sec) * 1000.0;
#endif
	}




};


}
