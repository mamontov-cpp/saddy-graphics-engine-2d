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
void erase_equal_points(hst::vector<p2d::Point> & set);
}

/*!
 * Tests graham erase equal function
 */
struct GrahamTestEqual : tpunit::TestFixture
{
 public:
   GrahamTestEqual() : tpunit::TestFixture(
	   TEST(GrahamTestEqual::testEmpty),
	   TEST(GrahamTestEqual::testOne),
	   TEST(GrahamTestEqual::testTwo),
	   TEST(GrahamTestEqual::testTwoEqual),
	   TEST(GrahamTestEqual::testFiveNonEqual),
	   TEST(GrahamTestEqual::testComplex)
   ) {}
   
   void testEmpty()
   {
	   hst::vector<p2d::Point> p;
	   p2d::erase_equal_points(p);
	   ASSERT_EQUAL(p.size(), 0);
   }

   void testOne()
   {
	   hst::vector<p2d::Point> p;
	   p << p2d::Point(2,2);
	   p2d::erase_equal_points(p);
	   ASSERT_EQUAL(p.size(), 1);
	   ASSERT_FLOAT_EQUAL(p[0].x(), 2);
	   ASSERT_FLOAT_EQUAL(p[0].y(), 2);
   }

   void testTwo()
   {
	   hst::vector<p2d::Point> p;
	   p << p2d::Point(2,2);
	   p << p2d::Point(3,3);
	   p2d::erase_equal_points(p);
	   ASSERT_EQUAL(p.size(), 2);
	   ASSERT_FLOAT_EQUAL(p[0].x(), 2);
	   ASSERT_FLOAT_EQUAL(p[0].y(), 2);
	   ASSERT_FLOAT_EQUAL(p[1].x(), 3);
	   ASSERT_FLOAT_EQUAL(p[1].y(), 3);
   }

   void testTwoEqual()
   {
	   hst::vector<p2d::Point> p;
	   p << p2d::Point(2,2);
	   p << p2d::Point(2,2);
	   p2d::erase_equal_points(p);
	   ASSERT_EQUAL(p.size(), 1);
	   ASSERT_FLOAT_EQUAL(p[0].x(), 2);
	   ASSERT_FLOAT_EQUAL(p[0].y(), 2);
   }

   void testFiveNonEqual()
   {
	   hst::vector<p2d::Point> p;
	   p << p2d::Point(2,2);
	   p << p2d::Point(1,2);
	   p << p2d::Point(3,4);
	   p << p2d::Point(5,6);
	   p << p2d::Point(7,8);
	   p2d::erase_equal_points(p);
	   ASSERT_EQUAL(p.size(), 5);
	   ASSERT_FLOAT_EQUAL(p[0].x(), 2);
	   ASSERT_FLOAT_EQUAL(p[0].y(), 2);
	   ASSERT_FLOAT_EQUAL(p[1].x(), 1);
	   ASSERT_FLOAT_EQUAL(p[1].y(), 2);
	   ASSERT_FLOAT_EQUAL(p[2].x(), 3);
	   ASSERT_FLOAT_EQUAL(p[2].y(), 4);
	   ASSERT_FLOAT_EQUAL(p[3].x(), 5);
	   ASSERT_FLOAT_EQUAL(p[3].y(), 6);
	   ASSERT_FLOAT_EQUAL(p[4].x(), 7);
	   ASSERT_FLOAT_EQUAL(p[4].y(), 8);
   }

   void testComplex()
   {
	   hst::vector<p2d::Point> p;
	   p << p2d::Point(2,2);
	   p << p2d::Point(2,2);
	   p << p2d::Point(3,4);
	   p << p2d::Point(7,7);
	   p << p2d::Point(7,7);
	   p2d::erase_equal_points(p);
	   ASSERT_EQUAL(p.size(), 3);
	   ASSERT_FLOAT_EQUAL(p[0].x(), 2);
	   ASSERT_FLOAT_EQUAL(p[0].y(), 2);
	   ASSERT_FLOAT_EQUAL(p[1].x(), 3);
	   ASSERT_FLOAT_EQUAL(p[1].y(), 4);
	   ASSERT_FLOAT_EQUAL(p[2].x(), 7);
	   ASSERT_FLOAT_EQUAL(p[2].y(), 7);
   }

} test5;