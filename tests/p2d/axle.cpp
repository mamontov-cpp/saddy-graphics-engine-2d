#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "p2d/axle.h"
#pragma warning(pop)


/*!
 * Tests point operations
 */
struct AxleTest : tpunit::TestFixture
{
 public:
   AxleTest() : tpunit::TestFixture(
	   TEST(AxleTest::test_project),
	   TEST(AxleTest::test_collides)
   ) {}
   
   void test_project()
   {
	   p2d::Axle axle;
	   p2d::Cutter2D  v;
	   p2d::Cutter1D  c;
	   axle = p2d::axle(hPointF(0,0), hPointF(10,0));
	   v = p2d::Cutter2D(hPointF(2,2), hPointF(5,6));
	   c = p2d::project(v, axle);
	   ASSERT_FLOAT_EQUAL(c.p1(), 2);
	   ASSERT_FLOAT_EQUAL(c.p2(), 5);

	   axle = p2d::axle(hPointF(0,0), hPointF(0,10));
	   v = p2d::Cutter2D(hPointF(2,2), hPointF(5,6));
	   c = p2d::project(v, axle);
	   ASSERT_FLOAT_EQUAL(c.p1(), 2);
	   ASSERT_FLOAT_EQUAL(c.p2(), 6);

	   axle = p2d::axle(hPointF(0,0), hPointF(1,1));
	   v = p2d::Cutter2D(hPointF(2,2), hPointF(3,3));
	   c = p2d::project(v, axle);
	   ASSERT_FLOAT_EQUAL(c.p1(), 2 * M_SQRT2 );
	   ASSERT_FLOAT_EQUAL(c.p2(), sqrt(18.0) );

   }

   void test_collides()
   {
	   // There is not much tests, because for most part
	   // implementation relies on tested p2d::collides1d
	   ASSERT_FALSE( p2d::collides( p2d::Cutter1D(0.0, 0.5), 
		                            p2d::Cutter1D(1.0, 2.0)
								  ) );
	   ASSERT_TRUE( p2d::collides( p2d::Cutter1D(0.0, 1.0), 
		                           p2d::Cutter1D(1.0, 2.0)
								   ) );
   }

} test4;