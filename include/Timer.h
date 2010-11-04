/*! \file   timer.h
    \author FreakyBlast

	\brief  Declaration of SadTimer.

	This file contains a declaration of SadTimer. 
*/

#ifndef Timer_h__
#define Timer_h__

#include "types.h"

/*!\class Timer class
*/
class SadTimer
{

	long long StartTime;  //!< Beat frequency timer
public:
	SadTimer(void);
	~SadTimer(void);

	/*!\fn Start timer 
	*/
	void __fastcall sadStartTiming();
    /*!\fn How much time passed in ticks
	   \return */
	long long __fastcall sadTimeElapsed();
};

/*!\fn Current time in milliseconds
   \return Time in milliseconds since start of timer
   */
long long __fastcall sadTime(void);
/*!\fn Gets time in ticks
   \return Time in ticks since start of timer
   */
long long __fastcall sadGetTicksTime(void);
/*!\fn Converts ticks to seconds
   \param[in] ticks Quantity of ticks
   \return Time in seconds
   */
double sadConvertTicksToSeconds(long long ticks);

/*!\fn Initialize all operation with timer
*/
void __fastcall sadInitTimeOperation(void);

#endif // Timer_h__