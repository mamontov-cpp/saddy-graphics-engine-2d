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
0xff1b, static_cast<unsigned int>(sad::KeyboardKey::Esc),
0xffbe, static_cast<unsigned int>(sad::KeyboardKey::F1),
0xffbf, static_cast<unsigned int>(sad::KeyboardKey::F2),
0xffc0, static_cast<unsigned int>(sad::KeyboardKey::F3),
0xffc1, static_cast<unsigned int>(sad::KeyboardKey::F4),
0xffc2, static_cast<unsigned int>(sad::KeyboardKey::F5),
0xffc3, static_cast<unsigned int>(sad::KeyboardKey::F6),
0xffc4, static_cast<unsigned int>(sad::KeyboardKey::F7),
0xffc5, static_cast<unsigned int>(sad::KeyboardKey::F8),
0xffc6, static_cast<unsigned int>(sad::KeyboardKey::F9),
0xffc7, static_cast<unsigned int>(sad::KeyboardKey::F10),
0xffc8, static_cast<unsigned int>(sad::KeyboardKey::F11),
0xffc9, static_cast<unsigned int>(sad::KeyboardKey::F12),

0xff15, static_cast<unsigned int>(sad::KeyboardKey::PrintScreen),
0xff14, static_cast<unsigned int>(sad::KeyboardKey::ScrollLock),
0xff13, static_cast<unsigned int>(sad::KeyboardKey::Pause),
0x007e, static_cast<unsigned int>(sad::KeyboardKey::Tilde),
0x0060, static_cast<unsigned int>(sad::KeyboardKey::Tilde),

0x06a3, static_cast<unsigned int>(sad::KeyboardKey::Tilde),

0x0031, static_cast<unsigned int>(sad::KeyboardKey::Numeric1),
0x0021, static_cast<unsigned int>(sad::KeyboardKey::Numeric1),
0x0032, static_cast<unsigned int>(sad::KeyboardKey::Numeric2),
0x0040, static_cast<unsigned int>(sad::KeyboardKey::Numeric2),
0x0033, static_cast<unsigned int>(sad::KeyboardKey::Numeric3),
0x0034, static_cast<unsigned int>(sad::KeyboardKey::Numeric4),
0x0035, static_cast<unsigned int>(sad::KeyboardKey::Numeric5),
0x0036, static_cast<unsigned int>(sad::KeyboardKey::Numeric6),
0x0037, static_cast<unsigned int>(sad::KeyboardKey::Numeric7),
0x0038, static_cast<unsigned int>(sad::KeyboardKey::Numeric8),
0x0039, static_cast<unsigned int>(sad::KeyboardKey::Numeric9),
0x0030, static_cast<unsigned int>(sad::KeyboardKey::Numeric0),
0x002d, static_cast<unsigned int>(sad::KeyboardKey::Minus),
0x005f, static_cast<unsigned int>(sad::KeyboardKey::Minus),
0x003d, static_cast<unsigned int>(sad::KeyboardKey::Equal),
0x002b, static_cast<unsigned int>(sad::KeyboardKey::Equal),
0x005c, static_cast<unsigned int>(sad::KeyboardKey::BackSlash),
0x007c, static_cast<unsigned int>(sad::KeyboardKey::BackSlash),
0xff08, static_cast<unsigned int>(sad::KeyboardKey::Backspace),
0xff63, static_cast<unsigned int>(sad::KeyboardKey::Insert),
0xff50, static_cast<unsigned int>(sad::KeyboardKey::Home),
0xff55, static_cast<unsigned int>(sad::KeyboardKey::PageUp),
0xff7f, static_cast<unsigned int>(sad::KeyboardKey::NumLock),
0xff09, static_cast<unsigned int>(sad::KeyboardKey::Tab),
0x0061, static_cast<unsigned int>(sad::KeyboardKey::A),
0x0041, static_cast<unsigned int>(sad::KeyboardKey::A),

0x06c6, static_cast<unsigned int>(sad::KeyboardKey::A),
0x06e6, static_cast<unsigned int>(sad::KeyboardKey::A),

0x0062, static_cast<unsigned int>(sad::KeyboardKey::B),
0x0042, static_cast<unsigned int>(sad::KeyboardKey::B),

0x06c9, static_cast<unsigned int>(sad::KeyboardKey::B),
0x06e9, static_cast<unsigned int>(sad::KeyboardKey::B),

0x0063, static_cast<unsigned int>(sad::KeyboardKey::C),
0x0043, static_cast<unsigned int>(sad::KeyboardKey::C),

0x06d3, static_cast<unsigned int>(sad::KeyboardKey::C),
0x06f3, static_cast<unsigned int>(sad::KeyboardKey::C),

0x0064, static_cast<unsigned int>(sad::KeyboardKey::D),
0x0044, static_cast<unsigned int>(sad::KeyboardKey::D),

