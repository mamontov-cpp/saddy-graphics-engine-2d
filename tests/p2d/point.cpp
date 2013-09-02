#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "p2d/point.h"
#pragma warning(pop)


/*!
 * Tests point operations
 */
struct PointTest : tpunit::TestFixture
{
 public:
   PointTest() : tpunit::TestFixture(
	   TEST(PointTest::testDistance)	   
   ) {}
   
   void testDistance()
   {
	   ASSERT_FLOAT_EQUAL( p2d::Point(0.0, 0.0).distanceTo(p2d::Point(0.0,0.0)), 0);
	   ASSERT_FLOAT_EQUAL( p2d::Point(0.0, 0.0).distanceTo(p2d::Point(0.0,1.0)), 1);
	   ASSERT_FLOAT_EQUAL( p2d::Point(0.0, 0.0).distanceTo(p2d::Point(1.0,1.0)), M_SQRT2);
   }

} test2;