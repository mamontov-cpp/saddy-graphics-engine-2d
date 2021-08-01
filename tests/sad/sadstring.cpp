#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "sadstring.h"
#include "fuzzyequal.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

/*!
 * Tests sad::Size compilation
 */
struct SadStringTest : tpunit::TestFixture
{
 public:
   SadStringTest() : tpunit::TestFixture(
       TEST(SadStringTest::testSplitKeepEmptyParts),
       TEST(SadStringTest::testSplitOmitEmptyParts),
       TEST(SadStringTest::testTrimSpaces),
       TEST(SadStringTest::testParseUInt)
   ) {}
   
   void testSplitKeepEmptyParts()
   {
       {
           sad::String s = "separator;;;";
           sad::StringList list = s.split(";", sad::String::SplitBehaviour::KEEP_EMPTY_PARTS);
           ASSERT_TRUE( list.size() == 3 );
       }
       {
           sad::String s = "separator;;;sep";
           sad::StringList list = s.split(";", sad::String::SplitBehaviour::KEEP_EMPTY_PARTS);
           ASSERT_TRUE( list.size() == 4 );
       }
   }

   void testSplitOmitEmptyParts()
   {
       {
           sad::String s = "separator;;;";
           sad::StringList list = s.split(";", sad::String::SplitBehaviour::OMIT_EMPTY_PARTS);
           ASSERT_TRUE( list.size() == 1 );
       }
       {
           sad::String s = "separator;;;sep";
           sad::StringList list = s.split(";", sad::String::SplitBehaviour::OMIT_EMPTY_PARTS);
           ASSERT_TRUE( list.size() == 2 );
       }
   }

   void testTrimSpaces()
   {
       {
           sad::String s = "string";
           s.trimSpaces();
           ASSERT_TRUE( s == "string" );
       }
       {
           sad::String s = "   string    ";
           s.trimSpaces();
           ASSERT_TRUE( s == "string" );
       }
       {
           sad::String s = "    string string2   ";
           s.trimSpaces();
           ASSERT_TRUE( s == "string string2" );
       }
       {
           sad::String s = "    string string2";
           s.trimSpaces();
           ASSERT_TRUE( s == "string string2" );
       }
       {
           sad::String s = "string string2   ";
           s.trimSpaces();
           ASSERT_TRUE( s == "string string2" );
       }
       {
           sad::String s = "       ";
           s.trimSpaces();
           ASSERT_TRUE( s.length() == 0 );
       }
   }

   void testParseUInt()
   {
        {
            unsigned int f = 22;
            const char* b = "a";
            ASSERT_TRUE( sad::String::parseUInt(b, b, &f) == false );
            ASSERT_TRUE( f == 22);
        }

        {
            unsigned int f = 22;
            const char* b = "a23";
            ASSERT_TRUE( sad::String::parseUInt(b, b + 2, &f) == false );
            ASSERT_TRUE( f == 22);
        }

        {
            unsigned int f = 22;
            const char* b = "12a3";
            ASSERT_TRUE( sad::String::parseUInt(b, b + 3, &f) == false );
            ASSERT_TRUE( f == 22);
        }

        {
            unsigned int f = 22;
            const char* b = "0";
            ASSERT_TRUE( sad::String::parseUInt(b, b, &f) == true );
            ASSERT_TRUE( f == 0);
        }

        {
            unsigned int f = 22;
            const char* b = "9";
            ASSERT_TRUE( sad::String::parseUInt(b, b, &f) == true );
            ASSERT_TRUE( f == 9);
        }

        {
            unsigned int f = 22;
            const char* b = "192";
            ASSERT_TRUE( sad::String::parseUInt(b, b + 2, &f) == true );
            ASSERT_TRUE( f == 192);
        }

        {
            unsigned int f = 22;
            const char* b = "092";
            ASSERT_TRUE( sad::String::parseUInt(b, b + 2, &f) == true );
            ASSERT_TRUE( f == 92);
        }

        {
            unsigned int f = 22;
            const char* b = "10034";
            ASSERT_TRUE( sad::String::parseUInt(b, b + 4, &f) == true );
            ASSERT_TRUE( f == 10034);
        }
   }

} _sad_string_test;