0x06d7, static_cast<unsigned int>(sad::KeyboardKey::D),
0x06f7, static_cast<unsigned int>(sad::KeyboardKey::D),

0x0065, static_cast<unsigned int>(sad::KeyboardKey::E),
0x0045, static_cast<unsigned int>(sad::KeyboardKey::E),

0x06d5, static_cast<unsigned int>(sad::KeyboardKey::E),
0x06f5, static_cast<unsigned int>(sad::KeyboardKey::E),

0x0066, static_cast<unsigned int>(sad::KeyboardKey::F),
0x0046, static_cast<unsigned int>(sad::KeyboardKey::F),

0x06c1, static_cast<unsigned int>(sad::KeyboardKey::F),
0x06e1, static_cast<unsigned int>(sad::KeyboardKey::F),

0x0067, static_cast<unsigned int>(sad::KeyboardKey::G),
0x0047, static_cast<unsigned int>(sad::KeyboardKey::G),

0x06d0, static_cast<unsigned int>(sad::KeyboardKey::G),
0x06f0, static_cast<unsigned int>(sad::KeyboardKey::G),

0x0068, static_cast<unsigned int>(sad::KeyboardKey::H),
0x0048, static_cast<unsigned int>(sad::KeyboardKey::H),

0x06d2, static_cast<unsigned int>(sad::KeyboardKey::H),
0x06f2, static_cast<unsigned int>(sad::KeyboardKey::H),

0x0069, static_cast<unsigned int>(sad::KeyboardKey::I),
0x0049, static_cast<unsigned int>(sad::KeyboardKey::I),

0x06db, static_cast<unsigned int>(sad::KeyboardKey::I),
0x06fb, static_cast<unsigned int>(sad::KeyboardKey::I),

0x006a, static_cast<unsigned int>(sad::KeyboardKey::J),
0x004a, static_cast<unsigned int>(sad::KeyboardKey::J),

0x06cf, static_cast<unsigned int>(sad::KeyboardKey::J),
0x06ef, static_cast<unsigned int>(sad::KeyboardKey::J),

0x006b, static_cast<unsigned int>(sad::KeyboardKey::K),
0x004b, static_cast<unsigned int>(sad::KeyboardKey::K),

0x06cc, static_cast<unsigned int>(sad::KeyboardKey::K),
0x06ec, static_cast<unsigned int>(sad::KeyboardKey::K),

0x006c, static_cast<unsigned int>(sad::KeyboardKey::L),
0x004c, static_cast<unsigned int>(sad::KeyboardKey::L),

0x06c4, static_cast<unsigned int>(sad::KeyboardKey::L),
0x06e4, static_cast<unsigned int>(sad::KeyboardKey::L),

0x006d, static_cast<unsigned int>(sad::KeyboardKey::M),
0x004d, static_cast<unsigned int>(sad::KeyboardKey::M),

0x06d8, static_cast<unsigned int>(sad::KeyboardKey::M),
0x06f8, static_cast<unsigned int>(sad::KeyboardKey::M),

0x006e, static_cast<unsigned int>(sad::KeyboardKey::N),
0x004e, static_cast<unsigned int>(sad::KeyboardKey::N),

0x06d4, static_cast<unsigned int>(sad::KeyboardKey::N),
0x06f4, static_cast<unsigned int>(sad::KeyboardKey::N),

0x006f, static_cast<unsigned int>(sad::KeyboardKey::O),
0x004f, static_cast<unsigned int>(sad::KeyboardKey::O),

0x06dd, static_cast<unsigned int>(sad::KeyboardKey::O),
0x06fd, static_cast<unsigned int>(sad::KeyboardKey::O),

0x0070, static_cast<unsigned int>(sad::KeyboardKey::P),
0x0050, static_cast<unsigned int>(sad::KeyboardKey::P),

0x06da, static_cast<unsigned int>(sad::KeyboardKey::P),
0x06fa, static_cast<unsigned int>(sad::KeyboardKey::P),

0x0071, static_cast<unsigned int>(sad::KeyboardKey::Q),
0x0051, static_cast<unsigned int>(sad::KeyboardKey::Q),

0x06ca, static_cast<unsigned int>(sad::KeyboardKey::Q),
0x06ea, static_cast<unsigned int>(sad::KeyboardKey::Q),

0x0072, static_cast<unsigned int>(sad::KeyboardKey::R),
0x0052, static_cast<unsigned int>(sad::KeyboardKey::R),

0x06cb, static_cast<unsigned int>(sad::KeyboardKey::R),
0x06eb, static_cast<unsigned int>(sad::KeyboardKey::R),

0x0073, static_cast<unsigned int>(sad::KeyboardKey::S),
0x0053, static_cast<unsigned int>(sad::KeyboardKey::S),

