#include "qt/sadqtkeytosadkey.h"

#ifdef WIN32
#include <windows.h>
#endif

#ifdef X11
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#endif

int table_100000_1000026[][2] = {
    { Qt::Key_Escape, sad::Esc },          //0
    { Qt::Key_Tab, sad::Tab },             //1
    { Qt::Key_Backtab, sad::KeyNone },     //2
    { Qt::Key_Backspace, sad::Backspace }, //3
    { Qt::Key_Return, sad::Enter },        //4
    { Qt::Key_Enter, sad::Enter },         //5
    { Qt::Key_Insert, sad::Insert },       //6
    { Qt::Key_Delete, sad::Delete },       //7
    { Qt::Key_Pause, sad::Pause },         //8
    { Qt::Key_Print, sad::KeyNone},        //9
    { Qt::Key_SysReq, sad::PrintScreen },  //10
    { Qt::Key_Clear, sad::KeyNone },       //11
    { 0x0100000c, sad::KeyNone},
    { 0x0100000d, sad::KeyNone },
    { 0x0100000e, sad::KeyNone },
    { 0x0100000f, sad::KeyNone },
    { Qt::Key_Home, sad::Home },           //12
    { Qt::Key_End, sad::End },             //13
    { Qt::Key_Left, sad::KeyLeft },        //14
    { Qt::Key_Up, sad::KeyUp },            //15
    { Qt::Key_Right, sad::KeyRight },      //16
    { Qt::Key_Down, sad::KeyDown },        //17
    { Qt::Key_PageUp, sad::PageUp },       //18
    { Qt::Key_PageDown, sad::PageDown },   //19
    { 0x01000018, sad::KeyNone },
    { 0x01000019, sad::KeyNone },
    { 0x0100001a, sad::KeyNone },
    { 0x0100001b, sad::KeyNone },
    { 0x0100001c, sad::KeyNone },
    { 0x0100001d, sad::KeyNone },
    { 0x0100001e, sad::KeyNone },
    { 0x0100001f, sad::KeyNone },
    { Qt::Key_Shift, sad::LeftShift },     //20
    { Qt::Key_Control, sad::LeftControl }, //21
    { Qt::Key_Meta, sad::WindowsKey },     //22
    { Qt::Key_Alt, sad::LeftAlt },         //23
    { Qt::Key_CapsLock, sad::CapsLock },   //24
    { Qt::Key_NumLock, sad::NumLock },     //25
    { Qt::Key_ScrollLock, sad::ScrollLock }//26
};

int table_100030_100003b[][2] = {
    { Qt::Key_F1, sad::F1 },
    { Qt::Key_F2, sad::F2 },
    { Qt::Key_F3, sad::F3 },
    { Qt::Key_F4, sad::F4 },
    { Qt::Key_F5, sad::F5 },
    { Qt::Key_F6, sad::F6 },
    { Qt::Key_F7, sad::F7 },
    { Qt::Key_F8, sad::F8 },
    { Qt::Key_F9, sad::F9 },
    { Qt::Key_F10, sad::F10 },
    { Qt::Key_F11, sad::F11 },
    { Qt::Key_F12, sad::F12 }
};


const int table_common_size = 11;
int table_common[table_common_size][2] = {
    { Qt::Key_BracketLeft, sad::OpeningSquareBracket },  //0
    { Qt::Key_BracketRight, sad::ClosingSquareBracket }, //1
    { Qt::Key_QuoteLeft, sad::Tilde },                   //2
    { Qt::Key_Minus, sad::Minus },                       //3
    { Qt::Key_Plus, sad::Equal },                        //4
    { Qt::Key_Slash, sad::Slash },                       //5
    { Qt::Key_Backslash, sad::BackSlash },               //6
    { Qt::Key_Period, sad::Period },                     //7
    { Qt::Key_Semicolon, sad::Semicolon },               //8
    { Qt::Key_Apostrophe, sad::Apostrophe },             //9
    { Qt::Key_Comma, sad::Comma }                        //10
};

