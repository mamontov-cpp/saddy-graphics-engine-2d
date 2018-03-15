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
    sad::KeyNone,              //   0   0x00
    sad::KeyNone,              //   1   0x01   VK_LBUTTON          | Left mouse button
    sad::KeyNone,              //   2   0x02   VK_RBUTTON          | Right mouse button
    sad::KeyNone,              //   3   0x03   VK_CANCEL           | Control-Break processing
    sad::KeyNone,              //   4   0x04   VK_MBUTTON          | Middle mouse button
    sad::KeyNone,              //   5   0x05   VK_XBUTTON1         | X1 mouse button
    sad::KeyNone,              //   6   0x06   VK_XBUTTON2         | X2 mouse button
    sad::KeyNone,              //   7   0x07                       | -- unassigned --
    sad::Backspace,            //   8   0x08   VK_BACK             | BackSpace key
    sad::Tab,                  //   9   0x09   VK_TAB              | Tab key
    sad::KeyNone,              //  10   0x0A                       | -- reserved --
    sad::KeyNone,              //  11   0x0B                       | -- reserved --
    sad::KeyNone,              //  12   0x0C   VK_CLEAR            | Clear key
    sad::Enter  ,              //  13   0x0D   VK_RETURN           | Enter key
    sad::KeyNone,              //  14   0x0E                       | -- unassigned --
    sad::KeyNone,              //  15   0x0F                       | -- unassigned --
    sad::LeftShift,            //  16   0x10   VK_SHIFT            | Shift key
    sad::LeftControl,          //  17   0x11   VK_CONTROL          | Ctrl key
    sad::LeftAlt,              //  18   0x12   VK_MENU             | Alt key
    sad::Pause,                //  19   0x13   VK_PAUSE            | Pause key
    sad::CapsLock,             //  20   0x14   VK_CAPITAL          | Caps-Lock
    sad::KeyNone,              //  21   0x15   VK_KANA / VK_HANGUL | IME Kana or Hangul mode
    sad::KeyNone,              //  22   0x16                       | -- unassigned --
    sad::KeyNone,              //  23   0x17   VK_JUNJA            | IME Junja mode
    sad::KeyNone,              //  24   0x18   VK_FINAL            | IME final mode
    sad::KeyNone,              //  25   0x19   VK_HANJA / VK_KANJI | IME Hanja or Kanji mode
    sad::KeyNone,              //  26   0x1A                       | -- unassigned --
    sad::Esc,                  //  27   0x1B   VK_ESCAPE           | Esc key
    sad::KeyNone,              //  28   0x1C   VK_CONVERT          | IME convert
    sad::KeyNone,              //  29   0x1D   VK_NONCONVERT       | IME non-convert
    sad::KeyNone,              //  30   0x1E   VK_ACCEPT           | IME accept
    sad::KeyNone,              //  31   0x1F   VK_MODECHANGE       | IME mode change request
    sad::Space,                //  32   0x20   VK_SPACE            | Spacebar
    sad::PageUp,               //  33   0x21   VK_PRIOR            | Page Up key
    sad::PageDown,             //  34   0x22   VK_NEXT             | Page Down key
    sad::End,                  //  35   0x23   VK_END              | End key
    sad::Home,                 //  36   0x24   VK_HOME             | Home key
    sad::KeyLeft,              //  37   0x25   VK_LEFT             | Left arrow key
    sad::KeyUp,                //  38   0x26   VK_UP               | Up arrow key
    sad::KeyRight,             //  39   0x27   VK_RIGHT            | Right arrow key
    sad::KeyDown,              //  40   0x28   VK_DOWN             | Down arrow key
    sad::KeyNone,              //  41   0x29   VK_SELECT           | Select key
    sad::KeyNone,              //  42   0x2A   VK_PRINT            | Print key
    sad::KeyNone,              //  43   0x2B   VK_EXECUTE          | Execute key
    sad::PrintScreen,          //  44   0x2C   VK_SNAPSHOT         | Print Screen key
    sad::Insert,               //  45   0x2D   VK_INSERT           | Ins key
    sad::Delete,               //  46   0x2E   VK_DELETE           | Del key
    sad::KeyNone,              //  47   0x2F   VK_HELP             | Help key
    sad::Numeric0,             //  48   0x30   (VK_0)              | 0 key
    sad::Numeric1,             //  49   0x31   (VK_1)              | 1 key
    sad::Numeric2,             //  50   0x32   (VK_2)              | 2 key
    sad::Numeric3,             //  51   0x33   (VK_3)              | 3 key
    sad::Numeric4,             //  52   0x34   (VK_4)              | 4 key
    sad::Numeric5,             //  53   0x35   (VK_5)              | 5 key
    sad::Numeric6,             //  54   0x36   (VK_6)              | 6 key
    sad::Numeric7,             //  55   0x37   (VK_7)              | 7 key
    sad::Numeric8,             //  56   0x38   (VK_8)              | 8 key
    sad::Numeric9,             //  57   0x39   (VK_9)              | 9 key
    sad::KeyNone,              //  58   0x3A                       | -- unassigned --
    sad::KeyNone,              //  59   0x3B                       | -- unassigned --
    sad::KeyNone,              //  60   0x3C                       | -- unassigned --
    sad::KeyNone,              //  61   0x3D                       | -- unassigned --
    sad::KeyNone,              //  62   0x3E                       | -- unassigned --
    sad::KeyNone,              //  63   0x3F                       | -- unassigned --
    sad::KeyNone,              //  64   0x40                       | -- unassigned --
    sad::A,                    //  65   0x41   (VK_A)              | A key
    sad::B,                    //  66   0x42   (VK_B)              | B key
    sad::C,                    //  67   0x43   (VK_C)              | C key
    sad::D,                    //  68   0x44   (VK_D)              | D key
    sad::E,                    //  69   0x45   (VK_E)              | E key
    sad::F,                    //  70   0x46   (VK_F)              | F key
    sad::G,                    //  71   0x47   (VK_G)              | G key
    sad::H,                    //  72   0x48   (VK_H)              | H key
    sad::I,                    //  73   0x49   (VK_I)              | I key
    sad::J,                    //  74   0x4A   (VK_J)              | J key
    sad::K,                    //  75   0x4B   (VK_K)              | K key
    sad::L,                    //  76   0x4C   (VK_L)              | L key
    sad::M,                    //  77   0x4D   (VK_M)              | M key
    sad::N,                    //  78   0x4E   (VK_N)              | N key
    sad::O,                    //  79   0x4F   (VK_O)              | O key
    sad::P,                    //  80   0x50   (VK_P)              | P key
    sad::Q,                    //  81   0x51   (VK_Q)              | Q key
    sad::R,                    //  82   0x52   (VK_R)              | R key
    sad::S,                    //  83   0x53   (VK_S)              | S key
    sad::T,                    //  84   0x54   (VK_T)              | T key
    sad::U,                    //  85   0x55   (VK_U)              | U key
    sad::V,                    //  86   0x56   (VK_V)              | V key
    sad::W,                    //  87   0x57   (VK_W)              | W key
    sad::X,                    //  88   0x58   (VK_X)              | X key
    sad::Y,                    //  89   0x59   (VK_Y)              | Y key
    sad::Z,                    //  90   0x5A   (VK_Z)              | Z key
    sad::WindowsKey,           //  91   0x5B   VK_LWIN             | Left Windows  - MS Natural kbd
    sad::WindowsKey,           //  92   0x5C   VK_RWIN             | Right Windows - MS Natural kbd
    sad::AppKey,               //  93   0x5D   VK_APPS             | Application key-MS Natural kbd
    sad::KeyNone,              //  94   0x5E   -- reserved --
    sad::KeyNone,              //  95   0x5F   VK_SLEEP
    sad::Numeric0,             //  96   0x60   VK_NUMPAD0          | Numeric keypad 0 key
    sad::Numeric1,             //  97   0x61   VK_NUMPAD1          | Numeric keypad 1 key
    sad::Numeric2,             //  98   0x62   VK_NUMPAD2          | Numeric keypad 2 key
    sad::Numeric3,             //  99   0x63   VK_NUMPAD3          | Numeric keypad 3 key
    sad::Numeric4,             // 100   0x64   VK_NUMPAD4          | Numeric keypad 4 key
    sad::Numeric5,             // 101   0x65   VK_NUMPAD5          | Numeric keypad 5 key
    sad::Numeric6,             // 102   0x66   VK_NUMPAD6          | Numeric keypad 6 key
    sad::Numeric7,             // 103   0x67   VK_NUMPAD7          | Numeric keypad 7 key
    sad::Numeric8,             // 104   0x68   VK_NUMPAD8          | Numeric keypad 8 key
    sad::Numeric9,             // 105   0x69   VK_NUMPAD9          | Numeric keypad 9 key
    sad::KeyNone,              // 106   0x6A   VK_MULTIPLY         | Multiply key
    sad::KeyNone,              // 107   0x6B   VK_ADD              | Add key
    sad::KeyNone,              // 108   0x6C   VK_SEPARATOR        | Separator key
    sad::KeyNone,              // 109   0x6D   VK_SUBTRACT         | Subtract key
    sad::KeyNone,              // 110   0x6E   VK_DECIMAL          | Decimal key
    sad::KeyNone,              // 111   0x6F   VK_DIVIDE           | Divide key
    sad::F1,                   // 112   0x70   VK_F1               | F1 key
    sad::F2,                   // 113   0x71   VK_F2               | F2 key
    sad::F3,                   // 114   0x72   VK_F3               | F3 key
    sad::F4,                   // 115   0x73   VK_F4               | F4 key
    sad::F5,                   // 116   0x74   VK_F5               | F5 key
    sad::F6,                   // 117   0x75   VK_F6               | F6 key
    sad::F7,                   // 118   0x76   VK_F7               | F7 key
    sad::F8,                   // 119   0x77   VK_F8               | F8 key
    sad::F9,                   // 120   0x78   VK_F9               | F9 key
    sad::F10,                  // 121   0x79   VK_F10              | F10 key
    sad::F11,                  // 122   0x7A   VK_F11              | F11 key
    sad::F12,                  // 123   0x7B   VK_F12              | F12 key
    sad::KeyNone,              // 124   0x7C   VK_F13              | F13 key
    sad::KeyNone,              // 125   0x7D   VK_F14              | F14 key
    sad::KeyNone,              // 126   0x7E   VK_F15              | F15 key
    sad::KeyNone,              // 127   0x7F   VK_F16              | F16 key
    sad::KeyNone,              // 128   0x80   VK_F17              | F17 key
    sad::KeyNone,              // 129   0x81   VK_F18              | F18 key
    sad::KeyNone,              // 130   0x82   VK_F19              | F19 key
    sad::KeyNone,              // 131   0x83   VK_F20              | F20 key
    sad::KeyNone,              // 132   0x84   VK_F21              | F21 key
    sad::KeyNone,              // 133   0x85   VK_F22              | F22 key
    sad::KeyNone,              // 134   0x86   VK_F23              | F23 key
    sad::KeyNone,              // 135   0x87   VK_F24              | F24 key
    sad::KeyNone,              // 136   0x88   -- unassigned --
    sad::KeyNone,              // 137   0x89   -- unassigned --
    sad::KeyNone,              // 138   0x8A                       | -- unassigned --
    sad::KeyNone,              // 139   0x8B                       | -- unassigned --
    sad::KeyNone,              // 140   0x8C                       | -- unassigned --
    sad::KeyNone,              // 141   0x8D                       | -- unassigned --
    sad::KeyNone,              // 142   0x8E                       | -- unassigned --
    sad::KeyNone,              // 143   0x8F                       | -- unassigned --
    sad::NumLock,              // 144   0x90  VK_NUMLOCK           | Num Lock key
    sad::ScrollLock,           // 145   0x91  VK_SCROLL            | Scroll Lock key
                               // Fujitsu/OASYS kbd --------------------
    sad::KeyNone,              // 146   0x92  VK_OEM_FJ_JISHO      | 'Dictionary' key /
                               //             VK_OEM_NEC_EQUAL     | key on numpad on NEC PC-9800 kbd
    sad::KeyNone,              // 147   0x93  VK_OEM_FJ_MASSHOU    | 'Unregister word' key
    sad::KeyNone,              // 148   0x94  VK_OEM_FJ_TOUROKU    | 'Register word' key
    sad::KeyNone,              // 149   0x95  VK_OEM_FJ_LOYA       | 'Left OYAYUBI' key
    sad::KeyNone,              // 150   0x96  VK_OEM_FJ_ROYA       | 'Right OYAYUBI' key
    sad::KeyNone,              // 151   0x97                       | -- unassigned --
    sad::KeyNone,              // 152   0x98                       | -- unassigned --
    sad::KeyNone,              // 153   0x99                       | -- unassigned --
    sad::KeyNone,              // 154   0x9A                       | -- unassigned --
    sad::KeyNone,              // 155   0x9B                       | -- unassigned --
    sad::KeyNone,              // 156   0x9C                       | -- unassigned --
    sad::KeyNone,              // 157   0x9D                       | -- unassigned --
    sad::KeyNone,              // 158   0x9E                       | -- unassigned --
    sad::KeyNone,              // 159   0x9F                       | -- unassigned --
    sad::LeftShift,            // 160   0xA0  VK_LSHIFT            | Left Shift key
    sad::RightShift,           // 161   0xA1  VK_RSHIFT            | Right Shift key
    sad::LeftControl,          // 162   0xA2  VK_LCONTROL          | Left Ctrl key
    sad::RightControl,         // 163   0xA3  VK_RCONTROL          | Right Ctrl key
    sad::LeftAlt,              // 164   0xA4  VK_LMENU             | Left Menu key
    sad::RightAlt,             // 165   0xA5  VK_RMENU             | Right Menu key
    sad::KeyNone,              // 166
    sad::KeyNone,              // 167
    sad::KeyNone,              // 168
    sad::KeyNone,              // 169
    sad::KeyNone,              // 170
    sad::KeyNone,              // 171
    sad::KeyNone,              // 172
    sad::KeyNone,              // 173
    sad::KeyNone,              // 174
    sad::KeyNone,              // 175
    sad::KeyNone,              // 176
    sad::KeyNone,              // 177
    sad::KeyNone,              // 178
    sad::KeyNone,              // 179
    sad::KeyNone,              // 180
    sad::KeyNone,              // 181
    sad::KeyNone,              // 182
    sad::KeyNone,              // 183
    sad::KeyNone,              // 184
    sad::KeyNone,              // 185
    sad::Semicolon,            // 186   0xBA  VK_OEM_1
    sad::Equal,                // 187   0xBB  VK_OEM_PLUS
    sad::Comma,                // 188   0xBC  VK_OEM_COMMA
    sad::Minus,                // 189   0xBD  VK_OEM_MINUS
    sad::Period,               // 190   0xBE  VK_OEM_PERIOD
    sad::Slash,                // 191   0xBF  VK_OEM_2
    sad::Tilde,                // 192   0xC0  VK_OEM_3
    sad::KeyNone,              // 193
    sad::KeyNone,              // 194
    sad::KeyNone,              // 195
    sad::KeyNone,              // 196
    sad::KeyNone,              // 197
    sad::KeyNone,              // 198
    sad::KeyNone,              // 199
    sad::KeyNone,              // 200
    sad::KeyNone,              // 201
    sad::KeyNone,              // 202
    sad::KeyNone,              // 203
    sad::KeyNone,              // 204
    sad::KeyNone,              // 205
    sad::KeyNone,              // 206
    sad::KeyNone,              // 207
    sad::KeyNone,              // 208
    sad::KeyNone,              // 209
    sad::KeyNone,              // 210
    sad::KeyNone,              // 211
    sad::KeyNone,              // 212
    sad::KeyNone,              // 213
    sad::KeyNone,              // 214
    sad::KeyNone,              // 215
    sad::KeyNone,              // 216
    sad::KeyNone,              // 217
    sad::KeyNone,              // 218
    sad::OpeningSquareBracket, // 219   0xDB VK_OEM_4
    sad::BackSlash,            // 220   0xDC VK_OEM_5
    sad::ClosingSquareBracket, // 221   0xDD VK_OEM_6
    sad::Apostrophe            // 222   0xDE VK_OEM_6
};



