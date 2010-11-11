/*! \file   timer.h
    \author FreakyBlast

	\brief  Declaration of SadTimer.

	This file contains a declaration of SadTimer. 
*/

#ifndef Timer_h__
#define Timer_h__

#include "types.h"

namespace Sad{
/*!\class Timer class
*/
class Timer
{

	long long StartTime;  //!< Beat frequency timer
public:
	Timer(void);
	~Timer(void);

	/*!\fn Start timer 
	*/
	void Start();
    /*!\fn How much time passed in ticks
	   \return */
	unsigned ElapsedInTicks();
};
}

/*!\fn Current time in milliseconds
   \return Time in milliseconds since start of timer
   */
long long sadTime(void);
/*!\fn Gets time in ticks
   \return Time in ticks since start of timer
   */
long long  sadGetTicksTime(void);
/*!\fn Converts ticks to seconds
   \param[in] ticks Quantity of ticks
   \return Time in seconds
   */
double sadConvertTicksToSeconds(long long ticks);

/*!\fn Initialize all operation with timer
*/
void sadInitTimeOperation(void);

#endif // Timer_h__