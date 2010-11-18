/*! \file    input.h
    \author  HiddenSeeker

	Definition of main input
*/
#include "hhash.hpp"
#pragma once

#define MOUSE_BUTTON_LEFT   -16535
#define MOUSE_BUTTON_RIGHT  -16524

namespace sad
{
	/*! Event, that was passed on window 
	*/
	class Event
	{
	 public:
		     float x;      //!< X coordinate
			 float y;      //!< Y coordinate
			 float z;      //!< Z coordinate
			 int   key;    //!< Pressed key (or mouse button)
			 float delta;  //!< Delta for wheel coordinate

			 /*! Key event
			 */
			 Event(float x,float y,float z,int key);
			 /*! Mouse wheel 
			 */
			 Event(float x,float y,float z,float delta);
			 /*! Key press
			 */
			 Event(int key);
			 /*! Empty event
			 */
			 Event();
			 /*! Destructor
			 */
			 ~Event();
	}; 
	namespace misc
	{
		template<typename T> 
		union unicaster //!< Unsafe caster union
		{
			T * from;
			void * to;
		};
		template<typename T> void * void_cast(T * from) { unicaster u; u.from=from; return u.to; }
        template<typename T> T    * t_cast(void * from) { unicaster u; u.to=to; return u.from; }
		/*! Deletes an object
		*/
		template<typename T> void destroy(void * m)
		{
		  delete t_cast<T*>(m);	
		}
		/*! Invoke trait for working
		*/
		template<typename T> void invoke(void * m,const sad::Event & ev)
		{
          (*(t_cast<T*>(m)))(ev);
		}
		/*! Instantiations for template functors
		*/
		void invoke_ptr (void * m,const sad::Event & ev );
	}
	
	/*! Handler functor function, used for handling objects
	*/
	class EventHandler
	{
	 private:
		      void * m_functor;                             //!< What should we invoke
			  void (*m_invoke)(void *,const sad::Event &);  //!< Invokation function
			  void (*m_destroy)(void *);                    //!< Destroying function
	 public:
		    /*! Empty constructor
			*/
			EventHandler();
			/*! Captures a functor
			*/
            template<typename T>
			EventHandler(T * functor);
		     
			/*! Captures a simple functor
			*/
			EventHandler( void (*functor)(const sad::Event &) );
			/*! Invokes a functor with event
			     \param[in] o event
			*/
			void operator()(const sad::Event & o);
			/*! Destructor
			*/
			~EventHandler();
	};

	class Input
	{
	 private:

		      sad::EventHandler   m_mousemove;   //!<  Move
			  sad::EventHandler   m_mousedown;   //!<  Down
			  sad::EventHandler   m_mouseclick;  //!<  Click
			  sad::EventHandler   m_mouseup;     //!<  Up
			  sad::EventHandler   m_mousewheel;  //!<  Wheel
			  sad::EventHandler   m_dblclick;    //!<  Double click

			  sad::EventHandler   m_keyup;       //!<  Key up
			  sad::EventHandler   m_keydown;     //!<  Key down

			  hst::hash<int,sad::EventHandler>  m_ups;  //!< Key up functors
			  hst::hash<int,sad::EventHandler>  m_down; //!< Key down functors

		      Input & operator=(const Input &);
		      Input(const Input&);
		      /*! Default constructor
			  */
			  Input();
			  /*! Instance */
			  static Input * m_instance;
			  /*! Destroys an instance
			  */
			  static void freeInst();
	 public:
		     /*! Returns an instance
			 */
		     static Input * inst();
		     /*! Destroys an instance
			 */
		     ~Input();

			  void setMouseMoveHandler(const sad::EventHandler & h);
			  void setMouseDownHandler(const sad::EventHandler & h);
              void setMouseClickHandler(const sad::EventHandler & h);
			  void setMouseUpHandler(const sad::EventHandler & h);
			  void setMouseDblClickHandler(const sad::EventHandler & h);
			  void setMouseWheelHandler(const sad::EventHandler & h);
				
			  void setKeyUpHandler(const sad::EventHandler & h);
			  void setKeyDownHandler(const sad::EventHandler & h);

			  void bindKeyUp(int key,const sad::EventHandler & h);
			  void bindKeyDown(int key,const sad::EventHandler & h);
			
			  void postMouseMove(const sad::Event & ev);
			  void postMouseDown(const sad::Event & ev);
			  void postMouseClick(const sad::Event & ev);
			  void postMouseUp(const sad::Event & ev);
			  void postMouseDblClick(const sad::Event & ev);
			  void postMouseWheel(const sad::Event & ev);
			  void postKeyUp(const sad::Event & ev);
			  void postKeyDown(const sad::Event & ev);
	};
}


//====Source code====
template<typename T>
sad::EventHandler::EventHandler(T * functor)
{
	m_functor=sad::misc::void_cast(functor);
	m_invoke=sad::misc::invoke<T>;
	m_destroy=sad::misc::destroy<T>;
}
