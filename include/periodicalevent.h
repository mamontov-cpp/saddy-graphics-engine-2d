/*! \file periodicalevent.h
    

    Describes a period event, which performs in specified interval
 */
#pragma once
#include "pipeline/pipelineprocess.h"
#include "timer.h"

namespace sad
{

/*! An special event, which is performed periodically with specified interval.
    Interval testing is done via sad::PeriodicalEvent::tryPerform(). Override
    sad::PeriodicalEvent::perform() to specify action, that need to be done
 */
class PeriodicalEvent
{
public:
    /*! Sets default interval - 50ms
     */
    PeriodicalEvent();
    /*! Sets interval
        \param[in] interval interval in milliseconds
     */
    void setInterval(double interval);
    /*! Returns an interval
        \return interval for event
     */
    inline double interval() const { return m_interval; }
    /*! Tries to perform inner operation inside event
     */
    virtual void tryPerform();
    /*! Whether event performing is enabled
     */
    inline bool  enabled() const { return m_enabled; }
    /*! Disables event performing
     */
    void disable();
    /*! Enables event performing
     */
    void enable();
    /*! Kept for purpose of inheritance
     */
    virtual ~PeriodicalEvent();
protected:
    /*! Performs an action, when timer expires
     */
    virtual void perform() = 0;
    bool  m_enabled;   //!< Whether periodical event is enabled
    double m_interval; //!< Interval of performing in milliseconds
    sad::Timer m_timer;  //!< A timer
};

/*! A special case of process, which invokes a 
    sad::PeriodicalEvent inside. Can be used as process in pipeline
 */
class PeriodicalEventPollProcess: public sad::pipeline::AbstractProcess
{
protected:
    PeriodicalEvent * m_event; //!< Inner event to be performed
public:
    /*! Created s periodical task with specified event
        \param[in] e event
     */
    PeriodicalEventPollProcess(PeriodicalEvent * e);
    /*! Returns an event
        \return event
     */
    PeriodicalEvent * e();
    /*! Sets a periodical event
        \param[in] e event
     */
    void setEvent(PeriodicalEvent * e);
    /*! Destroys event
     */
    virtual ~PeriodicalEventPollProcess() override;
    /*! Creates new periodical task for specified event
        \return task
     */
    template<typename _Event>
    static PeriodicalEventPollProcess * create()
    {
        return new PeriodicalEventPollProcess( new _Event() );
    }
protected:
    /*! Calls sad::PeriodicalEvent::tryPerform() if called
     */
    virtual void _process() override;
};

}
