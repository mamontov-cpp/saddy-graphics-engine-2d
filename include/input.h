/*! \file    input.h
    \author  HiddenSeeker

	Definition of main input
*/
#include "templates/hhash.hpp"
#include "os/mutex.h"
#pragma once

#ifdef WIN32
#define _WIN32_WINDOWS 0x0501 // Set windows version to obtain control over mouse wheel.
#endif

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
			 bool   alt;   //!< Whether alt is pressed
			 bool   shift; //!< Whether shift is pressed
			 bool   capslock; //!< Whether caps lock is toggled
			 bool   ctrl;        //!< Whether ctrl is pressed   
			 float delta;  //!< Delta for wheel coordinate. It's will be filled under Windows. On X11, you can't see it due a poor protocol specidfication, so on wheelup  event it will be filled with 1, on wheeldown -1.

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
	/*! Resize event for window
	*/
	class ResizeEvent
	{
	 public:
			unsigned int old_height;  //!< Old height
			unsigned int old_width;   //!< Old width
			unsigned int new_height;  //!< New height
			unsigned int new_width;   //!< New width
	        //! Default constructor
			inline ResizeEvent(unsigned int oh, unsigned int ow, unsigned int nh, unsigned int nw)
			{ old_height=oh; old_width=ow; new_height=nh; new_width=nw;  }
			inline ~ResizeEvent() {}
	};
	namespace misc
	{
		template<typename T> 
		union unicaster //!< Unsafe caster union
		{
			T * from;
			void * to;
		};
		template<typename T> void * void_cast(T * from) { unicaster<T> u; u.from=from; return u.to; }
        template<typename T> T    * t_cast(void * from) { unicaster<T> u; u.to=from; return u.from; }
		/*! Deletes an object
		*/
		template<typename T> void destroy(void * m)
		{
		  delete t_cast<T*>(m);	
		}
		/*! Invoke trait for working
		*/
		template<typename T, typename EventType> void invoke(void * m,const EventType & ev)
		{
          (*(t_cast<T*>(m)))(ev);
		}
		/*! Instantiations for template functors
		*/
		template<typename EventType>
		void invoke_ptr (void * m,const EventType & ev );
	}
	
	/*! Handler functor function, used for handling objects
	*/
	template<typename EventType>
	class BasicEventHandler
	{
	 private:
		      void * m_functor;                             //!< What should we invoke
			  void (*m_invoke)(void *,const EventType &);  //!< Invokation function
			  void (*m_destroy)(void *);                    //!< Destroying function
	 public:
		    /*! Empty constructor
			*/
			BasicEventHandler();
			/*! Captures a functor
			*/
            template<typename T>
			BasicEventHandler(T * functor);
		     
			/*! Captures a simple functor
			*/
			BasicEventHandler( void (*functor)(const EventType &) );
			/*! Invokes a functor with event
			     \param[in] o event
			*/
			virtual void operator()(const EventType & o);
			/*! Destructor
			*/
			virtual ~BasicEventHandler();
			/*! Is it is empty
			*/
			inline bool empty() { return !m_functor; }
	};
	/*! Common input handler
	*/
	typedef BasicEventHandler<sad::Event> EventHandler;
	/*! Resize event handler
	*/
	typedef BasicEventHandler<sad::ResizeEvent> ResizeEventHandler;

	/*! \class CountedTask
	    Declares a task, that must be after n specific calls of tryPerform() function
	*/
	class CountableTask
	{
	 private:
		      int m_task_time; //!< Determines amount of loops, when he must be run
			  void (*m_task)();  //!< Determines a task functions
	 public:
		      /*! Creates a new task
			  */
		      CountableTask(int time=1);
			  /*! Creates a new task with specific functions
				  \param[in] task task functions
				  \param[in] time amount of tryPerform() calls;
			  */
			  CountableTask(void (*m_task)(),int time=1);
			  /*! Tries to perform task
			      \return whether perform() is called
			  */
			  inline bool tryPerform() { --m_task_time; if (!m_task_time) perform(); return m_task_time==0; };
		      /*! Determines, that task must be performed
			  */
			  virtual void perform();
			  /*! Destructor
			  */
			  virtual ~CountableTask();
	};
	class Input
	{
	 private:

		      sad::EventHandler *  m_mousemove;   //!<  Move
			  sad::EventHandler *  m_mousedown;   //!<  Down
			  sad::EventHandler *  m_mouseclick;  //!<  Click
			  sad::EventHandler *  m_mouseup;     //!<  Up
			  sad::EventHandler *  m_mousewheel;  //!<  Wheel
			  sad::EventHandler *  m_dblclick;    //!<  Double click

			  sad::EventHandler *  m_keyup;       //!<  Key up
			  sad::EventHandler *  m_keydown;     //!<  Key down

			  hst::vector<sad::ResizeEventHandler *> m_resizelisteners;  //!< Handler for resize
			  hst::vector<bool>						 m_removelisteners;  //!< Whether we are going to kill listeners

			  hst::vector<CountableTask *>           m_postrender_tasks;  //!< Tasks for postrendering
			  hst::vector<CountableTask *>           m_prerender_tasks;   //!< Tasks for prerendering

			  hst::hash<int,sad::EventHandler*>  m_ups;  //!< Key up functors
			  hst::hash<int,sad::EventHandler*>  m_down; //!< Key down functors
			  
			  os::mutex            m_umutex; //!< up mutex binding
			  os::mutex            m_dmutex; //!< down mutex binding

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
			  /*! Invokes try perform and removes worked task
				  \param[in,out] v vector
			  */
			  void tryPerform(hst::vector<sad::CountableTask *> & v);
	 public:
		     /*! Returns an instance
			 */
		     static Input * inst();
		     /*! Destroys an instance
			 */
		     ~Input();

			  void setMouseMoveHandler(  sad::EventHandler * h);
			  void setMouseDownHandler(  sad::EventHandler * h);
              void setMouseClickHandler(  sad::EventHandler * h);
			  void setMouseUpHandler(   sad::EventHandler * h);
			  void setMouseDblClickHandler(  sad::EventHandler * h);
			  void setMouseWheelHandler(   sad::EventHandler * h);
				
			  void setKeyUpHandler(   sad::EventHandler * h);
			  void setKeyDownHandler(  sad::EventHandler * h);

			  void addResizeHandler(sad::ResizeEventHandler  * h,bool autoremove=true);
			  void removeResizeHandler(sad::ResizeEventHandler * h);

			  void bindKeyUp(int key, sad::EventHandler * h);
			  void bindKeyDown(int key, sad::EventHandler * h);
			
			  inline void bindKeyUp(int key, void (*h)(const sad::Event&) ) { bindKeyUp(key, new sad::EventHandler(h)); }
			  inline void bindKeyDown(int key, void (*h)(const sad::Event&) ) { bindKeyDown(key, new sad::EventHandler(h)); }
			
			  void postMouseMove(const sad::Event & ev);
			  void postMouseDown(const sad::Event & ev);
			  void postMouseClick(const sad::Event & ev);
			  void postMouseUp(const sad::Event & ev);
			  void postMouseDblClick(const sad::Event & ev);
			  void postMouseWheel(const sad::Event & ev);
			  void postKeyUp(const sad::Event & ev);
			  void postKeyDown(const sad::Event & ev);

			  void postResize(const sad::ResizeEvent & ev);

			  /*! Add a pre-render task
				  \param[in] tsk task
				  \param[in] time time for taks
			  */
			  inline void addPreRenderTask(void (*tsk)(),int time=1)  {addPreRenderTask(new sad::CountableTask(tsk,time)); }
			  /*! Add a post-render task
				  \param[in] tsk task
				  \param[in] time time for taks
			  */
			  inline void addPostRenderTask(void (*tsk)(),int time=1)  {addPostRenderTask(new sad::CountableTask(tsk,time)); }
			  /*! Adds a pre-render task	
				  \param[in] tsk new task
			  */
			  inline void addPreRenderTask(sad::CountableTask * tsk) {m_prerender_tasks<<tsk; }
			  /*! Adds a post-render task
				  \param[in] tsk new task
			  */
			  inline void addPostRenderTask(sad::CountableTask * tsk) {m_postrender_tasks<<tsk; }
			  /*! Posts prerender time
			  */
			  inline void preRender() { tryPerform(m_prerender_tasks); }
			  /*! Posts postrender time
			  */
			  inline void postRender() { tryPerform(m_postrender_tasks); }
			  /*! Detects, whether we are not watching for mouse tracking
			      \{
			  */
			  inline bool areMovingNotTracked() { if (!m_mousemove)  return true; return m_mousemove->empty(); } 
			  inline bool areClickNotTracked()  { if (!m_mouseclick)  return true; return m_mouseclick->empty(); }
			  inline bool areDblClickNotTracked() { if (!m_dblclick)  return true; return m_dblclick->empty(); }
			  inline bool areWheelNotTracked()   { if (!m_mousewheel) return true; return m_mousewheel->empty(); }
			  inline bool areUpNotTracked()      { if (!m_mouseup)   return true; return m_mouseup->empty(); }
			  inline bool areDownNotTracked()    { if (!m_mousedown) return true; return m_mousedown->empty(); }
			  inline bool areResizeNotTracked()     { return m_resizelisteners.count()==0; }
			  /*!  \}
			  */
	};
}


