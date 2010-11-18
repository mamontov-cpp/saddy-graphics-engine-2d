/*! \file    input.h
    \author  HiddenSeeker

	Definition of main input
*/
#include "hhash.hpp"
#pragma once

//KEY BINDINGS
#define KEY_ESC 257
#define KEY_F1  258
#define KEY_F2  259
#define KEY_F3  260
#define KEY_F4  261
#define KEY_F5  262
#define KEY_F6  263
#define KEY_F7  264
#define KEY_F8  265
#define KEY_F9  265
#define KEY_F10  266
#define KEY_F11  267
#define KEY_F12  268
#define KEY_PAUSE 269
#define KEY_INSERT 270
#define KEY_DELETE 271
#define KEY_LCONTROL 272
#define KEY_BACKSPACE 273
#define KEY_ENTER     274
#define KEY_SPACE        275
#define KEY_PRINTSCREEN  276
#define KEY_HOME         277
#define KEY_END          278
#define KEY_PGUP         279
#define KEY_PGDOWN       280
#define KEY_TAB          281
#define KEY_CAPSLOCK     282
#define KEY_NUMLOCK      283
#define KEY_SCROLLOCK    284
#define KEY_CAPSLOCK     285
#define KEY_LSHIFT       286
#define KEY_LCTRL        287
#define KEY_WIN         288
#define KEY_LALT         289
#define KEY_RALT         290
#define KEY_RCTRL        291
#define KEY_RSHIFT       292
#define KEY_LEFT         293
#define KEY_UP           294
#define KEY_DOWN         295
#define KEY_RIGHT        296
#define KEY_APP          297
/*! Other keys are binded natively.
    Use default char values to bind it. Note,that they are binded in upper case
*/
#define MOUSE_BUTTON_LEFT   -16535
#define MOUSE_BUTTON_MIDDLE -16524
#define MOUSE_BUTTON_RIGHT  -16523

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