0x06d9, static_cast<unsigned int>(sad::KeyboardKey::S),
0x06f9, static_cast<unsigned int>(sad::KeyboardKey::S),

0x0074, static_cast<unsigned int>(sad::KeyboardKey::T),
0x0054, static_cast<unsigned int>(sad::KeyboardKey::T),

0x06c5, static_cast<unsigned int>(sad::KeyboardKey::T),
0x06e5, static_cast<unsigned int>(sad::KeyboardKey::T),

0x0075, static_cast<unsigned int>(sad::KeyboardKey::U),
0x0055, static_cast<unsigned int>(sad::KeyboardKey::U),

0x06c7, static_cast<unsigned int>(sad::KeyboardKey::U),
0x06e7, static_cast<unsigned int>(sad::KeyboardKey::U),

0x0076, static_cast<unsigned int>(sad::KeyboardKey::V),
0x0056, static_cast<unsigned int>(sad::KeyboardKey::V),

0x06cd, static_cast<unsigned int>(sad::KeyboardKey::V),
0x06ed, static_cast<unsigned int>(sad::KeyboardKey::V),

0x0077, static_cast<unsigned int>(sad::KeyboardKey::W),
0x0057, static_cast<unsigned int>(sad::KeyboardKey::W),

0x06c3, static_cast<unsigned int>(sad::KeyboardKey::W),
0x06e3, static_cast<unsigned int>(sad::KeyboardKey::W),

0x0078, static_cast<unsigned int>(sad::KeyboardKey::X),
0x0058, static_cast<unsigned int>(sad::KeyboardKey::X),

0x06de, static_cast<unsigned int>(sad::KeyboardKey::X),
0x06fe, static_cast<unsigned int>(sad::KeyboardKey::X),

0x0079, static_cast<unsigned int>(sad::KeyboardKey::Y),
0x0059, static_cast<unsigned int>(sad::KeyboardKey::Y),

0x06ce, static_cast<unsigned int>(sad::KeyboardKey::Y),
0x06ee, static_cast<unsigned int>(sad::KeyboardKey::Y),

0x007a, static_cast<unsigned int>(sad::KeyboardKey::Z),
0x005a, static_cast<unsigned int>(sad::KeyboardKey::Z),

0x06d1, static_cast<unsigned int>(sad::KeyboardKey::Z),
0x06f1, static_cast<unsigned int>(sad::KeyboardKey::Z),

0x005b, static_cast<unsigned int>(sad::KeyboardKey::OpeningSquareBracket),
0x007b, static_cast<unsigned int>(sad::KeyboardKey::OpeningSquareBracket),

0x06c8, static_cast<unsigned int>(sad::KeyboardKey::OpeningSquareBracket),
0x06e8, static_cast<unsigned int>(sad::KeyboardKey::OpeningSquareBracket),

0x005d, static_cast<unsigned int>(sad::KeyboardKey::ClosingSquareBracket),
0x007d, static_cast<unsigned int>(sad::KeyboardKey::ClosingSquareBracket),

0x06df, static_cast<unsigned int>(sad::KeyboardKey::ClosingSquareBracket),
0x06ff, static_cast<unsigned int>(sad::KeyboardKey::ClosingSquareBracket),

0x003b, static_cast<unsigned int>(sad::KeyboardKey::Semicolon),
0x003a, static_cast<unsigned int>(sad::KeyboardKey::Semicolon),

0x06d6, static_cast<unsigned int>(sad::KeyboardKey::Semicolon),
0x06f6, static_cast<unsigned int>(sad::KeyboardKey::Semicolon),

0x0027, static_cast<unsigned int>(sad::KeyboardKey::Apostrophe),
0x0022, static_cast<unsigned int>(sad::KeyboardKey::Apostrophe),

0x06dc, static_cast<unsigned int>(sad::KeyboardKey::Apostrophe),
0x06fc, static_cast<unsigned int>(sad::KeyboardKey::Apostrophe),

0x002c, static_cast<unsigned int>(sad::KeyboardKey::Comma),
0x003c, static_cast<unsigned int>(sad::KeyboardKey::Comma),

0x06c2, static_cast<unsigned int>(sad::KeyboardKey::Comma),
0x06e2, static_cast<unsigned int>(sad::KeyboardKey::Comma),

0x002e, static_cast<unsigned int>(sad::KeyboardKey::Period),
0x003e, static_cast<unsigned int>(sad::KeyboardKey::Period),

0x06c0, static_cast<unsigned int>(sad::KeyboardKey::Period),
0x06e0, static_cast<unsigned int>(sad::KeyboardKey::Period),

