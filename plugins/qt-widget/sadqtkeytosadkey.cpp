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
    { Qt::Key_Escape, static_cast<int>(sad::KeyboardKey::Esc) },          //0
    { Qt::Key_Tab, static_cast<int>(sad::KeyboardKey::Tab) },             //1
    { Qt::Key_Backtab, static_cast<int>(sad::KeyboardKey::KeyNone) },     //2
    { Qt::Key_Backspace, static_cast<int>(sad::KeyboardKey::Backspace) }, //3
    { Qt::Key_Return, static_cast<int>(sad::KeyboardKey::Enter) },        //4
    { Qt::Key_Enter, static_cast<int>(sad::KeyboardKey::Enter) },         //5
    { Qt::Key_Insert, static_cast<int>(sad::KeyboardKey::Insert) },       //6
    { Qt::Key_Delete, static_cast<int>(sad::KeyboardKey::Delete) },       //7
    { Qt::Key_Pause, static_cast<int>(sad::KeyboardKey::Pause) },         //8
    { Qt::Key_Print, static_cast<int>(sad::KeyboardKey::KeyNone)},        //9
    { Qt::Key_SysReq, static_cast<int>(sad::KeyboardKey::PrintScreen) },  //10
    { Qt::Key_Clear, static_cast<int>(sad::KeyboardKey::KeyNone) },       //11
    { 0x0100000c, static_cast<int>(sad::KeyboardKey::KeyNone)},
    { 0x0100000d, static_cast<int>(sad::KeyboardKey::KeyNone) },
    { 0x0100000e, static_cast<int>(sad::KeyboardKey::KeyNone) },
    { 0x0100000f, static_cast<int>(sad::KeyboardKey::KeyNone) },
    { Qt::Key_Home, static_cast<int>(sad::KeyboardKey::Home) },           //12
    { Qt::Key_End, static_cast<int>(sad::KeyboardKey::End) },             //13
    { Qt::Key_Left, static_cast<int>(sad::KeyboardKey::KeyLeft) },        //14
    { Qt::Key_Up, static_cast<int>(sad::KeyboardKey::KeyUp) },            //15
    { Qt::Key_Right, static_cast<int>(sad::KeyboardKey::KeyRight) },      //16
    { Qt::Key_Down, static_cast<int>(sad::KeyboardKey::KeyDown) },        //17
    { Qt::Key_PageUp, static_cast<int>(sad::KeyboardKey::PageUp) },       //18
    { Qt::Key_PageDown, static_cast<int>(sad::KeyboardKey::PageDown) },   //19
    { 0x01000018, static_cast<int>(sad::KeyboardKey::KeyNone) },
    { 0x01000019, static_cast<int>(sad::KeyboardKey::KeyNone) },
    { 0x0100001a, static_cast<int>(sad::KeyboardKey::KeyNone) },
    { 0x0100001b, static_cast<int>(sad::KeyboardKey::KeyNone) },
    { 0x0100001c, static_cast<int>(sad::KeyboardKey::KeyNone) },
    { 0x0100001d, static_cast<int>(sad::KeyboardKey::KeyNone) },
    { 0x0100001e, static_cast<int>(sad::KeyboardKey::KeyNone) },
    { 0x0100001f, static_cast<int>(sad::KeyboardKey::KeyNone) },
    { Qt::Key_Shift, static_cast<int>(sad::KeyboardKey::LeftShift) },     //20
    { Qt::Key_Control, static_cast<int>(sad::KeyboardKey::LeftControl) }, //21
    { Qt::Key_Meta, static_cast<int>(sad::KeyboardKey::WindowsKey) },     //22
    { Qt::Key_Alt, static_cast<int>(sad::KeyboardKey::LeftAlt) },         //23
    { Qt::Key_CapsLock, static_cast<int>(sad::KeyboardKey::CapsLock) },   //24
    { Qt::Key_NumLock, static_cast<int>(sad::KeyboardKey::NumLock) },     //25
    { Qt::Key_ScrollLock, static_cast<int>(sad::KeyboardKey::ScrollLock) }//26
};

