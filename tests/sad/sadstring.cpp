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
       TEST(SadStringTest::testTrimSpaces)
   ) {}
   
   void testSplitKeepEmptyParts()
   {
       {
	       sad::String s = "separator;;;";
           sad::StringList list = s.split(";", sad::String::KEEP_EMPTY_PARTS);
	       ASSERT_TRUE( list.size() == 3 );
       }
       {
           sad::String s = "separator;;;sep";
           sad::StringList list = s.split(";", sad::String::KEEP_EMPTY_PARTS);
	       ASSERT_TRUE( list.size() == 4 );
       }
   }

   void testSplitOmitEmptyParts()
   {
	   {
	       sad::String s = "separator;;;";
           sad::StringList list = s.split(";", sad::String::OMIT_EMPTY_PARTS);
	       ASSERT_TRUE( list.size() == 1 );
       }
       {
           sad::String s = "separator;;;sep";
           sad::StringList list = s.split(";", sad::String::OMIT_EMPTY_PARTS);
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

} _sad_string_test;
