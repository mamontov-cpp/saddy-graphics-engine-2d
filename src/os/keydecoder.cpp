#include "os/keydecoder.h"
#include "os/systemwindowevent.h"
#include "window.h"
#include <cstdint>

sad::os::KeyDecoder::KeyDecoder()
{
    init();
}



#ifdef WIN32

const int syskeycount = 223;

// Partially taken from Qt SDK, via qkeymapper_win.cpp
sad::KeyboardKey syskeys[syskeycount] = { // Keyboard mapping table
                               // Dec |  Hex | Windows Virtual key | Description
    sad::KeyboardKey::KeyNone,              //   0   0x00
    sad::KeyboardKey::KeyNone,              //   1   0x01   VK_LBUTTON          | Left mouse button
    sad::KeyboardKey::KeyNone,              //   2   0x02   VK_RBUTTON          | Right mouse button
    sad::KeyboardKey::KeyNone,              //   3   0x03   VK_CANCEL           | Control-Break processing
    sad::KeyboardKey::KeyNone,              //   4   0x04   VK_MBUTTON          | Middle mouse button
    sad::KeyboardKey::KeyNone,              //   5   0x05   VK_XBUTTON1         | X1 mouse button
    sad::KeyboardKey::KeyNone,              //   6   0x06   VK_XBUTTON2         | X2 mouse button
    sad::KeyboardKey::KeyNone,              //   7   0x07                       | -- unassigned --
    sad::KeyboardKey::Backspace,            //   8   0x08   VK_BACK             | BackSpace key
    sad::KeyboardKey::Tab,                  //   9   0x09   VK_TAB              | Tab key
    sad::KeyboardKey::KeyNone,              //  10   0x0A                       | -- reserved --
    sad::KeyboardKey::KeyNone,              //  11   0x0B                       | -- reserved --
    sad::KeyboardKey::KeyNone,              //  12   0x0C   VK_CLEAR            | Clear key
    sad::KeyboardKey::Enter  ,              //  13   0x0D   VK_RETURN           | Enter key
    sad::KeyboardKey::KeyNone,              //  14   0x0E                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              //  15   0x0F                       | -- unassigned --
    sad::KeyboardKey::LeftShift,            //  16   0x10   VK_SHIFT            | Shift key
    sad::KeyboardKey::LeftControl,          //  17   0x11   VK_CONTROL          | Ctrl key
    sad::KeyboardKey::LeftAlt,              //  18   0x12   VK_MENU             | Alt key
    sad::KeyboardKey::Pause,                //  19   0x13   VK_PAUSE            | Pause key
    sad::KeyboardKey::CapsLock,             //  20   0x14   VK_CAPITAL          | Caps-Lock
    sad::KeyboardKey::KeyNone,              //  21   0x15   VK_KANA / VK_HANGUL | IME Kana or Hangul mode
    sad::KeyboardKey::KeyNone,              //  22   0x16                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              //  23   0x17   VK_JUNJA            | IME Junja mode
    sad::KeyboardKey::KeyNone,              //  24   0x18   VK_FINAL            | IME final mode
    sad::KeyboardKey::KeyNone,              //  25   0x19   VK_HANJA / VK_KANJI | IME Hanja or Kanji mode
    sad::KeyboardKey::KeyNone,              //  26   0x1A                       | -- unassigned --
    sad::KeyboardKey::Esc,                  //  27   0x1B   VK_ESCAPE           | Esc key
    sad::KeyboardKey::KeyNone,              //  28   0x1C   VK_CONVERT          | IME convert
    sad::KeyboardKey::KeyNone,              //  29   0x1D   VK_NONCONVERT       | IME non-convert
    sad::KeyboardKey::KeyNone,              //  30   0x1E   VK_ACCEPT           | IME accept
    sad::KeyboardKey::KeyNone,              //  31   0x1F   VK_MODECHANGE       | IME mode change request
    sad::KeyboardKey::Space,                //  32   0x20   VK_SPACE            | Spacebar
    sad::KeyboardKey::PageUp,               //  33   0x21   VK_PRIOR            | Page Up key
    sad::KeyboardKey::PageDown,             //  34   0x22   VK_NEXT             | Page Down key
    sad::KeyboardKey::End,                  //  35   0x23   VK_END              | End key
    sad::KeyboardKey::Home,                 //  36   0x24   VK_HOME             | Home key
    sad::KeyboardKey::KeyLeft,              //  37   0x25   VK_LEFT             | Left arrow key
    sad::KeyboardKey::KeyUp,                //  38   0x26   VK_UP               | Up arrow key
    sad::KeyboardKey::KeyRight,             //  39   0x27   VK_RIGHT            | Right arrow key
    sad::KeyboardKey::KeyDown,              //  40   0x28   VK_DOWN             | Down arrow key
    sad::KeyboardKey::KeyNone,              //  41   0x29   VK_SELECT           | Select key
    sad::KeyboardKey::KeyNone,              //  42   0x2A   VK_PRINT            | Print key
    sad::KeyboardKey::KeyNone,              //  43   0x2B   VK_EXECUTE          | Execute key
    sad::KeyboardKey::PrintScreen,          //  44   0x2C   VK_SNAPSHOT         | Print Screen key
    sad::KeyboardKey::Insert,               //  45   0x2D   VK_INSERT           | Ins key
    sad::KeyboardKey::Delete,               //  46   0x2E   VK_DELETE           | Del key
    sad::KeyboardKey::KeyNone,              //  47   0x2F   VK_HELP             | Help key
    sad::KeyboardKey::Numeric0,             //  48   0x30   (VK_0)              | 0 key
    sad::KeyboardKey::Numeric1,             //  49   0x31   (VK_1)              | 1 key
    sad::KeyboardKey::Numeric2,             //  50   0x32   (VK_2)              | 2 key
    sad::KeyboardKey::Numeric3,             //  51   0x33   (VK_3)              | 3 key
    sad::KeyboardKey::Numeric4,             //  52   0x34   (VK_4)              | 4 key
    sad::KeyboardKey::Numeric5,             //  53   0x35   (VK_5)              | 5 key
    sad::KeyboardKey::Numeric6,             //  54   0x36   (VK_6)              | 6 key
    sad::KeyboardKey::Numeric7,             //  55   0x37   (VK_7)              | 7 key
    sad::KeyboardKey::Numeric8,             //  56   0x38   (VK_8)              | 8 key
    sad::KeyboardKey::Numeric9,             //  57   0x39   (VK_9)              | 9 key
    sad::KeyboardKey::KeyNone,              //  58   0x3A                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              //  59   0x3B                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              //  60   0x3C                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              //  61   0x3D                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              //  62   0x3E                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              //  63   0x3F                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              //  64   0x40                       | -- unassigned --
    sad::KeyboardKey::A,                    //  65   0x41   (VK_A)              | A key
    sad::KeyboardKey::B,                    //  66   0x42   (VK_B)              | B key
    sad::KeyboardKey::C,                    //  67   0x43   (VK_C)              | C key
    sad::KeyboardKey::D,                    //  68   0x44   (VK_D)              | D key
    sad::KeyboardKey::E,                    //  69   0x45   (VK_E)              | E key
    sad::KeyboardKey::F,                    //  70   0x46   (VK_F)              | F key
    sad::KeyboardKey::G,                    //  71   0x47   (VK_G)              | G key
    sad::KeyboardKey::H,                    //  72   0x48   (VK_H)              | H key
    sad::KeyboardKey::I,                    //  73   0x49   (VK_I)              | I key
    sad::KeyboardKey::J,                    //  74   0x4A   (VK_J)              | J key
    sad::KeyboardKey::K,                    //  75   0x4B   (VK_K)              | K key
    sad::KeyboardKey::L,                    //  76   0x4C   (VK_L)              | L key
    sad::KeyboardKey::M,                    //  77   0x4D   (VK_M)              | M key
    sad::KeyboardKey::N,                    //  78   0x4E   (VK_N)              | N key
    sad::KeyboardKey::O,                    //  79   0x4F   (VK_O)              | O key
    sad::KeyboardKey::P,                    //  80   0x50   (VK_P)              | P key
    sad::KeyboardKey::Q,                    //  81   0x51   (VK_Q)              | Q key
    sad::KeyboardKey::R,                    //  82   0x52   (VK_R)              | R key
    sad::KeyboardKey::S,                    //  83   0x53   (VK_S)              | S key
    sad::KeyboardKey::T,                    //  84   0x54   (VK_T)              | T key
    sad::KeyboardKey::U,                    //  85   0x55   (VK_U)              | U key
    sad::KeyboardKey::V,                    //  86   0x56   (VK_V)              | V key
    sad::KeyboardKey::W,                    //  87   0x57   (VK_W)              | W key
    sad::KeyboardKey::X,                    //  88   0x58   (VK_X)              | X key
    sad::KeyboardKey::Y,                    //  89   0x59   (VK_Y)              | Y key
    sad::KeyboardKey::Z,                    //  90   0x5A   (VK_Z)              | Z key
    sad::KeyboardKey::WindowsKey,           //  91   0x5B   VK_LWIN             | Left Windows  - MS Natural kbd
    sad::KeyboardKey::WindowsKey,           //  92   0x5C   VK_RWIN             | Right Windows - MS Natural kbd
    sad::KeyboardKey::AppKey,               //  93   0x5D   VK_APPS             | Application key-MS Natural kbd
    sad::KeyboardKey::KeyNone,              //  94   0x5E   -- reserved --
    sad::KeyboardKey::KeyNone,              //  95   0x5F   VK_SLEEP
    sad::KeyboardKey::Numeric0,             //  96   0x60   VK_NUMPAD0          | Numeric keypad 0 key
    sad::KeyboardKey::Numeric1,             //  97   0x61   VK_NUMPAD1          | Numeric keypad 1 key
    sad::KeyboardKey::Numeric2,             //  98   0x62   VK_NUMPAD2          | Numeric keypad 2 key
    sad::KeyboardKey::Numeric3,             //  99   0x63   VK_NUMPAD3          | Numeric keypad 3 key
    sad::KeyboardKey::Numeric4,             // 100   0x64   VK_NUMPAD4          | Numeric keypad 4 key
    sad::KeyboardKey::Numeric5,             // 101   0x65   VK_NUMPAD5          | Numeric keypad 5 key
    sad::KeyboardKey::Numeric6,             // 102   0x66   VK_NUMPAD6          | Numeric keypad 6 key
    sad::KeyboardKey::Numeric7,             // 103   0x67   VK_NUMPAD7          | Numeric keypad 7 key
    sad::KeyboardKey::Numeric8,             // 104   0x68   VK_NUMPAD8          | Numeric keypad 8 key
    sad::KeyboardKey::Numeric9,             // 105   0x69   VK_NUMPAD9          | Numeric keypad 9 key
    sad::KeyboardKey::KeyNone,              // 106   0x6A   VK_MULTIPLY         | Multiply key
    sad::KeyboardKey::KeyNone,              // 107   0x6B   VK_ADD              | Add key
    sad::KeyboardKey::KeyNone,              // 108   0x6C   VK_SEPARATOR        | Separator key
    sad::KeyboardKey::KeyNone,              // 109   0x6D   VK_SUBTRACT         | Subtract key
    sad::KeyboardKey::KeyNone,              // 110   0x6E   VK_DECIMAL          | Decimal key
    sad::KeyboardKey::KeyNone,              // 111   0x6F   VK_DIVIDE           | Divide key
    sad::KeyboardKey::F1,                   // 112   0x70   VK_F1               | F1 key
    sad::KeyboardKey::F2,                   // 113   0x71   VK_F2               | F2 key
    sad::KeyboardKey::F3,                   // 114   0x72   VK_F3               | F3 key
    sad::KeyboardKey::F4,                   // 115   0x73   VK_F4               | F4 key
    sad::KeyboardKey::F5,                   // 116   0x74   VK_F5               | F5 key
    sad::KeyboardKey::F6,                   // 117   0x75   VK_F6               | F6 key
    sad::KeyboardKey::F7,                   // 118   0x76   VK_F7               | F7 key
    sad::KeyboardKey::F8,                   // 119   0x77   VK_F8               | F8 key
    sad::KeyboardKey::F9,                   // 120   0x78   VK_F9               | F9 key
    sad::KeyboardKey::F10,                  // 121   0x79   VK_F10              | F10 key
    sad::KeyboardKey::F11,                  // 122   0x7A   VK_F11              | F11 key
    sad::KeyboardKey::F12,                  // 123   0x7B   VK_F12              | F12 key
    sad::KeyboardKey::KeyNone,              // 124   0x7C   VK_F13              | F13 key
    sad::KeyboardKey::KeyNone,              // 125   0x7D   VK_F14              | F14 key
    sad::KeyboardKey::KeyNone,              // 126   0x7E   VK_F15              | F15 key
    sad::KeyboardKey::KeyNone,              // 127   0x7F   VK_F16              | F16 key
    sad::KeyboardKey::KeyNone,              // 128   0x80   VK_F17              | F17 key
    sad::KeyboardKey::KeyNone,              // 129   0x81   VK_F18              | F18 key
    sad::KeyboardKey::KeyNone,              // 130   0x82   VK_F19              | F19 key
    sad::KeyboardKey::KeyNone,              // 131   0x83   VK_F20              | F20 key
    sad::KeyboardKey::KeyNone,              // 132   0x84   VK_F21              | F21 key
    sad::KeyboardKey::KeyNone,              // 133   0x85   VK_F22              | F22 key
    sad::KeyboardKey::KeyNone,              // 134   0x86   VK_F23              | F23 key
    sad::KeyboardKey::KeyNone,              // 135   0x87   VK_F24              | F24 key
    sad::KeyboardKey::KeyNone,              // 136   0x88   -- unassigned --
    sad::KeyboardKey::KeyNone,              // 137   0x89   -- unassigned --
    sad::KeyboardKey::KeyNone,              // 138   0x8A                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              // 139   0x8B                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              // 140   0x8C                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              // 141   0x8D                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              // 142   0x8E                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              // 143   0x8F                       | -- unassigned --
    sad::KeyboardKey::NumLock,              // 144   0x90  VK_NUMLOCK           | Num Lock key
    sad::KeyboardKey::ScrollLock,           // 145   0x91  VK_SCROLL            | Scroll Lock key
                               // Fujitsu/OASYS kbd --------------------
    sad::KeyboardKey::KeyNone,              // 146   0x92  VK_OEM_FJ_JISHO      | 'Dictionary' key /
                               //             VK_OEM_NEC_EQUAL     | key on numpad on NEC PC-9800 kbd
    sad::KeyboardKey::KeyNone,              // 147   0x93  VK_OEM_FJ_MASSHOU    | 'Unregister word' key
    sad::KeyboardKey::KeyNone,              // 148   0x94  VK_OEM_FJ_TOUROKU    | 'Register word' key
    sad::KeyboardKey::KeyNone,              // 149   0x95  VK_OEM_FJ_LOYA       | 'Left OYAYUBI' key
    sad::KeyboardKey::KeyNone,              // 150   0x96  VK_OEM_FJ_ROYA       | 'Right OYAYUBI' key
    sad::KeyboardKey::KeyNone,              // 151   0x97                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              // 152   0x98                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              // 153   0x99                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              // 154   0x9A                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              // 155   0x9B                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              // 156   0x9C                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              // 157   0x9D                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              // 158   0x9E                       | -- unassigned --
    sad::KeyboardKey::KeyNone,              // 159   0x9F                       | -- unassigned --
    sad::KeyboardKey::LeftShift,            // 160   0xA0  VK_LSHIFT            | Left Shift key
    sad::KeyboardKey::RightShift,           // 161   0xA1  VK_RSHIFT            | Right Shift key
    sad::KeyboardKey::LeftControl,          // 162   0xA2  VK_LCONTROL          | Left Ctrl key
    sad::KeyboardKey::RightControl,         // 163   0xA3  VK_RCONTROL          | Right Ctrl key
    sad::KeyboardKey::LeftAlt,              // 164   0xA4  VK_LMENU             | Left Menu key
    sad::KeyboardKey::RightAlt,             // 165   0xA5  VK_RMENU             | Right Menu key
    sad::KeyboardKey::KeyNone,              // 166
    sad::KeyboardKey::KeyNone,              // 167
    sad::KeyboardKey::KeyNone,              // 168
    sad::KeyboardKey::KeyNone,              // 169
    sad::KeyboardKey::KeyNone,              // 170
    sad::KeyboardKey::KeyNone,              // 171
    sad::KeyboardKey::KeyNone,              // 172
    sad::KeyboardKey::KeyNone,              // 173
    sad::KeyboardKey::KeyNone,              // 174
    sad::KeyboardKey::KeyNone,              // 175
    sad::KeyboardKey::KeyNone,              // 176
    sad::KeyboardKey::KeyNone,              // 177
    sad::KeyboardKey::KeyNone,              // 178
    sad::KeyboardKey::KeyNone,              // 179
    sad::KeyboardKey::KeyNone,              // 180
    sad::KeyboardKey::KeyNone,              // 181
    sad::KeyboardKey::KeyNone,              // 182
    sad::KeyboardKey::KeyNone,              // 183
    sad::KeyboardKey::KeyNone,              // 184
    sad::KeyboardKey::KeyNone,              // 185
    sad::KeyboardKey::Semicolon,            // 186   0xBA  VK_OEM_1
    sad::KeyboardKey::Equal,                // 187   0xBB  VK_OEM_PLUS
    sad::KeyboardKey::Comma,                // 188   0xBC  VK_OEM_COMMA
    sad::KeyboardKey::Minus,                // 189   0xBD  VK_OEM_MINUS
    sad::KeyboardKey::Period,               // 190   0xBE  VK_OEM_PERIOD
    sad::KeyboardKey::Slash,                // 191   0xBF  VK_OEM_2
    sad::KeyboardKey::Tilde,                // 192   0xC0  VK_OEM_3
    sad::KeyboardKey::KeyNone,              // 193
    sad::KeyboardKey::KeyNone,              // 194
    sad::KeyboardKey::KeyNone,              // 195
    sad::KeyboardKey::KeyNone,              // 196
    sad::KeyboardKey::KeyNone,              // 197
    sad::KeyboardKey::KeyNone,              // 198
    sad::KeyboardKey::KeyNone,              // 199
    sad::KeyboardKey::KeyNone,              // 200
    sad::KeyboardKey::KeyNone,              // 201
    sad::KeyboardKey::KeyNone,              // 202
    sad::KeyboardKey::KeyNone,              // 203
    sad::KeyboardKey::KeyNone,              // 204
    sad::KeyboardKey::KeyNone,              // 205
    sad::KeyboardKey::KeyNone,              // 206
    sad::KeyboardKey::KeyNone,              // 207
    sad::KeyboardKey::KeyNone,              // 208
    sad::KeyboardKey::KeyNone,              // 209
    sad::KeyboardKey::KeyNone,              // 210
    sad::KeyboardKey::KeyNone,              // 211
    sad::KeyboardKey::KeyNone,              // 212
    sad::KeyboardKey::KeyNone,              // 213
    sad::KeyboardKey::KeyNone,              // 214
    sad::KeyboardKey::KeyNone,              // 215
    sad::KeyboardKey::KeyNone,              // 216
    sad::KeyboardKey::KeyNone,              // 217
    sad::KeyboardKey::KeyNone,              // 218
    sad::KeyboardKey::OpeningSquareBracket, // 219   0xDB VK_OEM_4
    sad::KeyboardKey::BackSlash,            // 220   0xDC VK_OEM_5
    sad::KeyboardKey::ClosingSquareBracket, // 221   0xDD VK_OEM_6
    sad::KeyboardKey::Apostrophe            // 222   0xDE VK_OEM_6
};



