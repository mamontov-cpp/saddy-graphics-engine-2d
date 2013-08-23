/*! \file periodicalevent.h
	\autor HiddenSeeker

	Describes a period event, which performs in specified interval
 */
#include <time.h>
#include <input.h>
#pragma once

/*! An event, that is performed if last time shot is less than specified interval
 */
class PeriodicalEvent
{
 protected:
	 bool  m_enabled;   //!< Whether periodical event is enabled
	 double m_interval; //!< Interval of perforiming in milliseconds
	 clock_t m_last_shot; //!< Last time, when event worked
	 /*! Performs an actions inside of specified event
	  */
	 virtual void perform() = 0;
 public:
	 /*! Sets default interval - 50ms
	  */
	 PeriodicalEvent();
	 /*! Sets interval
		 \param[in] interval
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
	 virtual ~PeriodicalEvent();
};

/*! A special case of repeating task, which invokes a periodical event
 */
class TimePeriodicalTask: public sad::RepeatingTask
{
  protected:
	  PeriodicalEvent * m_event; //!< Inner event to be performed
  public:
	  /*! Created s periodical task with specified event
		  \param[in] e event
	   */
	  TimePeriodicalTask(PeriodicalEvent * e);
	  /*! Returns an event
		  \return event
	   */
	  PeriodicalEvent * e();
	  /*! Calls try perform upon event
	   */
	  virtual void perform();
	  /*! Destroys event
	   */
	  ~TimePeriodicalTask();
	  /*! Creates new periodical task for specified event
	      \return task
	   */
	  template<typename _Event>
	  static TimePeriodicalTask * create()
	  {
		  return new TimePeriodicalTask( new _Event() );
	  }
};
