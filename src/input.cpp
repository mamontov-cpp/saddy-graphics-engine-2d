#include "input.h"
#include "renderer.h"
#include "keycodes.h"
#include <stdlib.h>

static void cleanup_ptr_vector(sad::Vector<sad::CountableTask *> & ff);

sad::Event::Event() : x(0), y(0), z(0), key(0), delta(0)
{
}
sad::Event::~Event() 
{ 

}
sad::Event::Event(int _key) : key(_key) 
{ 

}
sad::Event::Event(float _x, float _y, float _z, int _key) 
: x(_x), y(_y), z(_z), key(_key)
{

}
sad::Event::Event(float _x, float _y, float _z, float _delta)
: x(_x), y(_y), z(_z), delta(_delta)
{

}

sad::Input * sad::Input::m_instance=NULL;

sad::Input::Input()
{
	m_mousemove=NULL;
	m_mousedown=NULL;
	m_mouseclick=NULL;
	m_mouseup=NULL;
	m_mousewheel=NULL;
	m_dblclick=NULL;
	m_keyup=NULL;
	m_keydown=NULL;
	m_quit = NULL;
}
sad::Input::~Input()
{
	const size_t handlerlistlength = 9;
	sad::EventHandler * handlerlist[handlerlistlength] = {
		m_mousemove,
		m_mousedown,
		m_mouseclick,
		m_mouseup,
		m_mousewheel,
		m_dblclick,
		m_keyup,
		m_keydown,
		m_quit
	};
	for(size_t i = 0; i < handlerlistlength; i++)
	{
		delete handlerlist[i];
	}
	for (unsigned int i=0;i<m_resizelisteners.count();i++)
	{
		if (m_removelisteners[i]) delete m_resizelisteners[i];
	}

	cleanup_ptr_vector(m_postrender_tasks);
	cleanup_ptr_vector(m_prerender_tasks);

	m_resizelisteners.clear();
	m_removelisteners.clear();
	for (sad::Hash<int,sad::EventHandler *>::iterator it=m_ups.begin();it!=m_ups.end();++it)
	{
		delete it.value();
	}
    for (sad::Hash<int,sad::EventHandler *>::iterator it=m_down.begin();it!=m_down.end();++it)
	{
		delete it.value();
	}
}


sad::Input * sad::Input::ref()
{
	return sad::Renderer::ref()->controls();
}

#define DEFINE_SETTER_FOR(METHOD, PROPERTYNAME)                         \
void sad::Input:: METHOD (sad::EventHandler * h)                        \
{                                                                       \
	if (PROPERTYNAME) delete PROPERTYNAME;                              \
    PROPERTYNAME=h;                                                     \
}                                                                       \

DEFINE_SETTER_FOR(setMouseMoveHandler,m_mousemove)
DEFINE_SETTER_FOR(setMouseDownHandler,m_mousedown)
DEFINE_SETTER_FOR(setMouseClickHandler,m_mouseclick)
DEFINE_SETTER_FOR(setMouseUpHandler,m_mouseup)
DEFINE_SETTER_FOR(setMouseDblClickHandler,m_dblclick)
DEFINE_SETTER_FOR(setMouseWheelHandler,m_mousewheel)
DEFINE_SETTER_FOR(setKeyUpHandler,m_keyup)
DEFINE_SETTER_FOR(setKeyDownHandler,m_keydown)
DEFINE_SETTER_FOR(setQuitHandler,m_quit)

#undef TEMP_DEF

#define TEMP_DEF(X,Y)                                \
void sad::Input::  X (const sad::Event & ev)          \
{                                                    \
  if ( Y )                                           \
    (*  Y)(ev);                                        \
}                                                    \

TEMP_DEF(postMouseMove,m_mousemove)
TEMP_DEF(postMouseDown,m_mousedown)
TEMP_DEF(postMouseClick,m_mouseclick)
TEMP_DEF(postMouseUp,m_mouseup)
TEMP_DEF(postMouseDblClick,m_dblclick)
TEMP_DEF(postMouseWheel,m_mousewheel)

#undef TEMP_DEF

void sad::Input::bindKeyUp(int key, sad::EventHandler * h)
{
	m_umutex.lock();

	if (h)
	{
		if (m_ups.contains(key)) delete m_ups[key];
	     m_ups.insert(key,h);
	}
	else
		 m_ups.remove(key);
	
	m_umutex.unlock();
}
void sad::Input::bindKeyDown(int key, sad::EventHandler * h)
{
	m_dmutex.lock();

	if (h)
	{
		if (m_down.contains(key)) delete m_down[key];
		m_down.insert(key,h);
	}
	else
		m_down.remove(key);
	
	m_dmutex.unlock();
}
void sad::Input::postKeyUp(const sad::Event & ev)
{
	m_umutex.lock();

	if (m_ups.contains(ev.key))
		(*m_ups[ev.key])(ev);

	m_umutex.unlock();

	if (!m_keyup) return;
	(*m_keyup)(ev);
}
void sad::Input::postKeyDown(const sad::Event & ev)
{
	m_dmutex.lock();

	if (m_down.contains(ev.key))
		(*m_down[ev.key])(ev);
	
	m_dmutex.unlock();

	if (!m_keydown) return;
	(*m_keydown)(ev);
}

void sad::Input::addResizeHandler (sad::ResizeEventHandler * h,bool autoremove)          
{                                                    
	m_resizelisteners<<h;
	m_removelisteners<<autoremove;
}                                                    
void sad::Input::removeResizeHandler(sad::ResizeEventHandler * h)
{
	for (unsigned int i=0;i<m_resizelisteners.count();i++)
	{
	 if (m_resizelisteners[i]==h)
	 {
	  if (m_removelisteners[i]) delete m_resizelisteners[i];
      m_removelisteners.removeAt(i);
	  m_resizelisteners.removeAt(i);
	  --i;
	 }
	}
}
void sad::Input::postResize (const sad::ResizeEvent & ev)         
{                                                    
  for (unsigned int i=0;i<m_resizelisteners.count();i++)
	  (*m_resizelisteners[i])(ev);
}                                                    

sad::CountableTask::CountableTask(int time)
{
	m_task=NULL;
	m_task_time=time;
}
sad::CountableTask::CountableTask(void (*task)(),int time)
{
	m_task=task;
	m_task_time=time;
}
void sad::CountableTask::perform() 
{
	if (m_task)
		m_task();
}
bool sad::CountableTask::tryPerform()
{
  --m_task_time; if (!m_task_time) perform(); return m_task_time==0; 
}

sad::CountableTask::~CountableTask() {}

bool sad::RepeatingTask::tryPerform()
{
	perform();
	return !m_alive;
}
sad::RepeatingTask::~RepeatingTask() {}

static void cleanup_ptr_vector(sad::Vector<sad::CountableTask *> & ff)
{
	for (unsigned int i=0;i<ff.count();i++)
		delete ff[i];
	ff.clear();
}

void sad::Input::tryPerform(sad::Vector<sad::CountableTask *> & v)
{
	for (unsigned int i=0;i<v.count();i++)
	{
		if (v[i]->tryPerform())
		{
			delete v[i];
			v.removeAt(i);
			--i;
		}
	}
}

void sad::Input::postQuit()
{
	if (m_quit!=NULL)
		(*m_quit)(sad::Event());
}
