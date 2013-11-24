#include "os/systemwindowevent.h"

#ifdef WIN32

sad::os::SystemWindowEvent::SystemWindowEvent(
	HWND wnd, 
	UINT msg, 
	WPARAM wparam, 
	LPARAM lparam
):
WND(wnd), MSG(msg), WParam(wparam), LParam(lparam)
{

}

#endif

#ifdef X11

sad::os::SystemWindowEvent::SystemWindowEvent()
{

}

#endif
