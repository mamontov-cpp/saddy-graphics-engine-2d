#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <3rdparty/tpunit++/tpunit++.hpp>
#include <templates/hlvector.hpp>
#include <p2d/infiniteline.h>
#include <extra/geometry2d.h>
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
	   p2d::MaybePoint t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(1, 0),
		   p2d::cutter( 2, 0, 4, 0) 
	   );
	   ASSERT_TRUE( t.exists() );
	   ASSERT_FLOAT_EQUAL( t.data().x() , 2);
	   ASSERT_FLOAT_EQUAL( t.data().y() , 0);
	   t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(1, 0),
		   p2d::cutter( -2, 0, -4, 0) 
	   );
	   ASSERT_TRUE( t.exists() );
	   ASSERT_FLOAT_EQUAL( t.data().x() , -2);
	   ASSERT_FLOAT_EQUAL( t.data().y() , 0);
	   t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(1, 0),
		   p2d::cutter( -2, 1, -4, 1) 
	   );
	   ASSERT_FALSE( t.exists() );
	   t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(1, 0),
		   p2d::cutter( -2, -1, -4, -1) 
	   );
	   ASSERT_FALSE( t.exists() );
   }

   void test2()
   {
	   p2d::MaybePoint t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(0, 1),
		   p2d::cutter( 0, 2, 0, 4) 
	   );
	   ASSERT_TRUE( t.exists() );
	   ASSERT_FLOAT_EQUAL( t.data().x() , 0);
	   ASSERT_FLOAT_EQUAL( t.data().y() , 2);
	   t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(0, 1),
		   p2d::cutter( 0, -2, 0, -4) 
	   );
	   ASSERT_TRUE( t.exists() );
	   ASSERT_FLOAT_EQUAL( t.data().x() , 0);
	   ASSERT_FLOAT_EQUAL( t.data().y() , -2);
	   t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(0, 1),
		   p2d::cutter( 1, -2,  1, -4) 
	   );
	   ASSERT_FALSE( t.exists() );
	   t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(1, 0),
		   p2d::cutter( -1, -2, -1, -4) 
	   );
	   ASSERT_FALSE( t.exists() );
   }

   void test3()
   {
	   p2d::MaybePoint t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(1, 1),
		   p2d::cutter( 2, 2, 4, 4) 
	   );
	   ASSERT_TRUE( t.exists() );
	   ASSERT_FLOAT_EQUAL( t.data().x() , 2);
	   ASSERT_FLOAT_EQUAL( t.data().y() , 2);
	   t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(1, 1),
		   p2d::cutter( -2, -2, -4, -4) 
	   );
	   ASSERT_TRUE( t.exists() );
	   ASSERT_FLOAT_EQUAL( t.data().x() , -2);
	   ASSERT_FLOAT_EQUAL( t.data().y() , -2);
	   t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(1, 1),
		   p2d::cutter( 1, -2,  1, -4) 
	   );
	   ASSERT_FALSE( t.exists() );
	   t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(1, 1),
		   p2d::cutter( -1, -2, -1, -4) 
	   );
	   ASSERT_FALSE( t.exists() );
   }

   void test4()
   {
	   p2d::MaybePoint t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(1, 0),
		   p2d::cutter( 2, 2, 2, -2) 
	   );
	   ASSERT_TRUE( t.exists() );
	   ASSERT_FLOAT_EQUAL( t.data().x() , 2);
	   ASSERT_FLOAT_EQUAL( t.data().y() , 0);
   }
	
   void test5()
   {
	   p2d::MaybePoint t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(0, 1),
		   p2d::cutter( -2, 2, 2, 2) 
	   );
	   ASSERT_TRUE( t.exists() );
	   ASSERT_FLOAT_EQUAL( t.data().x() , 0);
	   ASSERT_FLOAT_EQUAL( t.data().y() , 2);
   }

   void test6()
   {
	   p2d::MaybePoint t = p2d::intersection(
		   p2d::Point(0, 0), 
		   p2d::Vector(1, 1),
		   p2d::cutter( 0, 4, 4, 0) 
	   );
	   ASSERT_TRUE( t.exists() );
	   ASSERT_FLOAT_EQUAL( t.data().x() , 2);
	   ASSERT_FLOAT_EQUAL( t.data().y() , 2);
   }

} test_av_intersection;