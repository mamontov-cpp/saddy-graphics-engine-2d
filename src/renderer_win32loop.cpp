#include "renderer.h"
#include "input.h"
#include "3rdparty/format/format.h"


#ifdef WIN32


#include <windowsx.h>

#ifndef GET_KEYSTATE_WPARAM 
	#define GET_KEYSTATE_WPARAM(wParam)     (LOWORD(wParam))
#endif

#ifndef GET_KEYSTATE_LPARAM
	#define GET_KEYSTATE_LPARAM(lParam) (LOWORD(lParam))
#endif


static  hst::hash<HWND, sad::Renderer *> m_renderers;
static  os::mutex m_data;
void sad::Renderer::mainLoop()
{
 SL_CRITICAL(( fmt::Print("{0}") << 5 ));
 m_data.lock();
 if (m_renderers.contains(m_window.hWND)) 
 {
	m_renderers[m_window.hWND] =  this;
 }
 else
 {
	 m_renderers.insert(m_window.hWND, this);
 }
 m_data.unlock();

 int frames=0;
 m_fps=60;
 //bool isMessagePumpActive;
 MSG msg;

 m_running = true;											// Program Looping Is Set To TRUE
 m_window.active=true;
 this->setTimer();


 while (m_running)											// Loop Until WM_QUIT Is Received
 {					
  // Check For Window Messages
  if (PeekMessage (&msg, m_window.hWND, 0, 0, PM_REMOVE) != 0)
  {
     TranslateMessage(&msg);
	 // Check For WM_QUIT Message
	 if (msg.message != WM_QUIT)						// Is The Message A WM_QUIT Message?
	 {		 
	  DispatchMessage(&msg);
	 }
	 else											// Otherwise (If Message Is WM_QUIT)
	 {
		m_running = false;				// Terminate The Message Loop
	 }
   }
   else												// If There Are No Messages
   {
	  // Process Application Loop
	  frames++;
	  if (this->elapsedInMSeconds() >= 1000)
	  {
		  setFPS(frames);frames=0;this->setTimer();
	  }
	  //Update a window, if active
	  if (m_window.active)
	     update();
	  else
		  Sleep(0);
	  //Change scene, if need so
	  if (m_chscene) 
	  { setCurrentScene(m_chscene); m_chscene=NULL;}
   }
  }
 this->controls()->postQuit();
 m_window.active=false;
 this->releaseWindow();
}


static hst::hash<WPARAM,int> table;

static void table_init()
{
   for (WPARAM t=VK_F1;t<=VK_F12;t++)
   table.insert(t,VK_F12-VK_F1+KEY_F1+t);
   table.insert(VK_ESCAPE,KEY_ESC);
   table.insert(VK_PAUSE,KEY_PAUSE);
   table.insert(VK_INSERT,KEY_INSERT);
   table.insert(VK_SNAPSHOT,KEY_PRINTSCREEN);
   table.insert(VK_DELETE,KEY_DELETE);
   table.insert(VK_BACK,KEY_BACKSPACE);
   table.insert(VK_TAB,KEY_TAB);
   table.insert(VK_NUMLOCK,KEY_NUMLOCK);
   table.insert(VK_SCROLL,KEY_SCROLLOCK);
   table.insert(VK_CAPITAL,KEY_CAPSLOCK);
   table.insert(VK_LSHIFT,KEY_LSHIFT);
   table.insert(VK_LCONTROL,KEY_LCTRL);
   table.insert(VK_LWIN,KEY_WIN);
   table.insert(VK_LMENU,KEY_LALT);
   table.insert(VK_APPS,KEY_APP);
   table.insert(VK_SPACE,KEY_SPACE);
   table.insert(VK_RMENU,KEY_RALT);
   table.insert(VK_RCONTROL,KEY_RCTRL);
   table.insert(VK_LEFT,KEY_LEFT);
   table.insert(VK_RIGHT,KEY_RIGHT);
   table.insert(VK_UP,KEY_UP);
   table.insert(VK_DOWN,KEY_DOWN);
   table.insert(VK_RETURN,KEY_ENTER);
   table.insert(VK_RSHIFT,KEY_RSHIFT);
   table.insert(VK_HOME,KEY_HOME);
   table.insert(VK_END,KEY_END);
   table.insert(VK_PRIOR,KEY_PGUP);
   table.insert(VK_NEXT,KEY_PGDOWN);
}


POINT sad::Renderer::_toClient(POINT  _pnt)
{
	POINT pnt;
	pnt.x = _pnt.x;
	pnt.y = _pnt.y;
	RECT r;
	GetClientRect(ref()->m_window.hWND, &r);
	pnt.y = r.bottom - pnt.y;
	return pnt;
}

POINT sad::Renderer::_toClient(LPARAM lParam)
{
	POINT pnt;
	pnt.x = GET_X_LPARAM(lParam);
	pnt.y = GET_Y_LPARAM(lParam);
	return sad::Renderer::_toClient(pnt);
}

