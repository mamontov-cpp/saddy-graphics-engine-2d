/*! \file sadsleep.h
    

    Describes crossplatform implementation of sleep - a function, which suspends function
    for time.
 */
#pragma once

namespace sad
{
/*! Suspend current thread for at least specified time of milliseconds
    \param[in] milliseconds
 */
void sleep(unsigned int milliseconds);

}
