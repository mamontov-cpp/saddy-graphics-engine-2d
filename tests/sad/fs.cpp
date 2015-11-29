#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "sadstring.h"
#include "fuzzyequal.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "util/fs.h"
#pragma warning(pop)

/*!
 * Tests sad::Size compilation
 */
struct SadUtilFSTest : tpunit::TestFixture
{
 public:
   SadUtilFSTest() : tpunit::TestFixture(
       TEST(SadUtilFSTest::testCanonicalizeAbsolutePathOnWindows),
       TEST(SadUtilFSTest::testCanonicalizeRelativePathOnWindows),
       TEST(SadUtilFSTest::testCanonicalizeAbsolutePathOnLinux),
       TEST(SadUtilFSTest::testCanonicalizeRelativePathOnLinux)
   ) {}
   
   void testCanonicalizeAbsolutePathOnWindows()
   {
      sad::String result;
      result = sad::util::canonicalizePath("C:\\1\\2\\3.txt");
      ASSERT_TRUE( result == "C:\\1\\2\\3.txt");
      result = sad::util::canonicalizePath("C:\\1\\2\\..\\..\\3.txt");
      ASSERT_TRUE( result == "C:\\3.txt");
      result = sad::util::canonicalizePath("C:\\1\\2\\..\\..\\..\\..\\..\\");
      ASSERT_TRUE( result == "C:");
      result = sad::util::canonicalizePath("C:\\1\\2\\..\\..\\..\\..\\..\\3.txt");
      ASSERT_TRUE( result == "C:\\3.txt");
      result = sad::util::canonicalizePath("C:\\1\\2\\..\\..\\..\\..\\../2/3.txt");
      ASSERT_TRUE( result == "C:\\2\\3.txt");
   }

   void testCanonicalizeRelativePathOnWindows()
   {
      sad::String result;
      result = sad::util::canonicalizePath("1\\2\\3.txt");
      ASSERT_TRUE( result == "1\\2\\3.txt");
      result = sad::util::canonicalizePath("1\\2\\..\\..\\3.txt");
      ASSERT_TRUE( result == "3.txt");
      result = sad::util::canonicalizePath("1\\2\\..\\..\\..\\..\\..\\");
      ASSERT_TRUE( result == "..\\..\\..");
      result = sad::util::canonicalizePath("1\\2\\..\\..\\..\\..\\..\\3.txt");
      ASSERT_TRUE( result == "..\\..\\..\\3.txt");
      result = sad::util::canonicalizePath("1\\2\\..\\..\\..\\..\\../2/3.txt");
      ASSERT_TRUE( result == "..\\..\\..\\2\\3.txt");	   
   }

   void testCanonicalizeAbsolutePathOnLinux()
   {
      sad::String result;
      
      result = sad::util::canonicalizePath("/1/2/3.txt");
      ASSERT_TRUE( result == "/1/2/3.txt");
      result = sad::util::canonicalizePath("/1/2/../../3.txt");
      ASSERT_TRUE( result == "/3.txt");
      result = sad::util::canonicalizePath("/1/2/../../../../../");
      ASSERT_TRUE( result == "/");
      result = sad::util::canonicalizePath("/1/2/../../../../../3.txt");
      ASSERT_TRUE( result == "/3.txt");	   
   }

   void testCanonicalizeRelativePathOnLinux()
   {
      sad::String result;

      result = sad::util::canonicalizePath("1/2/3.txt");
      ASSERT_TRUE( result == "1/2/3.txt");
      result = sad::util::canonicalizePath("1/2/../../3.txt");
      ASSERT_TRUE( result == "3.txt");
      result = sad::util::canonicalizePath("1/2/../../../../../");
      ASSERT_TRUE( result == "../../..");
      result = sad::util::canonicalizePath("1/2/../../../../../3.txt");
      ASSERT_TRUE( result == "../../../3.txt");	   	   
   }


} _sad_util_fs_test;
