#include "renderer.h"
#include "input.h"

#ifdef WIN32


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
  Renderer::instance().setTimer();					
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
	  {m_fps = frames;frames=0;Renderer::instance().setTimer();}
	  
	  if (m_window.active)
	     update();
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
		char af[20];
		GetKeyNameText(lParam,af,19);
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
