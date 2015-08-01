/*! \file force.h
	

	Describes a force, acting on object
 */
#pragma once

#include "vector.h"
#include "tickable.h"

#include "../sadvector.h"
#include "../fuzzyequal.h"

namespace sad
{

namespace p2d
{
/*! A class for getting default value for a tickable state
 */
template<>
class TickableDefaultValue<p2d::Vector>
{
 public:
	 static inline p2d::Vector get() { return p2d::Vector(0, 0); }
	 static inline p2d::Vector zero() { return p2d::Vector(0, 0); }
};


/*! Describes a specific force with value type T.
 */
template<typename T>
class Force
{
public:
	 /*! Creates zero force
	  */
	 inline Force() { m_alive = true; m_value = TickableDefaultValue<T>::zero(); }
     /*! Could be inherited
      */ 
     virtual ~Force() { }
	 /*! Creates a force with specific value
		 \param[in] v value
	  */
	 inline Force(const T & v) : m_alive(true), m_value(v) {}
	 /*! Forces a container to remove this force
	  */
	 inline void die() { m_alive = true; }
	 /*! Whether force is alive
		 \return whether it's alive
	  */
	 inline bool isAlive() const { return m_alive; }
     /*! Returns a value
		 \return value of force
	  */
	 virtual const T & value() const { return  m_value; }
     /*! Sets a value for force
		 \param[in] value a new value of force
	  */
	 virtual void setValue(const T & value) { m_value = value; }
	 /*! Steps a force to next iteration
	     \param[in] time a time step size
	  */
	 virtual void step(double time) {}
protected:
	 bool m_alive;  //!< If false, this force should be removed from container 
	 T    m_value;  //!< A value of force
};

/*! Describes a simple impulse force with value T
 */ 
template<typename T>
class ImpulseForce: public p2d::Force<T>
{
public:
	 /*! Creates zero force
	  */
	 inline ImpulseForce() : p2d::Force<T>() {  }
	 /*! Creates a force with specific value
		 \param[in] v value
	  */
	 inline ImpulseForce(const T & v) : p2d::Force<T>(v) {}

	 /*! Steps a force to next iteration
	     \param[in] time a time step size
	  */
	 virtual void step(double time) { this->die(); }
};


typedef p2d::Force<p2d::Vector>        TangentialForce;
typedef p2d::ImpulseForce<p2d::Vector> TangentialImpulseForce;


/*! Describes an acting forces on body
 */
template<
typename T
>
class ActingForces
{
public:
	 ActingForces() {}
	 /*! Force container owns all forces, belonging to it
	  */
	 virtual ~ActingForces() { clear(); }
	 /*! Immediately adds new force to container
		 \param[in] force new force
	  */
	 void add(p2d::Force<T> * force) { m_forces.push_back(force); }
	 /*! Schedules adding a force to container
	     \param[in] force new force
	  */
	 void scheduleAdd(p2d::Force<T> * force)  { m_queued << new ScheduledAdd(force); }
	 /*! Schedules adding a force to container
	     \param[in] force new force
		 \param[in] time a current time
	  */
	 void scheduleAdd(p2d::Force<T> * force, double time)  { m_queued << new ScheduledAddAt(time, force); }
	 /*! Clears a force container
	  */
	 void clear() 
	 {
		for(size_t i = 0; i < m_forces.size(); i++)
		{
			delete m_forces[i];
		}
		for(size_t i = 0; i < m_queued.size(); i++)
		{
			delete m_queued[i];
		}
	 }
	 /*! Steps an acting forces on time
		 \param[in] time a specific time
	  */
	 void step(double time)
	 {
		  for(size_t i = 0; i < m_forces.size(); i++)
		  {
			  p2d::Force<T> * p = m_forces[i];
			  p->step(time);
			  if (p->isAlive() == false)
			  {
				  delete m_forces[i];
				  m_forces.removeAt(i);
				  --i;
			  }
		  }
		  for(size_t i = 0; i < m_queued.size(); i++)
		  {
			  Command * c = m_queued[i];
			  if (c->perform(time, this))
			  {
				  delete m_queued[i];
				  m_queued.removeAt(i);
				  --i;
			  }
		  }
	 }
	 /*! Computes a value for force
		 \param[in] accumulator an accumulator
	  */
	 void value(T & accumulator) const
	 {
		 for(size_t i = 0; i < m_forces.size(); i++)
		 {
			 accumulator += m_forces[i]->value();
		 }
	 }
	 /*! Whether container has a forces
		 \return whether container has a forces
	  */
	 bool hasForces() const { return m_forces.size() != 0; }
protected:
	 /*! A queued command for performing of data
	  */
	 class Command
	 {
	  public:
		  /*! Performs a command on container
			  \param[in] time current time step size
			  \param[in] container a container
			  \return whether it was performed
		   */
		  virtual bool perform(double time, ActingForces<T> * container);

		  virtual ~Command() {}
	 };
	 /*! A sheduled adding, without time checking
	  */
	 class ScheduledAdd
	 {
	  protected:
		  p2d::Force<T> * m_force; //!< A force
	  public:
		  ScheduledAdd(p2d::Force<T> * f) : m_force(f) {}
		  /*! Adds a force to container
			  \param[in] time current time step size
			  \param[in] container a container
			  \return whether it was performed
		   */
		  virtual bool perform(double time, ActingForces<T> * container)
		  {
			  container->add(m_force);
			  m_force = NULL;
			  return true;
		  }
		  virtual ~ScheduledAdd() { delete m_force; }
	 };
	 /*! A sheduled adding, without time checking
	  */
	 class ScheduledAddAt
	 {
	  protected:
		  p2d::Force<T> * m_force; //!< A force
		  double  m_time;          //!< A time, when should be added a force  
	  public:
		  ScheduledAddAt(p2d::Force<T> * f) : m_force(f) {}
		  /*! Adds a force to container
			  \param[in] time current time step size
			  \param[in] container a container
			  \return whether it was performed
		   */
		  virtual bool perform(double time, ActingForces<T> * container)
		  {
			  if (time > m_time || ::sad::is_fuzzy_equal(time, m_time))
			  {
		          container->add(m_force);
			      m_force = NULL;
				  return true;
			  }
			  return false;
		  }
		  virtual ~ScheduledAddAt() { delete m_force; }
	 };
	 typedef p2d::Force<T> * force_t;
	 typedef typename p2d::ActingForces<T>::Command * command_t;
protected:
	 sad::Vector< force_t >    m_forces;                       //!< A forces list, acting on body
	 sad::Vector< command_t >  m_queued;                       //!< A queued list of commands
};


typedef p2d::ActingForces<p2d::Vector> TangentialActingForces;
}

}