int table_100030_100003b[][2] = {
    { Qt::Key_F1, static_cast<int>(sad::KeyboardKey::F1) },
    { Qt::Key_F2, static_cast<int>(sad::KeyboardKey::F2) },
    { Qt::Key_F3, static_cast<int>(sad::KeyboardKey::F3) },
    { Qt::Key_F4, static_cast<int>(sad::KeyboardKey::F4) },
    { Qt::Key_F5, static_cast<int>(sad::KeyboardKey::F5) },
    { Qt::Key_F6, static_cast<int>(sad::KeyboardKey::F6) },
    { Qt::Key_F7, static_cast<int>(sad::KeyboardKey::F7) },
    { Qt::Key_F8, static_cast<int>(sad::KeyboardKey::F8) },
    { Qt::Key_F9, static_cast<int>(sad::KeyboardKey::F9) },
    { Qt::Key_F10, static_cast<int>(sad::KeyboardKey::F10) },
    { Qt::Key_F11, static_cast<int>(sad::KeyboardKey::F11) },
    { Qt::Key_F12, static_cast<int>(sad::KeyboardKey::F12) }
};


const int table_common_size = 11;
int table_common[table_common_size][2] = {
    { Qt::Key_BracketLeft, static_cast<int>(sad::KeyboardKey::OpeningSquareBracket) },  //0
    { Qt::Key_BracketRight, static_cast<int>(sad::KeyboardKey::ClosingSquareBracket) }, //1
    { Qt::Key_QuoteLeft, static_cast<int>(sad::KeyboardKey::Tilde) },                   //2
    { Qt::Key_Minus, static_cast<int>(sad::KeyboardKey::Minus) },                       //3
    { Qt::Key_Plus, static_cast<int>(sad::KeyboardKey::Equal) },                        //4
    { Qt::Key_Slash, static_cast<int>(sad::KeyboardKey::Slash) },                       //5
    { Qt::Key_Backslash, static_cast<int>(sad::KeyboardKey::BackSlash) },               //6
    { Qt::Key_Period, static_cast<int>(sad::KeyboardKey::Period) },                     //7
    { Qt::Key_Semicolon, static_cast<int>(sad::KeyboardKey::Semicolon) },               //8
    { Qt::Key_Apostrophe, static_cast<int>(sad::KeyboardKey::Apostrophe) },             //9
    { Qt::Key_Comma, static_cast<int>(sad::KeyboardKey::Comma) }                        //10
};

sad::KeyboardKey sad::qt::qtKeyToSadKey(int key, quint32 nativeScanCode)
{
    if (key == 0 || key == Qt::Key_unknown)
    {
        return sad::KeyboardKey::KeyNone;
    }

    if (key == Qt::Key_AltGr)
    {
        return sad::KeyboardKey::RightAlt;
    }

    if (key  >= Qt::Key_0 && key <= Qt::Key_9)
    {
        return static_cast<sad::KeyboardKey>(static_cast<int>(key) - static_cast<int>(Qt::Key_0) + static_cast<int>(sad::KeyboardKey::Numeric0));
    }

    if (key >= Qt::Key_A && key <= Qt::Key_Z)
    {
        return static_cast<sad::KeyboardKey>(static_cast<int>(key) - static_cast<int>(Qt::Key_A) + static_cast<int>(sad::KeyboardKey::A));
    }


#ifdef WIN32
    if (nativeScanCode == VK_LCONTROL)
    {
        return sad::KeyboardKey::LeftControl;
    }

    if (nativeScanCode == VK_RCONTROL)
    {
        return sad::KeyboardKey::RightControl;
    }
    
    if (nativeScanCode == VK_LSHIFT)
    {
        return sad::KeyboardKey::LeftShift;
    }

    if (nativeScanCode == VK_RSHIFT)
    {
        return sad::KeyboardKey::RightShift;
    }


    if (nativeScanCode == VK_LMENU)
    {
        return sad::KeyboardKey::LeftAlt;
    }

    if (nativeScanCode == VK_RMENU)
    {
        return sad::KeyboardKey::RightAlt;
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
        return sad::KeyboardKey::WindowsKey;
    }

    if (key == Qt::Key_Menu)
    {
        return sad::KeyboardKey::AppKey;
    }

    if (key == Qt::Key_Space)
    {
        return sad::KeyboardKey::Space;
    }

    for(int i = 0; i < table_common_size; i++)
    {
        if (key == table_common[i][0])
        {
            return static_cast<sad::KeyboardKey>(table_common[i][1]);
        }
    }

    return sad::KeyboardKey::KeyNone;
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