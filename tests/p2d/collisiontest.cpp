#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include <3rdparty/tpunit++/tpunit++.hpp>
#include <sadvector.h>
#include <p2d/point.h>
#include <p2d/collisiontest.h>
#pragma warning(pop)

/*!
 * Tests collision detector for various shapes
 */
struct CollisionTestTest : tpunit::TestFixture
{
 public:
   CollisionTestTest() : tpunit::TestFixture(
       TEST(CollisionTestTest::testRtoR1),
       TEST(CollisionTestTest::testRtoC1),
       TEST(CollisionTestTest::testRtoL1),
       TEST(CollisionTestTest::testCtoC1),
       TEST(CollisionTestTest::testCtoL1),
       TEST(CollisionTestTest::testLtoL1),
       TEST(CollisionTestTest::testRtoB1)
   ) {}
   
   void testRtoR1()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Rectangle r2;
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 2), sad::Point2D(1, 3),
                   sad::Point2D(2, 2), sad::Point2D(1, 1))	  
       );
       r2.setRect(
            sad::Rect2D(sad::Point2D(2,0), sad::Point2D(2, 1.5),
                   sad::Point2D(4, 1.5), sad::Point2D(4, 0))
       );
       sad::p2d::CollisionTest t;
       ASSERT_FALSE( t.invoke(&r1, &r2) );
       ASSERT_FALSE( t.invoke(&r2, &r1) );
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 0), sad::Point2D(0, 2),
                   sad::Point2D(2, 2), sad::Point2D(2, 0))	  
       );
       r2.setRect(
            sad::Rect2D(sad::Point2D(0,1), sad::Point2D(1, 2),
                   sad::Point2D(2, 1), sad::Point2D(1, 0))
       );
       ASSERT_TRUE( t.invoke(&r1, &r2) );
       ASSERT_TRUE( t.invoke(&r2, &r1) );
       r2.setRect(
            sad::Rect2D(sad::Point2D(1,1), sad::Point2D(2, 2),
                   sad::Point2D(3, 1), sad::Point2D(2, 0))
       );
       ASSERT_TRUE( t.invoke(&r1, &r2) );
       ASSERT_TRUE( t.invoke(&r2, &r1) );
   }

   void testRtoC1()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 2), sad::Point2D(1, 3),
                   sad::Point2D(2, 2), sad::Point2D(1, 1))	  
       );
       r2.setCenter( sad::p2d::Point(1,1) );
       r2.setRadius(1);
       sad::p2d::CollisionTest t;
       ASSERT_TRUE( t.invoke(&r1, &r2) );
       ASSERT_TRUE( t.invoke(&r2, &r1) );
       r2.setCenter( sad::p2d::Point(1,1) );
       r2.setRadius(2);
       ASSERT_TRUE( t.invoke(&r1, &r2) );
       ASSERT_TRUE( t.invoke(&r2, &r1) );
       r2.setCenter( sad::p2d::Point(-1,-1) );
       r2.setRadius(1);
       ASSERT_FALSE( t.invoke(&r1, &r2) );
       ASSERT_FALSE( t.invoke(&r2, &r1) );
   }

   void testRtoL1()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 2), sad::Point2D(1, 3),
                   sad::Point2D(2, 2), sad::Point2D(1, 1))	  
       );
       r2.setCutter( 1, 0,  3, 2 );
       sad::p2d::CollisionTest t;
       ASSERT_FALSE( t.invoke(&r1, &r2) );
       ASSERT_FALSE( t.invoke(&r2, &r1) );
       r2.setCutter( 2, 1, 2, 3 );
       ASSERT_TRUE( t.invoke(&r1, &r2) );
       ASSERT_TRUE( t.invoke(&r2, &r1) );
       r2.setCutter( 1, 3, 2, 2 );
       ASSERT_TRUE( t.invoke(&r1, &r2) );
       ASSERT_TRUE( t.invoke(&r2, &r1) );
   }

   void testCtoC1()
   {
       sad::p2d::Circle r1;
       sad::p2d::Circle r2;
       r1.setCenter( sad::p2d::Point(0,0) );
       r1.setRadius(1);
       r2.setCenter( sad::p2d::Point(2, -2) );
       r2.setRadius(1);
       sad::p2d::CollisionTest t;
       ASSERT_FALSE( t.invoke(&r1, &r2) );
       ASSERT_FALSE( t.invoke(&r2, &r1) );
       r2.setCenter( sad::p2d::Point(1, -1) );
       ASSERT_TRUE( t.invoke(&r1, &r2) );
       ASSERT_TRUE( t.invoke(&r2, &r1) );	   
   }

   void testCtoL1()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       r1.setCenter( sad::p2d::Point(0,0) );
       r1.setRadius(1);
       r2.setCutter( -1, -4, 4, 3 );
       sad::p2d::CollisionTest t;
       ASSERT_FALSE( t.invoke(&r1, &r2) );
       ASSERT_FALSE( t.invoke(&r2, &r1) );
       r2.setCutter( 1, -1,  1, 1 );
       ASSERT_TRUE( t.invoke(&r1, &r2) );
       ASSERT_TRUE( t.invoke(&r2, &r1) );
       r2.setCutter( 1, -5, 1,  5 );
       ASSERT_TRUE( t.invoke(&r1, &r2) );
       ASSERT_TRUE( t.invoke(&r2, &r1) );
   }

   void testLtoL1()
   {
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       r1.setCutter( 0, 1, 1, 0 );
       r2.setCutter( 0, 3, 3, 0 );
       sad::p2d::CollisionTest t;
       ASSERT_FALSE( t.invoke(&r1, &r2) );
       ASSERT_FALSE( t.invoke(&r2, &r1) );
       r1.setCutter( 0, 3, 3, 0 );
       r2.setCutter( 1, 1, 2, 2 );
       ASSERT_TRUE( t.invoke(&r1, &r2) );
       ASSERT_TRUE( t.invoke(&r2, &r1) );
       r2.setCutter( 0, 0, 3, 0 );
       ASSERT_TRUE( t.invoke(&r1, &r2) );
       ASSERT_TRUE( t.invoke(&r2, &r1) );
   }

   void testRtoB1()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Bound r2;
       r1.setRect(
            sad::Rect2D(sad::Point2D(108, 0), sad::Point2D(192, 0),
                   sad::Point2D(108, 20), sad::Point2D(192, 20))	  
       );
       r2.setType(sad::p2d::BT_DOWN);
       r2.setPosition(0);
       sad::p2d::CollisionTest t;
       ASSERT_TRUE( t.invoke(&r1, &r2) );
       ASSERT_TRUE( t.invoke(&r2, &r1) );
   }

} test6;