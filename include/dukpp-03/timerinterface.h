/*! \file timerinteface.h
  
    Defines a TimerInterface, based on sad::Timer
 */
#pragma once
#include "../timer.h"

namespace sad
{

namespace dukpp03
{

/*! An interface for timer interaction with duktape
 */
struct TimerInterface
{
    typedef sad::Timer Timer;

    /*! Restarts timer
        \param[in, out] t timer to be restarted
     */ 
    inline static void restart(sad::Timer& t)
    {
        t.start();
    }
    /*! Returns elapsed time for interface in ms
        \param[in] t timer
        \return elapsed time in milliseconds
     */
    inline static double elapsed(sad::Timer& t)
    {
        t.stop();
        return t.elapsed();
    }
};


}

}