#endif


#ifdef X11


#ifdef  XK_CYRILLIC
const int totalmappingsize = 522;
#else
const int totalmappingsize = 392;
#endif

unsigned int mapping[totalmappingsize] = {
    XK_Escape, sad::Esc,
    XK_F1,     sad::F1,
    XK_F2,     sad::F2,
    XK_F3,     sad::F3,
    XK_F4,     sad::F4,
    XK_F5,     sad::F5,
    XK_F6,     sad::F6,
    XK_F7,     sad::F7,
    XK_F8,     sad::F8,
    XK_F9,     sad::F9,
    XK_F10,    sad::F10,
    XK_F11,    sad::F11,
    XK_F12,    sad::F12,
    // Most WM hook this key so binding on it still have no purpose.
    XK_Sys_Req,        sad::PrintScreen,
    XK_Scroll_Lock,    sad::ScrollLock,
    XK_Pause,          sad::Pause,
    XK_asciitilde,     sad::Tilde,
    XK_grave,          sad::Tilde,
#ifdef XK_CYRILLIC
    XK_Cyrillic_io,    sad::Tilde,
#endif
    XK_1,              sad::Numeric1,
    XK_exclam,         sad::Numeric1,
    XK_2,              sad::Numeric2,
    XK_at,             sad::Numeric2,
    XK_3,              sad::Numeric3,
    XK_4,              sad::Numeric4,
    XK_5,              sad::Numeric5,
    XK_6,              sad::Numeric6,
    XK_7,              sad::Numeric7,
    XK_8,              sad::Numeric8,
    XK_9,              sad::Numeric9,
    XK_0,              sad::Numeric0,
    XK_minus,          sad::Minus,
    XK_underscore,     sad::Minus,
    XK_equal,          sad::Equal,
    XK_plus,           sad::Equal,
    XK_backslash,      sad::BackSlash,
    XK_bar,            sad::BackSlash,
    XK_BackSpace,      sad::Backspace,
    XK_Insert,         sad::Insert,
    XK_Home,           sad::Home,
    XK_Prior,          sad::PageUp,
    XK_Num_Lock,       sad::NumLock,
    XK_Tab,            sad::Tab,
    XK_a, sad::A,
    XK_A, sad::A,
#ifdef XK_CYRILLIC
    XK_Cyrillic_ef, sad::A,
    XK_Cyrillic_EF, sad::A,
#endif
    XK_b, sad::B,
    XK_B, sad::B,
#ifdef XK_CYRILLIC
    XK_Cyrillic_i, sad::B,
    XK_Cyrillic_I, sad::B,
#endif
    XK_c, sad::C,
    XK_C, sad::C,
#ifdef XK_CYRILLIC
    XK_Cyrillic_es, sad::C,
    XK_Cyrillic_ES, sad::C,
#endif
    XK_d, sad::D,
    XK_D, sad::D,
#ifdef XK_CYRILLIC
    XK_Cyrillic_ve, sad::D,
    XK_Cyrillic_VE, sad::D,
#endif
    XK_e, sad::E,
    XK_E, sad::E,
#ifdef XK_CYRILLIC
    XK_Cyrillic_u, sad::E,
    XK_Cyrillic_U, sad::E,
#endif
    XK_f, sad::F,
    XK_F, sad::F,
#ifdef XK_CYRILLIC
    XK_Cyrillic_a, sad::F,
    XK_Cyrillic_A, sad::F,
#endif
    XK_g, sad::G,
    XK_G, sad::G,
#ifdef XK_CYRILLIC
    XK_Cyrillic_pe, sad::G,
    XK_Cyrillic_PE, sad::G,
#endif
    XK_h, sad::H,
    XK_H, sad::H,
#ifdef XK_CYRILLIC
    XK_Cyrillic_er, sad::H,
    XK_Cyrillic_ER, sad::H,
#endif
    XK_i, sad::I,
    XK_I, sad::I,
#ifdef XK_CYRILLIC
    XK_Cyrillic_sha, sad::I,
    XK_Cyrillic_SHA, sad::I,
#endif
    XK_j, sad::J,
    XK_J, sad::J,
#ifdef XK_CYRILLIC
    XK_Cyrillic_o, sad::J,
    XK_Cyrillic_O, sad::J,
#endif
    XK_k, sad::K,
    XK_K, sad::K,
#ifdef XK_CYRILLIC
    XK_Cyrillic_el, sad::K,
    XK_Cyrillic_EL, sad::K,
#endif
    XK_l, sad::L,
    XK_L, sad::L,
#ifdef XK_CYRILLIC
    XK_Cyrillic_de, sad::L,
    XK_Cyrillic_DE, sad::L,
#endif
    XK_m, sad::M,
    XK_M, sad::M,
#ifdef XK_CYRILLIC
    XK_Cyrillic_softsign, sad::M,
    XK_Cyrillic_SOFTSIGN, sad::M,
#endif
    XK_n, sad::N,
    XK_N, sad::N,
#ifdef XK_CYRILLIC
    XK_Cyrillic_te, sad::N,
    XK_Cyrillic_TE, sad::N,
#endif
    XK_o, sad::O,
    XK_O, sad::O,
#ifdef XK_CYRILLIC
    XK_Cyrillic_shcha, sad::O,
    XK_Cyrillic_SHCHA, sad::O,
#endif
    XK_p, sad::P,
    XK_P, sad::P,
#ifdef XK_CYRILLIC
    XK_Cyrillic_ze, sad::P,
    XK_Cyrillic_ZE, sad::P,
#endif
    XK_q, sad::Q,
    XK_Q, sad::Q,
#ifdef XK_CYRILLIC
    XK_Cyrillic_shorti, sad::Q,
    XK_Cyrillic_SHORTI, sad::Q,
#endif
    XK_r, sad::R,
    XK_R, sad::R,
#ifdef XK_CYRILLIC
    XK_Cyrillic_ka, sad::R,
    XK_Cyrillic_KA, sad::R,
#endif
    XK_s, sad::S,
    XK_S, sad::S,
#ifdef XK_CYRILLIC
    XK_Cyrillic_yeru, sad::S,
    XK_Cyrillic_YERU, sad::S,
#endif
    XK_t, sad::T,
    XK_T, sad::T,
#ifdef XK_CYRILLIC
    XK_Cyrillic_ie, sad::T,
    XK_Cyrillic_IE, sad::T,
#endif

    XK_u, sad::U,
    XK_U, sad::U,
#ifdef XK_CYRILLIC
    XK_Cyrillic_ghe, sad::U,
    XK_Cyrillic_GHE, sad::U,
#endif
    XK_v, sad::V,
    XK_V, sad::V,
#ifdef XK_CYRILLIC
    XK_Cyrillic_em, sad::V,
    XK_Cyrillic_EM, sad::V,
#endif
    XK_w, sad::W,
    XK_W, sad::W,
#ifdef XK_CYRILLIC
    XK_Cyrillic_tse, sad::W,
    XK_Cyrillic_TSE, sad::W,
#endif
    XK_x, sad::X,
    XK_X, sad::X,
#ifdef XK_CYRILLIC
    XK_Cyrillic_che, sad::X,
    XK_Cyrillic_CHE, sad::X,
#endif
    XK_y, sad::Y,
    XK_Y, sad::Y,
#ifdef XK_CYRILLIC
    XK_Cyrillic_en, sad::Y,
    XK_Cyrillic_EN, sad::Y,
#endif
    XK_z, sad::Z,
    XK_Z, sad::Z,
#ifdef XK_CYRILLIC
    XK_Cyrillic_ya, sad::Z,
    XK_Cyrillic_YA, sad::Z,
#endif
    XK_bracketleft,    sad::OpeningSquareBracket,
    XK_braceleft,      sad::OpeningSquareBracket,
#ifdef XK_CYRILLIC
    XK_Cyrillic_ha,    sad::OpeningSquareBracket,
    XK_Cyrillic_HA,    sad::OpeningSquareBracket,
#endif
    XK_bracketright,    sad::ClosingSquareBracket,
    XK_braceright,      sad::ClosingSquareBracket,
#ifdef XK_CYRILLIC
    XK_Cyrillic_hardsign,    sad::ClosingSquareBracket,
    XK_Cyrillic_HARDSIGN,    sad::ClosingSquareBracket,
#endif
    XK_semicolon,   sad::Semicolon,
    XK_colon,       sad::Semicolon,
#ifdef XK_CYRILLIC
    XK_Cyrillic_zhe,    sad::Semicolon,
    XK_Cyrillic_ZHE,    sad::Semicolon,
#endif
    XK_apostrophe,   sad::Apostrophe,
    XK_quotedbl,       sad::Apostrophe,
#ifdef XK_CYRILLIC
    XK_Cyrillic_e,    sad::Apostrophe,
    XK_Cyrillic_E,    sad::Apostrophe,
#endif
    XK_comma,    sad::Comma,
    XK_less,     sad::Comma,
#ifdef XK_CYRILLIC
    XK_Cyrillic_be,    sad::Comma,
    XK_Cyrillic_BE,    sad::Comma,
#endif
    XK_period,   sad::Period,
    XK_greater,  sad::Period,
#ifdef XK_CYRILLIC
    XK_Cyrillic_yu,    sad::Period,
    XK_Cyrillic_YU,    sad::Period,
#endif
    XK_slash,          sad::Slash,
    XK_question,       sad::Slash,
    XK_Return,         sad::Enter,
    XK_Delete,         sad::Delete,
    XK_End,            sad::End,
    XK_Next,           sad::PageDown,

    XK_Caps_Lock,      sad::CapsLock,
    XK_Shift_L,        sad::LeftShift,
    XK_Shift_R,        sad::RightShift,
    XK_Control_L,      sad::LeftControl,
    XK_Control_R,      sad::RightControl,
    XK_Super_L,        sad::WindowsKey,
    XK_Super_R,        sad::WindowsKey,
    XK_Alt_L,          sad::LeftAlt,
    XK_Alt_R,          sad::RightAlt,
    XK_space,          sad::Space,
    XK_Menu,           sad::AppKey,
    XK_Left,           sad::KeyLeft,
    XK_Up,             sad::KeyUp,
    XK_Down,           sad::KeyDown,
    XK_Left,           sad::KeyLeft,
    XK_Right,          sad::KeyRight
};