#endif


#ifdef X11


#ifdef  XK_CYRILLIC
const int totalmappingsize = 522;
#else
const int totalmappingsize = 392;
#endif

unsigned int mapping[totalmappingsize] = {
    XK_Escape, static_cast<int>(sad::KeyboardKey::Esc),
    XK_F1,     static_cast<int>(sad::KeyboardKey::F1),
    XK_F2,     static_cast<int>(sad::KeyboardKey::F2),
    XK_F3,     static_cast<int>(sad::KeyboardKey::F3),
    XK_F4,     static_cast<int>(sad::KeyboardKey::F4),
    XK_F5,     static_cast<int>(sad::KeyboardKey::F5),
    XK_F6,     static_cast<int>(sad::KeyboardKey::F6),
    XK_F7,     static_cast<int>(sad::KeyboardKey::F7),
    XK_F8,     static_cast<int>(sad::KeyboardKey::F8),
    XK_F9,     static_cast<int>(sad::KeyboardKey::F9),
    XK_F10,    static_cast<int>(sad::KeyboardKey::F10),
    XK_F11,    static_cast<int>(sad::KeyboardKey::F11),
    XK_F12,    static_cast<int>(sad::KeyboardKey::F12),
    // Most WM hook this key so binding on it still have no purpose.
    XK_Sys_Req,        static_cast<int>(sad::KeyboardKey::PrintScreen),
    XK_Scroll_Lock,    static_cast<int>(sad::KeyboardKey::ScrollLock),
    XK_Pause,          static_cast<int>(sad::KeyboardKey::Pause),
    XK_asciitilde,     static_cast<int>(sad::KeyboardKey::Tilde),
    XK_grave,          static_cast<int>(sad::KeyboardKey::Tilde),
#ifdef XK_CYRILLIC
    XK_Cyrillic_io,    static_cast<int>(sad::KeyboardKey::Tilde),
#endif
    XK_1,              static_cast<int>(sad::KeyboardKey::Numeric1),
    XK_exclam,         static_cast<int>(sad::KeyboardKey::Numeric1),
    XK_2,              static_cast<int>(sad::KeyboardKey::Numeric2),
    XK_at,             static_cast<int>(sad::KeyboardKey::Numeric2),
    XK_3,              static_cast<int>(sad::KeyboardKey::Numeric3),
    XK_4,              static_cast<int>(sad::KeyboardKey::Numeric4),
    XK_5,              static_cast<int>(sad::KeyboardKey::Numeric5),
    XK_6,              static_cast<int>(sad::KeyboardKey::Numeric6),
    XK_7,              static_cast<int>(sad::KeyboardKey::Numeric7),
    XK_8,              static_cast<int>(sad::KeyboardKey::Numeric8),
    XK_9,              static_cast<int>(sad::KeyboardKey::Numeric9),
    XK_0,              static_cast<int>(sad::KeyboardKey::Numeric0),
    XK_minus,          static_cast<int>(sad::KeyboardKey::Minus),
    XK_underscore,     static_cast<int>(sad::KeyboardKey::Minus),
    XK_equal,          static_cast<int>(sad::KeyboardKey::Equal),
    XK_plus,           static_cast<int>(sad::KeyboardKey::Equal),
    XK_backslash,      static_cast<int>(sad::KeyboardKey::BackSlash),
    XK_bar,            static_cast<int>(sad::KeyboardKey::BackSlash),
    XK_BackSpace,      static_cast<int>(sad::KeyboardKey::Backspace),
    XK_Insert,         static_cast<int>(sad::KeyboardKey::Insert),
    XK_Home,           static_cast<int>(sad::KeyboardKey::Home),
    XK_Prior,          static_cast<int>(sad::KeyboardKey::PageUp),
    XK_Num_Lock,       static_cast<int>(sad::KeyboardKey::NumLock),
    XK_Tab,            static_cast<int>(sad::KeyboardKey::Tab),
    XK_a, static_cast<int>(sad::KeyboardKey::A),
    XK_A, static_cast<int>(sad::KeyboardKey::A),
#ifdef XK_CYRILLIC
    XK_Cyrillic_ef, static_cast<int>(sad::KeyboardKey::A),
    XK_Cyrillic_EF, static_cast<int>(sad::KeyboardKey::A),
#endif
    XK_b, static_cast<int>(sad::KeyboardKey::B),
    XK_B, static_cast<int>(sad::KeyboardKey::B),
#ifdef XK_CYRILLIC
    XK_Cyrillic_i, static_cast<int>(sad::KeyboardKey::B),
    XK_Cyrillic_I, static_cast<int>(sad::KeyboardKey::B),
#endif
    XK_c, static_cast<int>(sad::KeyboardKey::C),
    XK_C, static_cast<int>(sad::KeyboardKey::C),
#ifdef XK_CYRILLIC
    XK_Cyrillic_es, static_cast<int>(sad::KeyboardKey::C),
    XK_Cyrillic_ES, static_cast<int>(sad::KeyboardKey::C),
#endif
    XK_d, static_cast<int>(sad::KeyboardKey::D),
    XK_D, static_cast<int>(sad::KeyboardKey::D),
#ifdef XK_CYRILLIC
    XK_Cyrillic_ve, static_cast<int>(sad::KeyboardKey::D),
    XK_Cyrillic_VE, static_cast<int>(sad::KeyboardKey::D),
#endif
    XK_e, static_cast<int>(sad::KeyboardKey::E),
    XK_E, static_cast<int>(sad::KeyboardKey::E),
#ifdef XK_CYRILLIC
    XK_Cyrillic_u, static_cast<int>(sad::KeyboardKey::E),
    XK_Cyrillic_U, static_cast<int>(sad::KeyboardKey::E),
#endif
    XK_f, static_cast<int>(sad::KeyboardKey::F),
    XK_F, static_cast<int>(sad::KeyboardKey::F),
#ifdef XK_CYRILLIC
    XK_Cyrillic_a, static_cast<int>(sad::KeyboardKey::F),
    XK_Cyrillic_A, static_cast<int>(sad::KeyboardKey::F),
#endif
    XK_g, static_cast<int>(sad::KeyboardKey::G),
    XK_G, static_cast<int>(sad::KeyboardKey::G),
#ifdef XK_CYRILLIC
    XK_Cyrillic_pe, static_cast<int>(sad::KeyboardKey::G),
    XK_Cyrillic_PE, static_cast<int>(sad::KeyboardKey::G),
#endif
    XK_h, static_cast<int>(sad::KeyboardKey::H),
    XK_H, static_cast<int>(sad::KeyboardKey::H),
#ifdef XK_CYRILLIC
    XK_Cyrillic_er, static_cast<int>(sad::KeyboardKey::H),
    XK_Cyrillic_ER, static_cast<int>(sad::KeyboardKey::H),
#endif
    XK_i, static_cast<int>(sad::KeyboardKey::I),
    XK_I, static_cast<int>(sad::KeyboardKey::I),
#ifdef XK_CYRILLIC
    XK_Cyrillic_sha, static_cast<int>(sad::KeyboardKey::I),
    XK_Cyrillic_SHA, static_cast<int>(sad::KeyboardKey::I),
#endif
    XK_j, static_cast<int>(sad::KeyboardKey::J),
    XK_J, static_cast<int>(sad::KeyboardKey::J),
#ifdef XK_CYRILLIC
    XK_Cyrillic_o, static_cast<int>(sad::KeyboardKey::J),
    XK_Cyrillic_O, static_cast<int>(sad::KeyboardKey::J),
#endif
    XK_k, static_cast<int>(sad::KeyboardKey::K),
    XK_K, static_cast<int>(sad::KeyboardKey::K),
#ifdef XK_CYRILLIC
    XK_Cyrillic_el, static_cast<int>(sad::KeyboardKey::K),
    XK_Cyrillic_EL, static_cast<int>(sad::KeyboardKey::K),
#endif
    XK_l, static_cast<int>(sad::KeyboardKey::L),
    XK_L, static_cast<int>(sad::KeyboardKey::L),
#ifdef XK_CYRILLIC
    XK_Cyrillic_de, static_cast<int>(sad::KeyboardKey::L),
    XK_Cyrillic_DE, static_cast<int>(sad::KeyboardKey::L),
#endif
    XK_m, static_cast<int>(sad::KeyboardKey::M),
    XK_M, static_cast<int>(sad::KeyboardKey::M),
#ifdef XK_CYRILLIC
    XK_Cyrillic_softsign, static_cast<int>(sad::KeyboardKey::M),
    XK_Cyrillic_SOFTSIGN, static_cast<int>(sad::KeyboardKey::M),
#endif
    XK_n, static_cast<int>(sad::KeyboardKey::N),
    XK_N, static_cast<int>(sad::KeyboardKey::N),
#ifdef XK_CYRILLIC
    XK_Cyrillic_te, static_cast<int>(sad::KeyboardKey::N),
    XK_Cyrillic_TE, static_cast<int>(sad::KeyboardKey::N),
#endif
    XK_o, static_cast<int>(sad::KeyboardKey::O),
    XK_O, static_cast<int>(sad::KeyboardKey::O),
#ifdef XK_CYRILLIC
    XK_Cyrillic_shcha, static_cast<int>(sad::KeyboardKey::O),
    XK_Cyrillic_SHCHA, static_cast<int>(sad::KeyboardKey::O),
#endif
    XK_p, static_cast<int>(sad::KeyboardKey::P),
    XK_P, static_cast<int>(sad::KeyboardKey::P),
#ifdef XK_CYRILLIC
    XK_Cyrillic_ze, static_cast<int>(sad::KeyboardKey::P),
    XK_Cyrillic_ZE, static_cast<int>(sad::KeyboardKey::P),
#endif
    XK_q, static_cast<int>(sad::KeyboardKey::Q),
    XK_Q, static_cast<int>(sad::KeyboardKey::Q),
#ifdef XK_CYRILLIC
    XK_Cyrillic_shorti, static_cast<int>(sad::KeyboardKey::Q),
    XK_Cyrillic_SHORTI, static_cast<int>(sad::KeyboardKey::Q),
#endif
    XK_r, static_cast<int>(sad::KeyboardKey::R),
    XK_R, static_cast<int>(sad::KeyboardKey::R),
#ifdef XK_CYRILLIC
    XK_Cyrillic_ka, static_cast<int>(sad::KeyboardKey::R),
    XK_Cyrillic_KA, static_cast<int>(sad::KeyboardKey::R),
#endif
    XK_s, static_cast<int>(sad::KeyboardKey::S),
    XK_S, static_cast<int>(sad::KeyboardKey::S),
#ifdef XK_CYRILLIC
    XK_Cyrillic_yeru, static_cast<int>(sad::KeyboardKey::S),
    XK_Cyrillic_YERU, static_cast<int>(sad::KeyboardKey::S),
#endif
    XK_t, static_cast<int>(sad::KeyboardKey::T),
    XK_T, static_cast<int>(sad::KeyboardKey::T),
#ifdef XK_CYRILLIC
    XK_Cyrillic_ie, static_cast<int>(sad::KeyboardKey::T),
    XK_Cyrillic_IE, static_cast<int>(sad::KeyboardKey::T),
#endif

    XK_u, static_cast<int>(sad::KeyboardKey::U),
    XK_U, static_cast<int>(sad::KeyboardKey::U),
#ifdef XK_CYRILLIC
    XK_Cyrillic_ghe, static_cast<int>(sad::KeyboardKey::U),
    XK_Cyrillic_GHE, static_cast<int>(sad::KeyboardKey::U),
#endif
    XK_v, static_cast<int>(sad::KeyboardKey::V),
    XK_V, static_cast<int>(sad::KeyboardKey::V),
#ifdef XK_CYRILLIC
    XK_Cyrillic_em, static_cast<int>(sad::KeyboardKey::V),
    XK_Cyrillic_EM, static_cast<int>(sad::KeyboardKey::V),
#endif
    XK_w, static_cast<int>(sad::KeyboardKey::W),
    XK_W, static_cast<int>(sad::KeyboardKey::W),
#ifdef XK_CYRILLIC
    XK_Cyrillic_tse, static_cast<int>(sad::KeyboardKey::W),
    XK_Cyrillic_TSE, static_cast<int>(sad::KeyboardKey::W),
#endif
    XK_x, static_cast<int>(sad::KeyboardKey::X),
    XK_X, static_cast<int>(sad::KeyboardKey::X),
#ifdef XK_CYRILLIC
    XK_Cyrillic_che, static_cast<int>(sad::KeyboardKey::X),
    XK_Cyrillic_CHE, static_cast<int>(sad::KeyboardKey::X),
#endif
    XK_y, static_cast<int>(sad::KeyboardKey::Y),
    XK_Y, static_cast<int>(sad::KeyboardKey::Y),
#ifdef XK_CYRILLIC
    XK_Cyrillic_en, static_cast<int>(sad::KeyboardKey::Y),
    XK_Cyrillic_EN, static_cast<int>(sad::KeyboardKey::Y),
#endif
    XK_z, static_cast<int>(sad::KeyboardKey::Z),
    XK_Z, static_cast<int>(sad::KeyboardKey::Z),
#ifdef XK_CYRILLIC
    XK_Cyrillic_ya, static_cast<int>(sad::KeyboardKey::Z),
    XK_Cyrillic_YA, static_cast<int>(sad::KeyboardKey::Z),
#endif
    XK_bracketleft,    static_cast<int>(sad::KeyboardKey::OpeningSquareBracket),
    XK_braceleft,      static_cast<int>(sad::KeyboardKey::OpeningSquareBracket),
#ifdef XK_CYRILLIC
    XK_Cyrillic_ha,    static_cast<int>(sad::KeyboardKey::OpeningSquareBracket),
    XK_Cyrillic_HA,    static_cast<int>(sad::KeyboardKey::OpeningSquareBracket),
#endif
    XK_bracketright,    static_cast<int>(sad::KeyboardKey::ClosingSquareBracket),
    XK_braceright,      static_cast<int>(sad::KeyboardKey::ClosingSquareBracket),
#ifdef XK_CYRILLIC
    XK_Cyrillic_hardsign,    static_cast<int>(sad::KeyboardKey::ClosingSquareBracket),
    XK_Cyrillic_HARDSIGN,    static_cast<int>(sad::KeyboardKey::ClosingSquareBracket),
#endif
    XK_semicolon,   static_cast<int>(sad::KeyboardKey::Semicolon),
    XK_colon,       static_cast<int>(sad::KeyboardKey::Semicolon),
#ifdef XK_CYRILLIC
    XK_Cyrillic_zhe,    static_cast<int>(sad::KeyboardKey::Semicolon),
    XK_Cyrillic_ZHE,    static_cast<int>(sad::KeyboardKey::Semicolon),
#endif
    XK_apostrophe,   static_cast<int>(sad::KeyboardKey::Apostrophe),
    XK_quotedbl,       static_cast<int>(sad::KeyboardKey::Apostrophe),
#ifdef XK_CYRILLIC
    XK_Cyrillic_e,    static_cast<int>(sad::KeyboardKey::Apostrophe),
    XK_Cyrillic_E,    static_cast<int>(sad::KeyboardKey::Apostrophe),
#endif
    XK_comma,    static_cast<int>(sad::KeyboardKey::Comma),
    XK_less,     static_cast<int>(sad::KeyboardKey::Comma),
#ifdef XK_CYRILLIC
    XK_Cyrillic_be,    static_cast<int>(sad::KeyboardKey::Comma),
    XK_Cyrillic_BE,    static_cast<int>(sad::KeyboardKey::Comma),
#endif
    XK_period,   static_cast<int>(sad::KeyboardKey::Period),
    XK_greater,  static_cast<int>(sad::KeyboardKey::Period),
#ifdef XK_CYRILLIC
    XK_Cyrillic_yu,    static_cast<int>(sad::KeyboardKey::Period),
    XK_Cyrillic_YU,    static_cast<int>(sad::KeyboardKey::Period),
#endif
    XK_slash,          static_cast<int>(sad::KeyboardKey::Slash),
    XK_question,       static_cast<int>(sad::KeyboardKey::Slash),
    XK_Return,         static_cast<int>(sad::KeyboardKey::Enter),
    XK_Delete,         static_cast<int>(sad::KeyboardKey::Delete),
    XK_End,            static_cast<int>(sad::KeyboardKey::End),
    XK_Next,           static_cast<int>(sad::KeyboardKey::PageDown),

    XK_Caps_Lock,      static_cast<int>(sad::KeyboardKey::CapsLock),
    XK_Shift_L,        static_cast<int>(sad::KeyboardKey::LeftShift),
    XK_Shift_R,        static_cast<int>(sad::KeyboardKey::RightShift),
    XK_Control_L,      static_cast<int>(sad::KeyboardKey::LeftControl),
    XK_Control_R,      static_cast<int>(sad::KeyboardKey::RightControl),
    XK_Super_L,        static_cast<int>(sad::KeyboardKey::WindowsKey),
    XK_Super_R,        static_cast<int>(sad::KeyboardKey::WindowsKey),
    XK_Alt_L,          static_cast<int>(sad::KeyboardKey::LeftAlt),
    XK_Alt_R,          static_cast<int>(sad::KeyboardKey::RightAlt),
    XK_space,          static_cast<int>(sad::KeyboardKey::Space),
    XK_Menu,           static_cast<int>(sad::KeyboardKey::AppKey),
    XK_Left,           static_cast<int>(sad::KeyboardKey::KeyLeft),
    XK_Up,             static_cast<int>(sad::KeyboardKey::KeyUp),
    XK_Down,           static_cast<int>(sad::KeyboardKey::KeyDown),
    XK_Left,           static_cast<int>(sad::KeyboardKey::KeyLeft),
    XK_Right,          static_cast<int>(sad::KeyboardKey::KeyRight)
};

