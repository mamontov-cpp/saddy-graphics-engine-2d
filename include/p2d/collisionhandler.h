/*! \file collisionhandler.h
	\author HiddenSeeker

	Describes a collision handler, used to deal with collision actions
 */
#include "collisionevent.h"
#include "body.h"
#pragma once

namespace p2d
{
/*! A basic collision handler for invoking events
 */
class BasicCollisionHandler
{
public:
	/*! Calls a function for basic collision event
		\param[in] ev event
	 */ 
	virtual void invoke(const BasicCollisionEvent & ev) = 0;

	virtual ~BasicCollisionHandler();
};

/*! A basic collision handler for invoking events, calling a free function
 */
template<typename T1, typename T2>
class FunctionCollisionHandler  : public p2d::BasicCollisionHandler
{
  private:
	  void (*m_p)(const p2d::CollisionEvent<T1, T2> & ev);
  public:
	  inline FunctionCollisionHandler( void (*p)(const p2d::CollisionEvent<T1, T2> & ev) )
	  : m_p(p)
	  {
	  }
	  /*! Calls a function for basic collision event
		  \param[in] ev event
	   */ 
	  virtual void invoke(const BasicCollisionEvent & ev)
	  {
		  if (ev.m_object_1 != NULL && ev.m_object_2 != NULL)
		  {
			  p2d::CollisionEvent<T1, T2> e;
			  e.p2d::BasicCollisionEvent::m_object_1 = ev.m_object_1;
			  e.p2d::BasicCollisionEvent::m_object_2 = ev.m_object_2;
			  e.m_time = ev.m_time;
			  e.m_object_1 = hst::checked_cast<T1>(ev.m_object_1->userObject());
			  e.m_object_2 = hst::checked_cast<T2>(ev.m_object_2->userObject());
			  m_p(e);
		  }
	  }

};

/*! A specific collision handler for invoking events, for working with bodies
 */
template<>
class FunctionCollisionHandler<p2d::Body, p2d::Body> : public p2d::BasicCollisionHandler
{
  private:
	  void (*m_p)(const BasicCollisionEvent & ev);
  public:
	  inline FunctionCollisionHandler( void (*p)(const BasicCollisionEvent & ev) )
	  : m_p(p)
	  {
	  }
	  /*! Calls a function for basic collision event
		  \param[in] ev event
	   */ 
	  virtual void invoke(const BasicCollisionEvent & ev);
};


/*! A basic collision handler for invoking events, calling a method
 */
template<typename _Class, typename T1, typename T2>
class MethodCollisionHandler  : public p2d::BasicCollisionHandler
{
  private:
	  _Class * m_o;
	  void (_Class::*m_p)(const p2d::CollisionEvent<T1, T2> & ev);
  public:
	  inline MethodCollisionHandler( _Class * o, void (_Class::*p)(const p2d::CollisionEvent<T1, T2> & ev) )
	  : m_o(o), m_p(p)
	  {
	  }
	  /*! Calls a method for basic collision event
		  \param[in] ev event
	   */ 
	  virtual void invoke(const BasicCollisionEvent & ev)
	  {
		  if (ev.m_object_1 != NULL && ev.m_object_2 != NULL)
		  {
			  p2d::CollisionEvent<T1, T2> e;
			  e.m_time = ev.m_time;
			  e.p2d::BasicCollisionEvent::m_object_1 = ev.m_object_1;
			  e.p2d::BasicCollisionEvent::m_object_2 = ev.m_object_2;
			  e.m_object_1 = hst::checked_cast<T1>(ev.m_object_1->userObject());
			  e.m_object_2 = hst::checked_cast<T2>(ev.m_object_2->userObject());
			  (m_o->*m_p)(e);
		  }
	  }

};

/*! A basic collision handler for invoking events, calling a method
 */
template<typename _Class>
class MethodCollisionHandler<_Class, p2d::Body, p2d::Body>  : public p2d::BasicCollisionHandler
{
  private:
	  _Class * m_o;
	  void (_Class::*m_p)(const p2d::BasicCollisionEvent& ev);
  public:
	  inline MethodCollisionHandler( _Class * o, void (_Class::*p)(const p2d::BasicCollisionEvent & ev) )
	  : m_o(o), m_p(p)
	  {
	  }
	  /*! Calls a method for basic collision event
		  \param[in] ev event
	   */ 
	  virtual void invoke(const BasicCollisionEvent & ev)
	  {
		  (m_o->*m_p)(ev);
	  }

};

}
