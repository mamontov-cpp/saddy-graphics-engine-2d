#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include <3rdparty/tpunit++/tpunit++.hpp>
#include <sadvector.h>
#include <p2d/infiniteline.h>
#include <geometry2d.h>
#pragma warning(pop)


/*!
 * Tests intersection, when we are determining intersection of applied vector
 * and a cutter
 */
struct TestAVIntersection : tpunit::TestFixture
{
 public:
   TestAVIntersection() : tpunit::TestFixture(
       TEST(TestAVIntersection::test1),
       TEST(TestAVIntersection::test2),
       TEST(TestAVIntersection::test3),
       TEST(TestAVIntersection::test4),
       TEST(TestAVIntersection::test5),
       TEST(TestAVIntersection::test6)
   ) {}
   
   void test1()
   {
       sad::p2d::MaybePoint t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(1, 0),
           sad::p2d::cutter( 2, 0, 4, 0) 
       );
       ASSERT_TRUE( t.exists() );
       ASSERT_FLOAT_EQUAL( t.value().x() , 2);
       ASSERT_FLOAT_EQUAL( t.value().y() , 0);
       t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(1, 0),
           sad::p2d::cutter( -2, 0, -4, 0) 
       );
       ASSERT_TRUE( t.exists() );
       ASSERT_FLOAT_EQUAL( t.value().x() , -2);
       ASSERT_FLOAT_EQUAL( t.value().y() , 0);
       t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(1, 0),
           sad::p2d::cutter( -2, 1, -4, 1) 
       );
       ASSERT_FALSE( t.exists() );
       t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(1, 0),
           sad::p2d::cutter( -2, -1, -4, -1) 
       );
       ASSERT_FALSE( t.exists() );
   }

   void test2()
   {
       sad::p2d::MaybePoint t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(0, 1),
           sad::p2d::cutter( 0, 2, 0, 4) 
       );
       ASSERT_TRUE( t.exists() );
       ASSERT_FLOAT_EQUAL( t.value().x() , 0);
       ASSERT_FLOAT_EQUAL( t.value().y() , 2);
       t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(0, 1),
           sad::p2d::cutter( 0, -2, 0, -4) 
       );
       ASSERT_TRUE( t.exists() );
       ASSERT_FLOAT_EQUAL( t.value().x() , 0);
       ASSERT_FLOAT_EQUAL( t.value().y() , -2);
       t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(0, 1),
           sad::p2d::cutter( 1, -2,  1, -4) 
       );
       ASSERT_FALSE( t.exists() );
       t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(1, 0),
           sad::p2d::cutter( -1, -2, -1, -4) 
       );
       ASSERT_FALSE( t.exists() );
   }

   void test3()
   {
       sad::p2d::MaybePoint t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(1, 1),
           sad::p2d::cutter( 2, 2, 4, 4) 
       );
       ASSERT_TRUE( t.exists() );
       ASSERT_FLOAT_EQUAL( t.value().x() , 2);
       ASSERT_FLOAT_EQUAL( t.value().y() , 2);
       t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(1, 1),
           sad::p2d::cutter( -2, -2, -4, -4) 
       );
       ASSERT_TRUE( t.exists() );
       ASSERT_FLOAT_EQUAL( t.value().x() , -2);
       ASSERT_FLOAT_EQUAL( t.value().y() , -2);
       t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(1, 1),
           sad::p2d::cutter( 1, -2,  1, -4) 
       );
       ASSERT_FALSE( t.exists() );
       t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(1, 1),
           sad::p2d::cutter( -1, -2, -1, -4) 
       );
       ASSERT_FALSE( t.exists() );
   }

   void test4()
   {
       sad::p2d::MaybePoint t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(1, 0),
           sad::p2d::cutter( 2, 2, 2, -2) 
       );
       ASSERT_TRUE( t.exists() );
       ASSERT_FLOAT_EQUAL( t.value().x() , 2);
       ASSERT_FLOAT_EQUAL( t.value().y() , 0);
   }
    
   void test5()
   {
       sad::p2d::MaybePoint t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(0, 1),
           sad::p2d::cutter( -2, 2, 2, 2) 
       );
       ASSERT_TRUE( t.exists() );
       ASSERT_FLOAT_EQUAL( t.value().x() , 0);
       ASSERT_FLOAT_EQUAL( t.value().y() , 2);
   }

   void test6()
   {
       sad::p2d::MaybePoint t = sad::p2d::intersection(
           sad::p2d::Point(0, 0), 
           sad::p2d::Vector(1, 1),
           sad::p2d::cutter( 0, 4, 4, 0) 
       );
       ASSERT_TRUE( t.exists() );
       ASSERT_FLOAT_EQUAL( t.value().x() , 2);
       ASSERT_FLOAT_EQUAL( t.value().y() , 2);
   }

} test_av_intersection;