#endif


void sad::os::KeyDecoder::init()
{
#ifdef WIN32
    for(unsigned int i = 0; i < syskeycount; i++)
    {
        m_table.insert(static_cast<sad::os::SystemKey>(i), syskeys[i]);
    }
    ZeroMemory(m_key_states, 256);
#endif

#ifdef X11
    unsigned int i = 0;
    for(i = 0; i < totalmappingsize; i += 2)
    {
        sad::os::SystemKey syskey = static_cast<sad::os::SystemKey>(mapping[i]);
        sad::KeyboardKey sadkey = static_cast<sad::KeyboardKey>(mapping[i+1)];
        m_table.insert(syskey, sadkey);
    }
#endif
}


sad::KeyboardKey sad::os::KeyDecoder::decode(sad::os::SystemWindowEvent * e)
{
#ifdef WIN32
    sad::KeyboardKey key = sad::KeyboardKey::KeyNone;
    if (m_table.contains(e->WParam))
    {
        key = m_table[e->WParam];
    }
    return key;
#endif

#ifdef X11
    sad::KeyboardKey key = sad::KeyboardKey::KeyNone;
    ::KeySym keysym = XLookupKeysym(&(e->Event.xkey), 0);
    if (m_table.contains(keysym))
    {
        key = m_table[keysym];
    }
    return key;
#endif
}