0x002f, static_cast<unsigned int>(sad::KeyboardKey::Slash),
0x003f, static_cast<unsigned int>(sad::KeyboardKey::Slash),
0xff0d, static_cast<unsigned int>(sad::KeyboardKey::Enter),
0xffff, static_cast<unsigned int>(sad::KeyboardKey::Delete),
0xff57, static_cast<unsigned int>(sad::KeyboardKey::End),
0xff56, static_cast<unsigned int>(sad::KeyboardKey::PageDown),

0xffe5, static_cast<unsigned int>(sad::KeyboardKey::CapsLock),
0xffe1, static_cast<unsigned int>(sad::KeyboardKey::LeftShift),
0xffe2, static_cast<unsigned int>(sad::KeyboardKey::RightShift),
0xffe3, static_cast<unsigned int>(sad::KeyboardKey::LeftControl),
0xffe4, static_cast<unsigned int>(sad::KeyboardKey::RightControl),
0xffeb, static_cast<unsigned int>(sad::KeyboardKey::WindowsKey),
0xffec, static_cast<unsigned int>(sad::KeyboardKey::WindowsKey),
0xffe9, static_cast<unsigned int>(sad::KeyboardKey::LeftAlt),
0xffea, static_cast<unsigned int>(sad::KeyboardKey::RightAlt),
0x0020, static_cast<unsigned int>(sad::KeyboardKey::Space),
0xff67, static_cast<unsigned int>(sad::KeyboardKey::AppKey),
0xff51, static_cast<unsigned int>(sad::KeyboardKey::KeyLeft),
0xff52, static_cast<unsigned int>(sad::KeyboardKey::KeyUp),
0xff54, static_cast<unsigned int>(sad::KeyboardKey::KeyDown),
0xff51, static_cast<unsigned int>(sad::KeyboardKey::KeyLeft),
0xff53, static_cast<unsigned int>(sad::KeyboardKey::KeyRight)
};

struct SadHashTest : tpunit::TestFixture
{
 public:
   SadHashTest() : tpunit::TestFixture(
       TEST(SadHashTest::testBug261113),
       TEST(SadHashTest::testRepeatInsert),
       TEST(SadHashTest::testMultipleRemoval),
       TEST(SadHashTest::testNoElementLookup),
       TEST(SadHashTest::testMultipleHashes),
       TEST(SadHashTest::testLookup)		
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

   void testRepeatInsert()
   {
       sad::Hash<int,int> hash;
       hash.insert(1,1);
       hash.insert(1,2);
       int value = hash[1];
       ASSERT_TRUE(value == 2);
   }
   
   void testMultipleRemoval()
   {
       sad::Hash<int,int> hash;
       hash.insert(1,1);
       hash.remove(1);
       hash.remove(2);
       hash.remove(1);
       hash.remove(2);
       ASSERT_TRUE( !(hash.contains(1)) );
       ASSERT_TRUE( !(hash.contains(2)) );	   
   }
   
   // This test should not segfault
   void testNoElementLookup()
   {
       sad::Hash<int, int> hash;
       int a = hash[1];
   }
   
   // Tests multiple hashes inclusion
   void testMultipleHashes()
   {
       sad::Hash<int, sad::Hash<int, int> > hash;
       sad::Hash<int, int> hash1;
       hash1.insert(1, 1);
       hash1.insert(2, 2);
       hash1.insert(3, 3);
       hash1.insert(4, 4);
       hash.insert(1, hash1);
       sad::Hash<int, int> hash2;
       hash2.insert(5, 5);
       hash2.insert(6, 6);
       hash2.insert(7, 7);
       hash2.insert(8, 8);
       hash2.insert(9, 9);
       hash.insert(0, hash2);
       // Map from keys to count of their occurences
       int keys_ethalon[10]   = {1,2,1,1,1,1,1,1,1,1};
       int values_ethalon[10] = {0,1,1,1,1,1,1,1,1,1};
       int keys[10]   = {0,0,0,0,0,0,0,0,0,0};
       int values[10] = {0,0,0,0,0,0,0,0,0,0};
       for(sad::Hash<int, sad::Hash<int, int> >::iterator it1 = hash.begin();
           it1 != hash.end();
           ++it1)
        {
            keys[it1.key()] += 1;
            sad::Hash<int,int>& inner_hash = it1.value();
            for(sad::Hash<int, int>::iterator it2 = inner_hash.begin();
                it2 != inner_hash.end();
                ++it2)
            {
                keys[it2.key()] += 1;
                values[it2.value()] += 1;
            }
        }
        for(unsigned int i = 0; i < 10; i++)
        {
            ASSERT_TRUE( keys[i] == keys_ethalon[i] );
            ASSERT_TRUE( values[i] == values_ethalon[i] );
        }
   }
   
   void testLookup()
   {
       sad::Hash<int,int> hash;
       hash.insert(1,1);
       int value = hash[1];
       ASSERT_TRUE(value == 1);
   }


} test_hash_test;

