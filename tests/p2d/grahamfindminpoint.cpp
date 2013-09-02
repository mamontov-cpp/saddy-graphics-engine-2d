#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <3rdparty/tpunit++/tpunit++.hpp>
#include <templates/hlvector.hpp>
#include <p2d/point.h>
#pragma warning(pop)

namespace p2d
{
int find_min_point_on_y_axis(const hst::vector<p2d::Point> & set);
}


/*!
 * Tests graham find_min_point_on_y_axis function
 */
struct GrahamFindMinPointTest : tpunit::TestFixture
{
 public:
   GrahamFindMinPointTest() : tpunit::TestFixture(
	   TEST(GrahamFindMinPointTest::testEmpty),
	   TEST(GrahamFindMinPointTest::testOne),
	   TEST(GrahamFindMinPointTest::testTwoSequential),
	   TEST(GrahamFindMinPointTest::testTwoReversal),
	   TEST(GrahamFindMinPointTest::testThree)
   ) {}
   
   void testEmpty()
   {
	   hst::vector<p2d::Point> p;
	   int i = p2d::find_min_point_on_y_axis(p);
	   ASSERT_EQUAL(i, -1);
   }

   void testOne()
   {
	   hst::vector<p2d::Point> p;
	   p << p2d::Point(2,2);
	   int i = p2d::find_min_point_on_y_axis(p);
	   ASSERT_EQUAL(i, 0);
   }

   void testTwoSequential()
   {
	   hst::vector<p2d::Point> p;
	   p << p2d::Point(2,2);
	   p << p2d::Point(3,3);
	   int i = p2d::find_min_point_on_y_axis(p);
	   ASSERT_EQUAL(i, 0);
   }

   void testTwoReversal()
   {
	   hst::vector<p2d::Point> p;
	   p << p2d::Point(3,3);
	   p << p2d::Point(2,2);
	   int i = p2d::find_min_point_on_y_axis(p);
	   ASSERT_EQUAL(i, 1);
   }

   void testThree()
   {
	   hst::vector<p2d::Point> p;
	   p << p2d::Point(3,4);
	   p << p2d::Point(1,2);
	   p << p2d::Point(5,6);
	   int i = p2d::find_min_point_on_y_axis(p);
	   ASSERT_EQUAL(i, 1);
   }

} test5;