// Taken from https://github.com/benkasminbullock/unicode-c/blob/master/unicode.c
#define UNI_SUR_HIGH_START  0xD800
#define UNI_SUR_HIGH_END    0xDBFF
#define UNI_SUR_LOW_START   0xDC00
#define UNI_SUR_LOW_END     0xDFFF
#define UNICODE_SURROGATE_PAIR -2
#define UNICODE_UTF8_4 0x1fffff
#define UNICODE_TOO_BIG -7
int32_t ucs2_to_utf8 (int32_t ucs2, unsigned char * utf8)
{
    if (ucs2 < 0x80) {
        utf8[0] = ucs2;
        utf8[1] = '\0';
        return 1;
    }
    if (ucs2 < 0x800) {
        utf8[0] = (ucs2 >> 6)   | 0xC0;
        utf8[1] = (ucs2 & 0x3F) | 0x80;
        utf8[2] = '\0';
        return 2;
    }
    if (ucs2 < 0xFFFF) {
        utf8[0] = ((ucs2 >> 12)       ) | 0xE0;
        utf8[1] = ((ucs2 >> 6 ) & 0x3F) | 0x80;
        utf8[2] = ((ucs2      ) & 0x3F) | 0x80;
        utf8[3] = '\0';
    if (ucs2 >= UNI_SUR_HIGH_START && ucs2 <= UNI_SUR_LOW_END) {
        /* Ill-formed. */
        return UNICODE_SURROGATE_PAIR;
    }
        return 3;
    }
    if (ucs2 <= UNICODE_UTF8_4) {
    /* http://tidy.sourceforge.net/cgi-bin/lxr/source/src/utf8.c#L380 */
    utf8[0] = 0xF0 | (ucs2 >> 18);
    utf8[1] = 0x80 | ((ucs2 >> 12) & 0x3F);
    utf8[2] = 0x80 | ((ucs2 >> 6) & 0x3F);
    utf8[3] = 0x80 | ((ucs2 & 0x3F));
        utf8[4] = '\0';
        return 4;
    }
    return UNICODE_TOO_BIG;
}

