#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdio>
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
       TEST(AxleTest::testProject),
       TEST(AxleTest::testCollides)
   ) {}
   
   void testProject()
   {
       sad::p2d::Axle axle;
       sad::p2d::Cutter2D  v;
       sad::p2d::Cutter1D  c;
       axle = sad::p2d::axle(sad::Point2D(0,0), sad::Point2D(10,0));
       v = sad::p2d::Cutter2D(sad::Point2D(2,2), sad::Point2D(5,6));
       c = sad::p2d::project(v, axle);
       ASSERT_FLOAT_EQUAL(c.p1(), 2);
       ASSERT_FLOAT_EQUAL(c.p2(), 5);

       axle = sad::p2d::axle(sad::Point2D(0,0), sad::Point2D(0,10));
       v = sad::p2d::Cutter2D(sad::Point2D(2,2), sad::Point2D(5,6));
       c = sad::p2d::project(v, axle);
       ASSERT_FLOAT_EQUAL(c.p1(), 2);
       ASSERT_FLOAT_EQUAL(c.p2(), 6);

       axle = sad::p2d::axle(sad::Point2D(0,0), sad::Point2D(1,1));
       v = sad::p2d::Cutter2D(sad::Point2D(2,2), sad::Point2D(3,3));
       c = sad::p2d::project(v, axle);
       ASSERT_FLOAT_EQUAL(c.p1(), 2 * M_SQRT2 );
       ASSERT_FLOAT_EQUAL(c.p2(), sqrt(18.0) );

   }

   void testCollides()
   {
       // There is not much tests, because for most part
       // implementation relies on tested sad::p2d::collides1d
       ASSERT_FALSE( sad::p2d::collides( sad::p2d::Cutter1D(0.0, 0.5), 
                                    sad::p2d::Cutter1D(1.0, 2.0)
                                  ) );
       ASSERT_TRUE( sad::p2d::collides( sad::p2d::Cutter1D(0.0, 1.0), 
                                   sad::p2d::Cutter1D(1.0, 2.0)
                                   ) );
   }

} test4;