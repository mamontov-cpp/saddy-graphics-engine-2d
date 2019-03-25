/*! \file pausabletimer.h
    

    Defines a timer, which could be paused
 */
#pragma once
#include "timer.h"

namespace sad
{

/*! A timer, which could be paused and resumed. 
    The time, between pause() and resume() calls will not be counted in elapsed
 */
class PausableTimer
{
public:
    /*! Constructs default pausable timer
     */
    PausableTimer();
    /*! Implemented mainly for safety reasons, doesn't do anything
     */
    ~PausableTimer();
    /*! Starts new iteration of timer, erasing local elapsed time
     */
    void start();
    /*! Stops timer, ending iteration for timer
     */
    void stop();
    /*! Pauses timer, saving local elapsed time
     */
    void pause();
    /*! Resumes timer, working to next iteration
     */
    void resume();
    /*! Returns elapsed time, between calls of sad::Timer::start() and
        sad::Timer::stop() using high-perfomance counter, if possible. For
        some chipsets could fall to wallclock time, since correct information
        won't be available
        \return time in milliseconds
     */
    double elapsed() const;
    /*! Whether timer is running
        \return whether timer is running
     */
    inline bool running() const { return m_running; }
    /*! Whether timer is paused
        \return whether timer is paused
     */
    inline bool paused() const { return !m_running; }
protected:
    /*! Whether timer is running
     */
    bool m_running;
    /*! A local elapsed time, used for pause
     */
    double m_elapsed;
    /*! A local timer for measuring time
     */
    sad::Timer m_timer;
};

}