LRESULT sad::Renderer::dispatchMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static bool msg_init=false;
	if (!msg_init)
	{
		msg_init=true;
		table_init();
	}

	if (uMsg==WM_CLOSE)
	{
     	this->m_running=false;					
		return 0;													
	}
	if (uMsg==WM_MOUSEMOVE)
	{
		if (this->controls()->areMovingNotTracked()) 
			return 0;
		float mx=0,my=0,mz=0;
		int key=(wParam==MK_LBUTTON)?MOUSE_BUTTON_LEFT:(wParam==MK_RBUTTON)?MOUSE_BUTTON_RIGHT:(wParam==MK_MBUTTON)?MOUSE_BUTTON_MIDDLE:0;
		POINT pnt = ref()->_toClient(lParam);
		this->mapToOGL(pnt.x,pnt.y,mx,my,mz);
		this->controls()->postMouseMove(sad::Event(mx,my,mz,key));
	}
	if (uMsg==WM_MOUSEWHEEL)
	{
		if (this->controls()->areWheelNotTracked()) 
			return 0;
		float mx=0,my=0,mz=0;
		float fw=GET_WHEEL_DELTA_WPARAM(wParam)/(float)WHEEL_DELTA;
		wParam=GET_KEYSTATE_WPARAM(wParam);
		int key=(wParam==MK_LBUTTON)?MOUSE_BUTTON_LEFT:(wParam==MK_RBUTTON)?MOUSE_BUTTON_RIGHT:(wParam==MK_MBUTTON)?MOUSE_BUTTON_MIDDLE:0;
		POINT pnt = ref()->_toClient(lParam);
		this->mapToOGL(pnt.x,pnt.y,mx,my,mz);
		sad::Event ev(mx,my,mz,key);
		ev.delta=fw;
		this->controls()->postMouseWheel(ev);
		return 0;
	}
	if (uMsg==WM_LBUTTONDBLCLK || uMsg==WM_MBUTTONDBLCLK || uMsg==WM_RBUTTONDBLCLK)
	{
		if (this->controls()->areDblClickNotTracked())
			return 0;
		float mx=0,my=0,mz=0;
		int key=(wParam==MK_LBUTTON)?MOUSE_BUTTON_LEFT:(wParam==MK_RBUTTON)?MOUSE_BUTTON_RIGHT:(wParam==MK_MBUTTON)?MOUSE_BUTTON_MIDDLE:0;
		POINT pnt = ref()->_toClient(lParam);
		this->mapToOGL(pnt.x,pnt.y,mx,my,mz);
		this->controls()->postMouseDblClick(sad::Event(mx,my,mz,key));
		return 0;
	}
	if (uMsg==WM_LBUTTONUP || uMsg==WM_MBUTTONUP || uMsg==WM_RBUTTONUP)
	{
		if (this->controls()->areUpNotTracked())
			return 0;
		float mx=0,my=0,mz=0;
		int key=(wParam==MK_LBUTTON)?MOUSE_BUTTON_LEFT:(wParam==MK_RBUTTON)?MOUSE_BUTTON_RIGHT:(wParam==MK_MBUTTON)?MOUSE_BUTTON_MIDDLE:0;
		POINT pnt = ref()->_toClient(lParam);
		this->mapToOGL(pnt.x,pnt.y,mx,my,mz);
		this->controls()->postMouseUp(sad::Event(mx,my,mz,key));
		return 0;
	}
	if (uMsg==WM_LBUTTONDOWN || uMsg==WM_MBUTTONDOWN || uMsg==WM_RBUTTONDOWN)
	{
		if (this->controls()->areDownNotTracked() && this->controls()->areClickNotTracked())
			return 0;
		float mx=0,my=0,mz=0;
		int key=(wParam==MK_LBUTTON)?MOUSE_BUTTON_LEFT:(wParam==MK_RBUTTON)?MOUSE_BUTTON_RIGHT:(wParam==MK_MBUTTON)?MOUSE_BUTTON_MIDDLE:0;
		POINT pnt = ref()->_toClient(lParam);
		ref()->mapToOGL(pnt.x,pnt.y,mx,my,mz);
		this->controls()->postMouseDown(sad::Event(mx,my,mz,key));
		this->controls()->postMouseClick(sad::Event(mx,my,mz,key));
		return 0;
	}
	if (uMsg==WM_KEYDOWN || uMsg==WM_KEYUP)
	{
		sad::Event sev(0);
		SHORT c=GetAsyncKeyState(VK_CAPITAL)==1;
		SHORT a=GetAsyncKeyState(VK_MENU)<0;
		SHORT ct=GetAsyncKeyState(VK_CONTROL)<0;
		SHORT sh=GetAsyncKeyState(VK_SHIFT)<0;
		sev.capslock=c!=0;
		sev.alt=a!=0;
		sev.ctrl=ct!=0;
		sev.shift=sh!=0;
        if (table.contains(wParam))
		{
			sev.key=table[wParam];
			if (uMsg==WM_KEYDOWN)
			     this->controls()->postKeyDown(sev);
			else
				 this->controls()->postKeyUp(sev);
			return 0;
		}
		char af[5];
		GetKeyNameTextA(lParam,af,5);
		sev.key=af[0];
		if (uMsg==WM_KEYUP)
				this->controls()->postKeyDown(sev);
		else
				this->controls()->postKeyUp(sev);
		return 0;
	}
	if (uMsg==WM_SIZE)
	{
		if (wParam==SIZE_MINIMIZED)
		{
			this->m_window.active=false;
		}
		else
		{
			this->m_window.active=true;
			this->reshape(LOWORD (lParam), HIWORD (lParam));
		}
		return 0;
	}
	return 1;
}

LRESULT CALLBACK sad::Renderer::WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_data.lock();
	LRESULT unhandled = (LRESULT)1;
	if (m_renderers.contains(hWnd))
	{
		unhandled =m_renderers[hWnd]->dispatchMessage(hWnd, uMsg, wParam, lParam);
	}
	m_data.unlock();
	if (!unhandled)
		return 0;
	return DefWindowProc(hWnd, uMsg, wParam, lParam);					// Pass Unhandled Messages To DefWindowProc
}

#endif
