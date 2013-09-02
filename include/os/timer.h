/*! \file      timer.h
    \author    HiddenSeeker
    
	Describes a crossplatform high-resolution timer.
 */
#pragma once
#ifdef WIN32
          #include  <windows.h>
#else
          #include  <sys/time.h>
#endif


namespace os
{


/*! A high-resolution timer to measure performance (sad::Renderer's fps and other places).
    An approach taken from http://stackoverflow.com/questions/2150291/how-do-i-measure-a-time-interval-in-c
 */
class timer
{
private:
#ifdef WIN32
	LARGE_INTEGER m_frequency;    
    LARGE_INTEGER m_start;         
	LARGE_INTEGER m_end;         
#else
	timeval      m_start;         
	timeval      m_end;          
#endif
public:
	/*! Creates a new timer
	 */
	inline timer()
	{
#ifdef WIN32
	    QueryPerformanceFrequency(&m_frequency);
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
		gettimeofday(&m_start, NULL);
#endif
	}
	/*! Stops a timer
	 */
	inline void stop()
	{
#ifdef WIN32
		QueryPerformanceCounter(&m_end);
#else
		gettimeofday(&m_end, NULL);
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
		double result;
		result = (m_start.tv_sec - m_end.tv_sec) * 1000.0;     
	    result += (m_start.tv_usec - m_end.tv_usec) / 1000.0;   
#endif
	}




};


}