sad::KeyboardKey sad::qt::qtKeyToSadKey(int key, quint32 nativeScanCode)
{
    if (key == 0 || key == Qt::Key_unknown)
    {
        return sad::KeyNone;
    }

    if (key == Qt::Key_AltGr)
    {
        return sad::RightAlt;
    }

    if (key  >= Qt::Key_0 && key <= Qt::Key_9)
    {
        return static_cast<sad::KeyboardKey>(static_cast<int>(key) - static_cast<int>(Qt::Key_0) + static_cast<int>(sad::Numeric0));
    }

    if (key >= Qt::Key_A && key <= Qt::Key_Z)
    {
        return static_cast<sad::KeyboardKey>(static_cast<int>(key) - static_cast<int>(Qt::Key_A) + static_cast<int>(sad::A));
    }


#ifdef WIN32
    if (nativeScanCode == VK_LCONTROL)
    {
        return sad::LeftControl;
    }

    if (nativeScanCode == VK_RCONTROL)
    {
        return sad::RightControl;
    }
    
    if (nativeScanCode == VK_LSHIFT)
    {
        return sad::LeftShift;
    }

    if (nativeScanCode == VK_RSHIFT)
    {
        return sad::RightShift;
    }


    if (nativeScanCode == VK_LMENU)
    {
        return sad::LeftAlt;
    }

    if (nativeScanCode == VK_RMENU)
    {
        return sad::RightAlt;
    }
#endif

#ifdef X11
    if (nativeScanCode == XK_Control_L)
    {
        return sad::LeftControl;
    }

    if (nativeScanCode == XK_Control_R)
    {
        return sad::RightControl;
    }

    if (nativeScanCode == XK_Shift_L)
    {
        return sad::LeftShift;
    }

    if (nativeScanCode == XK_Shift_R)
    {
        return sad::RightShift;
    }


    if (nativeScanCode == XK_Alt_L)
    {
        return sad::LeftAlt;
    }

    if (nativeScanCode == XK_Alt_R)
    {
        return sad::RightAlt;
    }
#endif

    if (key >= Qt::Key_Escape && key <= Qt::Key_ScrollLock)
    {
        int pos = key;
        pos -= Qt::Key_Escape;
        assert(table_100000_1000026[pos][0] == key);
        return static_cast<sad::KeyboardKey>(table_100000_1000026[pos][1]);
    }

    if (key >= Qt::Key_F1 && key <= Qt::Key_F12)
    {
        int pos = key;
        pos -= Qt::Key_F1;
        assert(table_100030_100003b[pos][0] == key);
        return static_cast<sad::KeyboardKey>(table_100030_100003b[pos][1]);
    }

    if (key == Qt::Key_Super_L || key == Qt::Key_Super_R)
    {
        return sad::WindowsKey;
    }

    if (key == Qt::Key_Menu)
    {
        return sad::AppKey;
    }

    if (key == Qt::Key_Space)
    {
        return sad::Space;
    }

    for(int i = 0; i < table_common_size; i++)
    {
        if (key == table_common[i][0])
        {
            return static_cast<sad::KeyboardKey>(table_common[i][1]);
        }
    }

    return sad::KeyNone;
}


void sad::qt::qtKeyEventToSadKeyEvent(QKeyEvent* ev, sad::input::KeyEvent* sev)
{
    sev->Key = sad::qt::qtKeyToSadKey(ev->key(), ev->nativeScanCode());
    if (ev->text().size() != 0)
    {
        sev->ReadableKey.setValue(ev->text().toStdString());
    }
    sev->AltHeld = ((ev->modifiers() & Qt::AltModifier) != 0);
    sev->CtrlHeld = ((ev->modifiers() & Qt::ControlModifier) != 0);
    sev->ShiftHeld = ((ev->modifiers() & Qt::ShiftModifier) != 0);
}