#include "renderer.h"
#include "input.h"
#include "texturemanager.h"
#include "3rdparty/format/format.h"
#include "os/windowhandles.h"
#include "window.h"
#include "glcontext.h"
#include "sadsleep.h"

#ifdef WIN32


#include <windowsx.h>

#ifndef GET_KEYSTATE_WPARAM 
	#define GET_KEYSTATE_WPARAM(wParam)     (LOWORD(wParam))
#endif

#ifndef GET_KEYSTATE_LPARAM
	#define GET_KEYSTATE_LPARAM(lParam) (LOWORD(lParam))
#endif


sad::Hash<HWND, sad::Renderer *> m_renderers;
static  sad::Mutex m_data;



static int WINAPI  handleClosing(DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_CLOSE_EVENT)
	{
		for(sad::Hash<HWND, sad::Renderer *>::iterator it = m_renderers.begin();
			it != m_renderers.end();
			it++)
		{
			it.value()->textures()->unload();
		}
	}
	return FALSE;
}




void sad::Renderer::mainLoop()
{
 m_data.lock();
 if (m_renderers.contains(this->window()->handles()->WND)) 
 {
	m_renderers[this->window()->handles()->WND] =  this;
 }
 else
 {
	 m_renderers.insert(this->window()->handles()->WND, this);
 }
 m_data.unlock();


 SetConsoleCtrlHandler(handleClosing, FALSE);
 SetConsoleCtrlHandler(handleClosing, TRUE);

 int frames=0;
 
 m_fps = 75;
 m_setimmediately = true;
 m_frames = 0;

 //bool isMessagePumpActive;
 MSG msg;

 m_running = true;											// Program Looping Is Set To TRUE
 this->window()->setActive(true);

 while (m_running)											// Loop Until WM_QUIT Is Received
 {					
  // Check For Window Messages
	if (PeekMessage (&msg, this->window()->handles()->WND, 0, 0, PM_REMOVE) != 0)
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
	  //Update a window, if active
	  if (m_window->active())
	  {
		 update();
	  }
	  else 
	  {
		  // Reset timer, so no big FPS jump at time
		  m_timer.start();
		  sad::sleep(50);
	  }
	  
	  
   }
  }
 this->controls()->postQuit();
 m_window->setActive(false);
 m_renderers.remove(this->window()->handles()->WND);
}


static sad::Hash<WPARAM,int> table;

static sad::Mutex m_table_init_lock;

static void table_init()
{
   m_table_init_lock.lock();
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
   m_table_init_lock.unlock();
}


const int windowresizepointscount = 8;
LRESULT windowresizepoints[windowresizepointscount] = {
	HTTOPLEFT,
	HTTOP,
	
	HTTOPRIGHT,
	HTRIGHT,
	HTLEFT,

	HTBOTTOMLEFT,
	HTBOTTOM,
	HTBOTTOMRIGHT
};

#define MESSAGE_NOT_HANDLED 2000000

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
		sad::Point2D p(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		p = this->window()->toClient(p);
		sad::Point3D op = this->context()->mapToViewport(p, m_glsettings.ztest());
		this->controls()->postMouseMove(sad::Event(op.x(), op.y(), op.z(),key));
		return 0;
	}
	if (uMsg==WM_MOUSEWHEEL)
	{
		if (this->controls()->areWheelNotTracked()) 
			return 0;
		float mx=0,my=0,mz=0;
		float fw=GET_WHEEL_DELTA_WPARAM(wParam)/(float)WHEEL_DELTA;
		wParam=GET_KEYSTATE_WPARAM(wParam);
		int key=(wParam==MK_LBUTTON)?MOUSE_BUTTON_LEFT:(wParam==MK_RBUTTON)?MOUSE_BUTTON_RIGHT:(wParam==MK_MBUTTON)?MOUSE_BUTTON_MIDDLE:0;
		sad::Point2D p(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		p = this->window()->toClient(p);
		sad::Point3D op = this->context()->mapToViewport(p, m_glsettings.ztest());
		sad::Event ev(op.x(),op.y(),op.z(),key);
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
		sad::Point2D p(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		p = this->window()->toClient(p);
		sad::Point3D op = this->context()->mapToViewport(p, m_glsettings.ztest());
		this->controls()->postMouseDblClick(sad::Event(op.x(),op.y(),op.z(),key));
		return 0;
	}
	if (uMsg==WM_LBUTTONUP || uMsg==WM_MBUTTONUP || uMsg==WM_RBUTTONUP)
	{
		if (this->controls()->areUpNotTracked())
			return 0;
		float mx=0,my=0,mz=0;
		int key=(wParam==MK_LBUTTON)?MOUSE_BUTTON_LEFT:(wParam==MK_RBUTTON)?MOUSE_BUTTON_RIGHT:(wParam==MK_MBUTTON)?MOUSE_BUTTON_MIDDLE:0;
		sad::Point2D p(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		p = this->window()->toClient(p);
		sad::Point3D op = this->context()->mapToViewport(p, m_glsettings.ztest());
		this->controls()->postMouseUp(sad::Event(op.x(),op.y(),op.z(),key));
		return 0;
	}
	if (uMsg==WM_LBUTTONDOWN || uMsg==WM_MBUTTONDOWN || uMsg==WM_RBUTTONDOWN)
	{
		if (this->controls()->areDownNotTracked() && this->controls()->areClickNotTracked())
			return 0;
		float mx=0,my=0,mz=0;
		int key=(wParam==MK_LBUTTON)?MOUSE_BUTTON_LEFT:(wParam==MK_RBUTTON)?MOUSE_BUTTON_RIGHT:(wParam==MK_MBUTTON)?MOUSE_BUTTON_MIDDLE:0;
		sad::Point2D p(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		p = this->window()->toClient(p);
		sad::Point3D op = this->context()->mapToViewport(p, m_glsettings.ztest());
		this->controls()->postMouseDown(sad::Event(op.x(),op.y(),op.z(),key));
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
			window()->setActive(false);
		}
		else
		{
			window()->setActive(true);
			this->reshape(LOWORD (lParam), HIWORD (lParam));
		}
		return 0;
	}
	
	if (uMsg == WM_NCHITTEST && window()->fixed()) 
	{		
		LRESULT result = DefWindowProcA(hWnd, uMsg, wParam, lParam);
		bool isusertriestoresize = false;
		for(int i = 0; i < windowresizepointscount; i++)
			isusertriestoresize = isusertriestoresize || (windowresizepoints[i] == result);
		
		if (isusertriestoresize)
		{
			result = HTBORDER;
		}
		
		return result;
	}
	
	return MESSAGE_NOT_HANDLED;
}

LRESULT CALLBACK sad_renderer_window_proc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	sad::Renderer * r = NULL; 
	m_data.lock();
	LRESULT result = MESSAGE_NOT_HANDLED;
	if (m_renderers.contains(hWnd))
	{
		r = m_renderers[hWnd]; 
		result = r->dispatchMessage(hWnd, uMsg, wParam, lParam);
	}
	m_data.unlock();
	if (result != MESSAGE_NOT_HANDLED)
	{
		return result;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);				
}

#endif