//========Source code goes here==============
namespace sad
{
 namespace misc
 {
  template<typename EventType>
  void invoke_ptr (void * m,const EventType & ev )
  {
    union 
    {
	  void (*f1)(const EventType&);
	  void * f2;
    } u;
    u.f2=m;
   (u.f1)(ev);
  }
 }

 template<typename EventType>
 template<typename T>
 BasicEventHandler<EventType>::BasicEventHandler(T * functor)
 {
	 m_functor=sad::misc::void_cast(functor);
	 m_invoke=sad::misc::invoke<T>;
	 m_destroy=sad::misc::destroy<T>;
 }
 template<typename EventType>
 BasicEventHandler<EventType>::BasicEventHandler()
 {
	m_functor=NULL;
	m_invoke=NULL;
	m_destroy=NULL;
 }
 template<typename EventType>
 BasicEventHandler<EventType>::~BasicEventHandler()
 {
	if (m_destroy)
		m_destroy(m_functor);
 }
 template<typename EventType>
 BasicEventHandler<EventType>::BasicEventHandler( void (*functor)(const EventType &) )
 {
	union 
    {
	  void (*f1)(const EventType&);
	  void * f2;
    } u;
	u.f1=functor;
	m_functor=u.f2;
	m_invoke=sad::misc::invoke_ptr<EventType>;
	m_destroy=NULL;
 }

 template<typename EventType>
 void BasicEventHandler<EventType>::operator()(const EventType & o)
 {
	if (m_invoke)
		m_invoke(m_functor,o);
 }

}

