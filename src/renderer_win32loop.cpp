#include "renderer.h"
#include "input.h"



#ifdef WIN32


#include <windowsx.h>

#ifndef GET_KEYSTATE_WPARAM 
	#define GET_KEYSTATE_WPARAM(wParam)     (LOWORD(wParam))
#endif

#ifndef GET_KEYSTATE_LPARAM
	#define GET_KEYSTATE_LPARAM(lParam) (LOWORD(lParam))
#endif

void sad::Renderer::mainLoop()
{
 int frames=0;
 bool isMessagePumpActive;
 MSG msg;

 m_running = true;											// Program Looping Is Set To TRUE
 m_window.active=true;
 Renderer::setTimer();


 while (m_running)											// Loop Until WM_QUIT Is Received
 {					
  // Check For Window Messages
  if (PeekMessage (&msg, m_window.hWND, 0, 0, PM_REMOVE) != 0)
  {
	 // Check For WM_QUIT Message
	 if (msg.message != WM_QUIT)						// Is The Message A WM_QUIT Message?
	 {
	  TranslateMessage(&msg);				// Translate The Message
	  DispatchMessage(&msg);						// If Not, Dispatch The Message
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
	  if (Renderer::instance().elapsedInMSeconds() >= 1000)
	  {
		  setFPS(frames);frames=0;Renderer::instance().setTimer();
	  }
	  //Update a window, if active
	  if (m_window.active)
	     update();
	  //Change scene, if need so
	  if (m_chscene) 
	  { setCurrentScene(m_chscene); m_chscene=NULL;}
   }
  }
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
LRESULT CALLBACK sad::Renderer::WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static bool msg_init=false;
	if (!msg_init)
	{
		msg_init=true;
		table_init();
	}

	if (uMsg==WM_CLOSE)
	{
     	    sad::Renderer::instance().m_running=false;					
			return 0;													
	}
	if (uMsg==WM_MOUSEMOVE)
	{
		if (sad::Input::inst()->areMovingNotTracked()) 
			return 0;
		float mx=0,my=0,mz=0;
		int key=(wParam==MK_LBUTTON)?MOUSE_BUTTON_LEFT:(wParam==MK_RBUTTON)?MOUSE_BUTTON_RIGHT:(wParam==MK_MBUTTON)?MOUSE_BUTTON_MIDDLE:0;
		instance().mapToOGL(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),mx,my,mz);
		sad::Input::inst()->postMouseMove(sad::Event(mx,my,mz,key));
	}
	if (uMsg==WM_MOUSEWHEEL)
	{
		if (sad::Input::inst()->areWheelNotTracked()) 
			return 0;
		float mx=0,my=0,mz=0;
		float fw=GET_WHEEL_DELTA_WPARAM(wParam)/WHEEL_DELTA;
		wParam=GET_KEYSTATE_WPARAM(wParam);
		int key=(wParam==MK_LBUTTON)?MOUSE_BUTTON_LEFT:(wParam==MK_RBUTTON)?MOUSE_BUTTON_RIGHT:(wParam==MK_MBUTTON)?MOUSE_BUTTON_MIDDLE:0;
		instance().mapToOGL(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),mx,my,mz);
		sad::Event ev(mx,my,mz,key);
		ev.delta=fw;
		sad::Input::inst()->postMouseWheel(ev);
		return 0;
	}
	if (uMsg==WM_LBUTTONDBLCLK || uMsg==WM_MBUTTONDBLCLK || uMsg==WM_RBUTTONDBLCLK)
	{
		if (sad::Input::inst()->areDblClickNotTracked())
			return 0;
		float mx=0,my=0,mz=0;
		int key=(wParam==MK_LBUTTON)?MOUSE_BUTTON_LEFT:(wParam==MK_RBUTTON)?MOUSE_BUTTON_RIGHT:(wParam==MK_MBUTTON)?MOUSE_BUTTON_MIDDLE:0;
		instance().mapToOGL(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),mx,my,mz);
		sad::Input::inst()->postMouseDblClick(sad::Event(mx,my,mz,key));
		return 0;
	}
	if (uMsg==WM_LBUTTONUP || uMsg==WM_MBUTTONUP || uMsg==WM_RBUTTONUP)
	{
		if (sad::Input::inst()->areUpNotTracked())
			return 0;
		float mx=0,my=0,mz=0;
		int key=(wParam==MK_LBUTTON)?MOUSE_BUTTON_LEFT:(wParam==MK_RBUTTON)?MOUSE_BUTTON_RIGHT:(wParam==MK_MBUTTON)?MOUSE_BUTTON_MIDDLE:0;
		instance().mapToOGL(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),mx,my,mz);
		sad::Input::inst()->postMouseUp(sad::Event(mx,my,mz,key));
		return 0;
	}
	if (uMsg==WM_LBUTTONDOWN || uMsg==WM_MBUTTONDOWN || uMsg==WM_RBUTTONDOWN)
	{
		if (sad::Input::inst()->areDownNotTracked() && sad::Input::inst()->areClickNotTracked())
			return 0;
		float mx=0,my=0,mz=0;
		int key=(wParam==MK_LBUTTON)?MOUSE_BUTTON_LEFT:(wParam==MK_RBUTTON)?MOUSE_BUTTON_RIGHT:(wParam==MK_MBUTTON)?MOUSE_BUTTON_MIDDLE:0;
		instance().mapToOGL(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),mx,my,mz);
		sad::Input::inst()->postMouseDown(sad::Event(mx,my,mz,key));
		sad::Input::inst()->postMouseClick(sad::Event(mx,my,mz,key));
		return 0;
	}
	if (uMsg==WM_KEYDOWN || uMsg==WM_KEYUP)
	{
        if (table.contains(wParam))
		{
			if (uMsg==WM_KEYDOWN)
			     sad::Input::inst()->postKeyDown(sad::Event(table[wParam]));
			else
				 sad::Input::inst()->postKeyUp(sad::Event(table[wParam]));
			return 0;
		}
		char af[5];
		GetKeyNameTextA(lParam,af,5);
		if (uMsg==WM_KEYUP)
				sad::Input::inst()->postKeyDown(sad::Event((int)(af[0])));
		else
			    sad::Input::inst()->postKeyUp(sad::Event((int)(af[0])));
		return 0;
	}
	if (uMsg==WM_SIZE)
	{
		if (wParam==SIZE_MINIMIZED)
		{
			instance().m_window.active=false;
		}
		else
		{
			instance().m_window.active=true;
			instance().reshape(LOWORD (lParam), HIWORD (lParam));
		}
	}
	return DefWindowProc (hWnd, uMsg, wParam, lParam);					// Pass Unhandled Messages To DefWindowProc
}


#endif
