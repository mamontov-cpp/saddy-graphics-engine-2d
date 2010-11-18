#include "input.h"
#include <stdlib.h>

sad::EventHandler::EventHandler()
{
	m_functor=NULL;
	m_invoke=NULL;
	m_destroy=NULL;
}

sad::EventHandler::~EventHandler()
{
	if (m_destroy)
		m_destroy(m_functor);
}

void sad::misc::invoke_ptr (void * m,const sad::Event & ev )
{
  union 
  {
	  void (*f1)(const sad::Event&);
	  void * f2;
  } u;
  u.f2=m;
  u.f1(ev);
}
sad::EventHandler::EventHandler( void (*functor)(const sad::Event &) )
{
	union 
    {
	  void (*f1)(const sad::Event&);
	  void * f2;
    } u;
	u.f1=functor;
	m_functor=u.f2;
	m_invoke=sad::misc::invoke_ptr;
	m_destroy=NULL;
}
void sad::EventHandler::operator()(const sad::Event & o)
{
	if (m_invoke)
		m_invoke(m_functor,o);
}

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
}
sad::Input::~Input()
{
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
void sad::Input::##X(const sad::EventHandler & h)    \
{                                                    \
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

#undef

#define TEMP_DEF(X,Y)                                \
void sad::Input::##X(const sad::Event & ev)          \
{                                                    \
  ##Y(ev);                                           \
}                                                    \

TEMP_DEF(postMouseMove,m_mousemove)
TEMP_DEF(postMouseDown,m_mousedown)
TEMP_DEF(postMouseClick,m_mouseclick)
TEMP_DEF(postMouseUp,m_mouseup)
TEMP_DEF(postMouseDblClick,m_dblclick)
TEMP_DEF(postMouseWheel,m_mousewheel)

#undef TEMP_DEF

void sad::Input::bindKeyUp(int key,const sad::EventHandler & h)
{
	m_ups.insert(key,h);
}
void sad::Input::bindKeyDown(int key,const sad::EventHandler & h)
{
	m_down.insert(key,h);
}
void sad::Input::postKeyUp(const sad::Event & ev)
{
	if (m_ups.contains(ev.key))
		m_ups[ev.key](ev);
	m_keyup(ev);
}
void sad::Input::postKeyDown(const sad::Event & ev)
{
	if (m_down.contains(ev.key))
		m_down[ev.key](ev);
	m_keydown(ev);
}