sad::Maybe<sad::String>  sad::os::KeyDecoder::convert(sad::os::SystemWindowEvent * e, sad::Window * win)
{
    sad::KeyboardKey key = this->decode(e);
    sad::Maybe<sad::String>  result;
    if (isReadable(key) == false)
        return result;
#ifdef WIN32
    GetKeyboardState(m_key_states);
    char buffer[20] = "", buffer_utf8[20];
    HKL kl = GetKeyboardLayout(GetCurrentThreadId());
    ZeroMemory(buffer, 20);
    ToUnicodeEx(e->WParam, e->LParam, m_key_states, (LPWSTR)buffer, 5, 0, kl);
    // Man, this looks bad. But still, what could we do?
    ucs2_to_utf8(*reinterpret_cast<int32_t*>(buffer), reinterpret_cast<unsigned char*>(buffer_utf8));
    //buffer[1] = 0;
    result.setValue(buffer_utf8);

    m_key_states[e->WParam] = 0;
    m_key_states[VK_SHIFT] = 0;
    m_key_states[VK_CONTROL] = 0;
    m_key_states[VK_LCONTROL] = 0;
    m_key_states[VK_MENU] = 0;
    m_key_states[VK_RMENU] = 0;
    m_key_states[VK_CAPITAL] = 0;


#endif

#ifdef X11
    const int bufferlength = 150;
    char buffer[bufferlength];
    memset(buffer, 0, 150);
    Status status = 0;
    int length = Xutf8LookupString(win->handles()->IC, reinterpret_cast<XKeyPressedEvent*>(&(e->Event)), buffer, bufferlength, &m_key_sym, &status);
    if (length != 0)
    {
        result.setValue(buffer);
    }
    if (m_key_sym == XK_space)
    {
        result.setValue(" ");
    }
#endif

    return result;
}

const int readablekeyscount = 12;
sad::KeyboardKey readablekeys[readablekeyscount] = {
    sad::KeyboardKey::Minus,
    sad::KeyboardKey::Equal,

    sad::KeyboardKey::BackSlash,
    sad::KeyboardKey::Tilde,
    sad::KeyboardKey::OpeningSquareBracket,

    sad::KeyboardKey::ClosingSquareBracket,
    sad::KeyboardKey::Semicolon,
    sad::KeyboardKey::Comma,

    sad::KeyboardKey::Period,
    sad::KeyboardKey::Slash,
    sad::KeyboardKey::Apostrophe,
    sad::KeyboardKey::Space
};

bool sad::os::KeyDecoder::isReadable(sad::KeyboardKey key)
{
    bool is_readable = (static_cast<int>(key) >= '0' && static_cast<int>(key) <= '9');
    is_readable = is_readable || (static_cast<int>(key) >= 'A' && static_cast<int>(key) <= 'Z');
    for(int i = 0 ; i < readablekeyscount; i++)
    {
        is_readable = is_readable || (key == readablekeys[i]);
    }
    return is_readable;
}
