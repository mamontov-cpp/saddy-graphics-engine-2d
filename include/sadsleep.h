/*! \file sadsleep.h
    

    Describes cross-platform implementation of sleep - a function, which suspends function
    for time.
 */
#pragma once

namespace sad
{
/*! Suspend current thread for at least specified time of milliseconds
    \param[in] milliseconds time, the thread would be suspended for
 */
void sleep(unsigned int milliseconds);

}
