#include "os/keydecoder.h"

sad::os::KeyDecoder::KeyDecoder()
{

}

#ifdef WIN32

/**
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
*/

sad::os::SystemKey syskeys[] = {
   0
};

sad::os::Key keys[] = {
   0
};

#endif


#ifdef X11

/**
	rectbl.insert(XK_Escape,KEY_ESC);
        rectbl.insert(XK_F1,KEY_F1);
	rectbl.insert(XK_F2,KEY_F2);
	rectbl.insert(XK_F3,KEY_F3);
	rectbl.insert(XK_F4,KEY_F4);
	rectbl.insert(XK_F5,KEY_F5);
	rectbl.insert(XK_F6,KEY_F6);
	rectbl.insert(XK_F7,KEY_F7);
	rectbl.insert(XK_F8,KEY_F8);
	rectbl.insert(XK_F9,KEY_F9);
	rectbl.insert(XK_F10,KEY_F10);
	rectbl.insert(XK_F11,KEY_F11);
	rectbl.insert(XK_F12,KEY_F12);
	rectbl.insert(XK_grave,'~');
	rectbl.insert(XK_minus,'-');
	rectbl.insert(XK_equal,'=');
 	rectbl.insert(XK_backslash,'\\');
	rectbl.insert(XK_BackSpace,KEY_BACKSPACE);
	rectbl.insert(XK_Scroll_Lock,KEY_SCROLLOCK);
	rectbl.insert(XK_Pause,KEY_PAUSE);
	rectbl.insert(XK_Insert,KEY_INSERT);
	rectbl.insert(XK_Home,KEY_HOME);
	rectbl.insert(XK_Page_Up,KEY_PGUP);
	rectbl.insert(XK_Tab,KEY_TAB);
	rectbl.insert(XK_bracketleft,'[');
	rectbl.insert(XK_bracketright,']');
	rectbl.insert(XK_Return,KEY_ENTER);
	rectbl.insert(XK_Delete,KEY_DELETE);
	rectbl.insert(XK_End,KEY_END);
	rectbl.insert(XK_Page_Down,KEY_PGDOWN);
	rectbl.insert(XK_Caps_Lock,KEY_CAPSLOCK);
	rectbl.insert(XK_semicolon,';');
	rectbl.insert(XK_apostrophe,'\'');
	rectbl.insert(XK_Shift_L,KEY_LSHIFT);
	rectbl.insert(XK_comma,',');
	rectbl.insert(XK_period,'.');
	rectbl.insert(XK_slash,'/');
	rectbl.insert(XK_Shift_R,KEY_RSHIFT);
	rectbl.insert(XK_Left,KEY_LEFT);
	rectbl.insert(XK_Up,KEY_UP);
	rectbl.insert(XK_Right,KEY_RIGHT);
	rectbl.insert(XK_Down,KEY_DOWN);
	rectbl.insert(XK_Control_L,KEY_LCTRL);
	rectbl.insert(XK_Control_R,KEY_RCTRL);
	rectbl.insert(XK_Super_L,KEY_WIN);
	rectbl.insert(XK_Super_R,KEY_WIN);
	rectbl.insert(XK_Alt_L,KEY_LALT);
	rectbl.insert(XK_Alt_R,KEY_RALT);
	rectbl.insert(XK_space, KEY_SPACE);
	rectbl.insert(XK_Menu, KEY_APP);
 	rectbl.insert(XK_Sys_Req, KEY_PRINTSCREEN); //!< Most WM hook this key, so it  can't be called much.
 */

sad::os::SystemKey syskeys[] = {
  0
};

sad::os::Key keys[] = {
  0
};

#endif