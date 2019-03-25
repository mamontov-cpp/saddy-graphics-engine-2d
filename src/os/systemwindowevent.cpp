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

sad::os::SystemWindowEvent::SystemWindowEvent(const sad::os::SystemWindowEvent& o)
: WND(o.WND), MSG(o.MSG), WParam(o.WParam), LParam(o.LParam)
{
    
}

#endif

#ifdef X11

sad::os::SystemWindowEvent::SystemWindowEvent()
{

}

#endif
