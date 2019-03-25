#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "geometry2d.h"
#pragma warning(pop)


/*!
 * Tests collides1d operation
 */
struct AngleOfTest : tpunit::TestFixture
{
 public:
   AngleOfTest() : tpunit::TestFixture(
       TEST(AngleOfTest::testZero),
       TEST(AngleOfTest::testOneOfZero),
       TEST(AngleOfTest::testCommon)
   ) {}
   
   void testZero()
   {
       ASSERT_FLOAT_EQUAL( sad::angleOf(0, 0),  0);
   }

   void testOneOfZero()
   {
       ASSERT_FLOAT_EQUAL( sad::angleOf(-1, 0), M_PI );
       ASSERT_FLOAT_EQUAL( sad::angleOf(0, 1), M_PI /2 );
       ASSERT_FLOAT_EQUAL( sad::angleOf(1, 0), 0 );
       ASSERT_FLOAT_EQUAL( sad::angleOf(0, -1), M_PI * 1.5 );
   }

   void testCommon()
   {
       ASSERT_FLOAT_EQUAL( sad::angleOf(-1, 1), 3 * M_PI / 4 );
       ASSERT_FLOAT_EQUAL( sad::angleOf(1, 1),  M_PI / 4 );
       ASSERT_FLOAT_EQUAL( sad::angleOf(1, -1), 7 * M_PI / 4 );
       ASSERT_FLOAT_EQUAL( sad::angleOf(-1, -1), 5 * M_PI / 4 );
   }

} _test_angle_of;
