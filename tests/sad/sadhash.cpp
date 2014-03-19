#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include <sadhash.h>
#include <keycodes.h>
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

const int totalmappingsize = 522;

static unsigned int mapping[totalmappingsize] = {
0xff1b, sad::Esc,
0xffbe, sad::F1,
0xffbf, sad::F2,
0xffc0, sad::F3,
0xffc1, sad::F4,
0xffc2, sad::F5,
0xffc3, sad::F6,
0xffc4, sad::F7,
0xffc5, sad::F8,
0xffc6, sad::F9,
0xffc7, sad::F10,
0xffc8, sad::F11,
0xffc9, sad::F12,

0xff15, sad::PrintScreen,
0xff14, sad::ScrollLock,
0xff13, sad::Pause,
0x007e, sad::Tilde,
0x0060, sad::Tilde,

0x06a3, sad::Tilde,

0x0031, sad::Numeric1,
0x0021, sad::Numeric1,
0x0032, sad::Numeric2,
0x0040, sad::Numeric2,
0x0033, sad::Numeric3,
0x0034, sad::Numeric4,
0x0035, sad::Numeric5,
0x0036, sad::Numeric6,
0x0037, sad::Numeric7,
0x0038, sad::Numeric8,
0x0039, sad::Numeric9,
0x0030, sad::Numeric0,
0x002d, sad::Minus,
0x005f, sad::Minus,
0x003d, sad::Equal,
0x002b, sad::Equal,
0x005c, sad::BackSlash,
0x007c, sad::BackSlash,
0xff08, sad::Backspace,
0xff63, sad::Insert,
0xff50, sad::Home,
0xff55, sad::PageUp,
0xff7f, sad::NumLock,
0xff09, sad::Tab,
0x0061, sad::A,
0x0041, sad::A,

0x06c6, sad::A,
0x06e6, sad::A,

0x0062, sad::B,
0x0042, sad::B,

0x06c9, sad::B,
0x06e9, sad::B,

0x0063, sad::C,
0x0043, sad::C,

0x06d3, sad::C,
0x06f3, sad::C,

0x0064, sad::D,
0x0044, sad::D,

0x06d7, sad::D,
0x06f7, sad::D,

0x0065, sad::E,
0x0045, sad::E,

0x06d5, sad::E,
0x06f5, sad::E,

0x0066, sad::F,
0x0046, sad::F,

0x06c1, sad::F,
0x06e1, sad::F,

0x0067, sad::G,
0x0047, sad::G,

0x06d0, sad::G,
0x06f0, sad::G,

0x0068, sad::H,
0x0048, sad::H,

0x06d2, sad::H,
0x06f2, sad::H,

0x0069, sad::I,
0x0049, sad::I,

0x06db, sad::I,
0x06fb, sad::I,

0x006a, sad::J,
0x004a, sad::J,

0x06cf, sad::J,
0x06ef, sad::J,

0x006b, sad::K,
0x004b, sad::K,

0x06cc, sad::K,
0x06ec, sad::K,

0x006c, sad::L,
0x004c, sad::L,

0x06c4, sad::L,
0x06e4, sad::L,

0x006d, sad::M,
0x004d, sad::M,

0x06d8, sad::M,
0x06f8, sad::M,

0x006e, sad::N,
0x004e, sad::N,

0x06d4, sad::N,
0x06f4, sad::N,

0x006f, sad::O,
0x004f, sad::O,

0x06dd, sad::O,
0x06fd, sad::O,

0x0070, sad::P,
0x0050, sad::P,

0x06da, sad::P,
0x06fa, sad::P,

0x0071, sad::Q,
0x0051, sad::Q,

0x06ca, sad::Q,
0x06ea, sad::Q,

0x0072, sad::R,
0x0052, sad::R,

0x06cb, sad::R,
0x06eb, sad::R,

0x0073, sad::S,
0x0053, sad::S,

0x06d9, sad::S,
0x06f9, sad::S,

0x0074, sad::T,
0x0054, sad::T,

0x06c5, sad::T,
0x06e5, sad::T,

0x0075, sad::U,
0x0055, sad::U,

0x06c7, sad::U,
0x06e7, sad::U,

0x0076, sad::V,
0x0056, sad::V,

0x06cd, sad::V,
0x06ed, sad::V,

0x0077, sad::W,
0x0057, sad::W,

0x06c3, sad::W,
0x06e3, sad::W,

0x0078, sad::X,
0x0058, sad::X,

0x06de, sad::X,
0x06fe, sad::X,

0x0079, sad::Y,
0x0059, sad::Y,

0x06ce, sad::Y,
0x06ee, sad::Y,

0x007a, sad::Z,
0x005a, sad::Z,

0x06d1, sad::Z,
0x06f1, sad::Z,

0x005b, sad::OpeningSquareBracket,
0x007b, sad::OpeningSquareBracket,

0x06c8, sad::OpeningSquareBracket,
0x06e8, sad::OpeningSquareBracket,

0x005d, sad::ClosingSquareBracket,
0x007d, sad::ClosingSquareBracket,

0x06df, sad::ClosingSquareBracket,
0x06ff, sad::ClosingSquareBracket,

0x003b, sad::Semicolon,
0x003a, sad::Semicolon,

0x06d6, sad::Semicolon,
0x06f6, sad::Semicolon,

0x0027, sad::Apostrophe,
0x0022, sad::Apostrophe,

0x06dc, sad::Apostrophe,
0x06fc, sad::Apostrophe,

0x002c, sad::Comma,
0x003c, sad::Comma,

0x06c2, sad::Comma,
0x06e2, sad::Comma,

0x002e, sad::Period,
0x003e, sad::Period,

0x06c0, sad::Period,
0x06e0, sad::Period,

0x002f, sad::Slash,
0x003f, sad::Slash,
0xff0d, sad::Enter,
0xffff, sad::Delete,
0xff57, sad::End,
0xff56, sad::PageDown,

0xffe5, sad::CapsLock,
0xffe1, sad::LeftShift,
0xffe2, sad::RightShift,
0xffe3, sad::LeftControl,
0xffe4, sad::RightControl,
0xffeb, sad::WindowsKey,
0xffec, sad::WindowsKey,
0xffe9, sad::LeftAlt,
0xffea, sad::RightAlt,
0x0020, sad::Space,
0xff67, sad::AppKey,
0xff51, sad::KeyLeft,
0xff52, sad::KeyUp,
0xff54, sad::KeyDown,
0xff51, sad::KeyLeft,
0xff53, sad::KeyRight
};

struct SadHashTest : tpunit::TestFixture
{
 public:
   SadHashTest() : tpunit::TestFixture(
	   TEST(SadHashTest::testBug261113)
   ) {}
   
   void testBug261113()
   {	
		sad::Hash<unsigned long, sad::KeyboardKey> m_keys;

		unsigned int i = 0;
		for(i = 0; i < totalmappingsize; i += 2)
		{
			unsigned long syskey = mapping[i];
			sad::KeyboardKey sadkey = (sad::KeyboardKey)mapping[i+1];
			m_keys.insert(syskey, sadkey);
		}

		ASSERT_TRUE(m_keys.contains(65509));
   }


} test_hash_test;

