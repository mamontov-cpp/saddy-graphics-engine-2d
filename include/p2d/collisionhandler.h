/*! \file collisionhandler.h
	\author HiddenSeeker

	Describes a collision handler, used to deal with collision actions
 */
#pragma once
#include "collisionevent.h"
#include "body.h"

namespace sad
{

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
	virtual void invoke(const sad::p2d::BasicCollisionEvent & ev) = 0;
	/*! Could be inherited
	 */
	virtual ~BasicCollisionHandler();
};

/*! A basic collision handler for invoking events, calling a free function
 */
template<typename T1, typename T2>
class FunctionCollisionHandler  : public sad::p2d::BasicCollisionHandler
{
public:
	  inline FunctionCollisionHandler( void (*p)(const sad::p2d::CollisionEvent<T1, T2> & ev) )
	  : m_p(p)
	  {
	  }
	  /*! Calls a function for basic collision event
		  \param[in] ev event
	   */ 
	  virtual void invoke(const sad::p2d::BasicCollisionEvent & ev)
	  {
		  if (ev.m_object_1 != NULL && ev.m_object_2 != NULL
			  // We must check those, because, when multiple collisions are found with same object
			  // object and his userObject can be removed. So we MUST check userObject to make 
			  // sure not to work with deleted metadata
			  && ev.m_object_1->userObject() != NULL
			  && ev.m_object_2->userObject() != NULL)
		  {
			  p2d::CollisionEvent<T1, T2> e;
			  e.sad::p2d::BasicCollisionEvent::m_object_1 = ev.m_object_1;
			  e.sad::p2d::BasicCollisionEvent::m_object_2 = ev.m_object_2;
			  e.m_time = ev.m_time;
			  e.m_object_1 = sad::checked_cast<T1>(ev.m_object_1->userObject());
			  e.m_object_2 = sad::checked_cast<T2>(ev.m_object_2->userObject());
			  m_p(e);
		  }
	  }
private:
	  void (*m_p)(const sad::p2d::CollisionEvent<T1, T2> & ev);
};

/*! A specific collision handler for invoking events, for working with bodies
 */
template<>
class FunctionCollisionHandler<p2d::Body, p2d::Body> : public sad::p2d::BasicCollisionHandler
{
public:
	  inline FunctionCollisionHandler( void (*p)(const sad::p2d::BasicCollisionEvent & ev) )
	  : m_p(p)
	  {
	  }
	  /*! Calls a function for basic collision event
		  \param[in] ev event
	   */ 
	  virtual void invoke(const sad::p2d::BasicCollisionEvent & ev);
private:
	  void (*m_p)(const sad::p2d::BasicCollisionEvent & ev);
};


/*! A basic collision handler for invoking events, calling a method
 */
template<typename _Class, typename T1, typename T2>
class MethodCollisionHandler  : public sad::p2d::BasicCollisionHandler
{
public:
	  inline MethodCollisionHandler( _Class * o, void (_Class::*p)(const sad::p2d::CollisionEvent<T1, T2> & ev) )
	  : m_o(o), m_p(p)
	  {
	  }
	  /*! Calls a method for basic collision event
		  \param[in] ev event
	   */ 
	  virtual void invoke(const sad::p2d::BasicCollisionEvent & ev)
	  {
		  if (ev.m_object_1 != NULL && ev.m_object_2 != NULL
			  // We must check those, because, when multiple collisions are found with same object
			  // object and his userObject can be removed. So we MUST check userObject to make 
			  // sure not to work with deleted metadata
			  && ev.m_object_1->userObject() != NULL
			  && ev.m_object_2->userObject() != NULL)
		  {
			  p2d::CollisionEvent<T1, T2> e;
			  e.m_time = ev.m_time;
			  e.sad::p2d::BasicCollisionEvent::m_object_1 = ev.m_object_1;
			  e.sad::p2d::BasicCollisionEvent::m_object_2 = ev.m_object_2;
			  e.m_object_1 = sad::checked_cast<T1>(ev.m_object_1->userObject());
			  e.m_object_2 = sad::checked_cast<T2>(ev.m_object_2->userObject());
			  (m_o->*m_p)(e);
		  }
	  }
private:
	  _Class * m_o;
	  void (_Class::*m_p)(const sad::p2d::CollisionEvent<T1, T2> & ev);
};

/*! A basic collision handler for invoking events, calling a method
 */
template<typename _Class>
class MethodCollisionHandler<_Class, sad::p2d::Body, sad::p2d::Body>  
: public sad::p2d::BasicCollisionHandler
{
public:
	  inline MethodCollisionHandler( _Class * o, void (_Class::*p)(const sad::p2d::BasicCollisionEvent & ev) )
	  : m_o(o), m_p(p)
	  {
	  }
	  /*! Calls a method for basic collision event
		  \param[in] ev event
	   */ 
	  virtual void invoke(const sad::p2d::BasicCollisionEvent & ev)
	  {
		  (m_o->*m_p)(ev);
	  }
private:
	  _Class * m_o;
	  void (_Class::*m_p)(const sad::p2d::BasicCollisionEvent& ev);
};

}

}
