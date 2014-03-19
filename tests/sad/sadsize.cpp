#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "sadsize.h"
#include "fuzzyequal.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

/*!
 * Tests sad::Size compilation
 */
struct SadSizeTest : tpunit::TestFixture
{
 public:
   SadSizeTest() : tpunit::TestFixture(
	   TEST(SadSizeTest::testDefaultConstructor),
	   TEST(SadSizeTest::testParametetricConstructor)
   ) {}
   
   void testDefaultConstructor()
   {
	   sad::Size2D s;
	   ASSERT_TRUE( sad::is_fuzzy_zero(s.Width) );
	   ASSERT_TRUE( sad::is_fuzzy_zero(s.Height) );
   }

   void testParametetricConstructor()
   {
	   sad::Size2D s(3.0, 4.0);
	   ASSERT_TRUE( sad::is_fuzzy_equal(s.Width, 3.0) );
	   ASSERT_TRUE( sad::is_fuzzy_equal(s.Height, 4.0) );
   }

} _sad_size_test;
