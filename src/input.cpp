#include "input.h"
#include <stdlib.h>

static void cleanup_ptr_vector(hst::vector<sad::CountableTask *> & ff);

sad::Event::Event()
{
	x=0;
	y=0;
	z=0;
	key=0;
	delta=0;
}
sad::Event::~Event()
{

}
sad::Event::Event(int _key)
{
	this->key=_key;
}
sad::Event::Event(float _x, float _y, float _z, int _key)
{
	x=_x;
	y=_y;
	z=_z;
	key=_key;
}
sad::Event::Event(float _x, float _y, float _z, float _delta)
{
	x=_x;
	y=_y;
	z=_z;
	delta=_delta;
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
}
sad::Input::~Input()
{
#define DEL(X) if (X) delete X;
	DEL(m_mousemove);DEL(m_mousedown); DEL(m_mouseclick);
	DEL(m_mouseup);DEL(m_mousewheel);DEL(m_dblclick); DEL(m_keyup);
	DEL(m_keydown);
#undef DEL
	for (int i=0;i<m_resizelisteners.count();i++)
	{
		if (m_removelisteners[i]) delete m_resizelisteners[i];
	}

	cleanup_ptr_vector(m_postrender_tasks);
	cleanup_ptr_vector(m_prerender_tasks);

	m_resizelisteners.clear();
	m_removelisteners.clear();
	for (hst::hash<int,sad::EventHandler *>::iterator it=m_ups.begin();it!=m_ups.end();++it)
	{
		delete it.value();
	}
    for (hst::hash<int,sad::EventHandler *>::iterator it=m_down.begin();it!=m_down.end();++it)
	{
		delete it.value();
	}
}

void sad::Input::freeInst()
{
	delete m_instance;
}
sad::Input * sad::Input::inst()
{
	if (!m_instance)
	{
		m_instance = new Input();
		atexit(sad::Input::freeInst);
	}
	return m_instance;
}

#define TEMP_DEF(X,Y)                                \
void sad::Input:: X (sad::EventHandler * h)          \
{                                                    \
	if (Y) delete Y;                                 \
    Y=h;                                             \
}                                                    \

TEMP_DEF(setMouseMoveHandler,m_mousemove)
TEMP_DEF(setMouseDownHandler,m_mousedown)
TEMP_DEF(setMouseClickHandler,m_mouseclick)
TEMP_DEF(setMouseUpHandler,m_mouseup)
TEMP_DEF(setMouseDblClickHandler,m_dblclick)
TEMP_DEF(setMouseWheelHandler,m_mousewheel)
TEMP_DEF(setKeyUpHandler,m_keyup)
TEMP_DEF(setKeyDownHandler,m_keydown)

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
  for (int i=0;i<m_resizelisteners.count();i++)
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

sad::CountableTask::~CountableTask() {}

static void cleanup_ptr_vector(hst::vector<sad::CountableTask *> & ff)
{
	for (int i=0;i<ff.count();i++)
		delete ff[i];
	ff.clear();
}

void sad::Input::tryPerform(hst::vector<sad::CountableTask *> & v)
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