#endif


void sad::os::KeyDecoder::init()
{
#ifdef WIN32
    for(unsigned int i = 0; i < syskeycount; i++)
    {
        m_table.insert((sad::os::SystemKey)i, syskeys[i]);
    }
    ZeroMemory(m_key_states, 256);
#endif

#ifdef X11
    unsigned int i = 0;
    for(i = 0; i < totalmappingsize; i += 2)
    {
        sad::os::SystemKey syskey = (sad::os::SystemKey)mapping[i];
        sad::KeyboardKey sadkey = (sad::KeyboardKey)mapping[i+1];
        m_table.insert(syskey, sadkey);
    }
#endif
}


sad::KeyboardKey sad::os::KeyDecoder::decode(sad::os::SystemWindowEvent * e)
{
#ifdef WIN32
    sad::KeyboardKey key = sad::KeyNone;
    if (m_table.contains(e->WParam))
    {
        key = m_table[e->WParam];
    }
    return key;
#endif

#ifdef X11
    sad::KeyboardKey key = sad::KeyNone;
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
    sad::Minus,
    sad::Equal,

    sad::BackSlash,
    sad::Tilde,
    sad::OpeningSquareBracket,

    sad::ClosingSquareBracket,
    sad::Semicolon,
    sad::Comma,

    sad::Period,
    sad::Slash,
    sad::Apostrophe,
    sad::Space
};

bool sad::os::KeyDecoder::isReadable(sad::KeyboardKey key)
{
    bool is_readable = (key >= '0' && key <= '9');
    is_readable = is_readable || (key >= 'A' && key <= 'Z');
    for(int i = 0 ; i < readablekeyscount; i++)
    {
        is_readable = is_readable || (key == readablekeys[i]);
    }
    return is_readable;
}
