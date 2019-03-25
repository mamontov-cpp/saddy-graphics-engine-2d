/*! Due to fast implementation of normal to surface for rectangle there is A LOT of bugs
    for it
 */
#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "p2d/rectangle.h"
#pragma warning(pop)


/*!
 * Tests collides1d operation
 */
struct RectangleNormalToSurfaceTest : tpunit::TestFixture
{
 public:
   RectangleNormalToSurfaceTest() : tpunit::TestFixture(
       TEST(RectangleNormalToSurfaceTest::test1),	
       TEST(RectangleNormalToSurfaceTest::test2),	
       TEST(RectangleNormalToSurfaceTest::test3),	
       TEST(RectangleNormalToSurfaceTest::test4),	
       TEST(RectangleNormalToSurfaceTest::test5),	
       TEST(RectangleNormalToSurfaceTest::test6),	
       TEST(RectangleNormalToSurfaceTest::test7),	
       TEST(RectangleNormalToSurfaceTest::test8)
   ) {}

   sad::p2d::Rectangle  testRect1()
   {
       sad::p2d::Rectangle r;
       r.setRect(
            sad::Rect2D(sad::Point2D(108, 0), sad::Point2D(192, 0),
                   sad::Point2D(108, 20), sad::Point2D(192, 20))	  
       );
       return  r; 
   }
   
   void test1()
   {
       sad::p2d::Rectangle r = this->testRect1();
       sad::p2d::Vector n;
       r.normalToPointOnSurface(sad::p2d::Point(108, 0), n);
       ASSERT_FLOAT_NEAR(n.x(), -M_SQRT1_2, 1.0E-2);
       ASSERT_FLOAT_NEAR(n.y(), -M_SQRT1_2, 1.0E-4);
   }

   void test2()
   {
       sad::p2d::Rectangle r = this->testRect1();
       sad::p2d::Vector n;
       r.normalToPointOnSurface(sad::p2d::Point(192, 0), n);
       ASSERT_FLOAT_NEAR(n.x(), M_SQRT1_2, 1.0E-4);
       ASSERT_FLOAT_NEAR(n.y(), -M_SQRT1_2, 1.0E-4);
   }

   void test3()
   {
       sad::p2d::Rectangle r = this->testRect1();
       sad::p2d::Vector n;
       r.normalToPointOnSurface(sad::p2d::Point(108, 20), n);
       ASSERT_FLOAT_NEAR(n.x(), -M_SQRT1_2, 1.0E-4);
       ASSERT_FLOAT_NEAR(n.y(), M_SQRT1_2, 1.0E-4);
   }

   void test4()
   {
       sad::p2d::Rectangle r = this->testRect1();
       sad::p2d::Vector n;
       r.normalToPointOnSurface(sad::p2d::Point(192, 20), n);
       ASSERT_FLOAT_NEAR(n.x(),M_SQRT1_2, 1.0E-4);
       ASSERT_FLOAT_NEAR(n.y(), M_SQRT1_2, 1.0E-4);
   }

   void test5()
   {
       sad::p2d::Rectangle r = this->testRect1();
       sad::p2d::Vector n;
       r.normalToPointOnSurface(sad::p2d::Point(150, 0), n);
       ASSERT_FLOAT_NEAR(n.x(), 0, 1.0E-4);
       ASSERT_FLOAT_NEAR(n.y(), -1, 1.0E-4);
   }

   void test6()
   {
       sad::p2d::Rectangle r = this->testRect1();
       sad::p2d::Vector n;
       r.normalToPointOnSurface(sad::p2d::Point(150, 20), n);
       ASSERT_FLOAT_NEAR(n.x(), 0, 1.0E-4);
       ASSERT_FLOAT_NEAR(n.y(), 1, 1.0E-4);
   }

   void test7()
   {
       sad::p2d::Rectangle r = this->testRect1();
       sad::p2d::Vector n;
       r.normalToPointOnSurface(sad::p2d::Point(108, 10), n);
       ASSERT_FLOAT_NEAR(n.x(), -1, 1.0E-4);
       ASSERT_FLOAT_NEAR(n.y(), 0, 1.0E-4);
   }

   void test8()
   {
       sad::p2d::Rectangle r = this->testRect1();
       sad::p2d::Vector n;
       r.normalToPointOnSurface(sad::p2d::Point(192, 10), n);
       ASSERT_FLOAT_NEAR(n.x(), 1, 1.0E-4);
       ASSERT_FLOAT_NEAR(n.y(), 0, 1.0E-4);
   }

} _rectangle_normal_to_surface_test;
