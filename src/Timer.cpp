/*! \file   timer.cpp
    \author FreakyBlast

	\brief  Definition of SadTimer.

	This file contains a definition of SadTimer. 
*/


#include "timer.h"

/*TODO from FB: Port to linux
Replace for QueryPerformanceCounter (WinAPI)

timeval t1, t2;
double elapsedTime;

// start timer
gettimeofday(&t1, NULL);

// do something
...

// stop timer
gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
cout << elapsedTime << " ms.\n";

visit http://www.songho.ca/misc/timer/timer.html
*/

SadTimer::SadTimer(void)
{

}

SadTimer::~SadTimer(void)
{
}


long long freq;  //!< Beat frequency timer


/*!\fn Initialize all operation with timer
*/
void sadInitTimeOperation()
{
	LARGE_INTEGER s;
	// gets a frequency
	QueryPerformanceFrequency(&s);
	// save frequency
	freq=s.QuadPart;
}
/*!\fn Gets current value of the global counter
*/ 
long long sadTime()
{
	LARGE_INTEGER s;
	// gets the counter
	QueryPerformanceCounter(&s);
	// returns only quad part of counter
	return s.QuadPart;
}

/*!\fn Gets current time in ticks*/
long long sadGetTicksTime()
{
	LARGE_INTEGER s;
	// gets the counter
	QueryPerformanceCounter(&s);
	return ((s.QuadPart)*1000/freq);
}

/*!\fn Sets start time (enable timer)*/
void SadTimer::sadStartTiming()
{
	// Saves time
	StartTime=sadTime();
}

/*!\fn Returns time elapsed
   \return Time in ticks*/ 
long long SadTimer::sadTimeElapsed()
{
	// Returns difference between current moment and start (in seconds)
	return ((sadTime()-StartTime)*1000/freq);
}

/*!\fn  Convert ticks to second
   \param[in] ticks Time in ticks
   \return Time in seconds */
double sadConvertTicksToSeconds(long long ticks)
{
	return (double)(ticks/freq);
}
