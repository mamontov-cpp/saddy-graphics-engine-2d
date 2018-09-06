#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include <3rdparty/tpunit++/tpunit++.hpp>
#include <sadvector.h>
#include <p2d/point.h>
#pragma warning(pop)



namespace sad
{

namespace p2d
{
void erase_equal_points(sad::Vector<sad::p2d::Point> & set);
}

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
       sad::Vector<sad::p2d::Point> p;
       sad::p2d::erase_equal_points(p);
       ASSERT_EQUAL(p.size(), 0);
   }

   void testOne()
   {
       sad::Vector<sad::p2d::Point> p;
       p << sad::p2d::Point(2,2);
       sad::p2d::erase_equal_points(p);
       ASSERT_EQUAL(p.size(), 1);
       ASSERT_FLOAT_EQUAL(p[0].x(), 2);
       ASSERT_FLOAT_EQUAL(p[0].y(), 2);
   }

   void testTwo()
   {
       sad::Vector<sad::p2d::Point> p;
       p << sad::p2d::Point(2,2);
       p << sad::p2d::Point(3,3);
       sad::p2d::erase_equal_points(p);
       ASSERT_EQUAL(p.size(), 2);
       ASSERT_FLOAT_EQUAL(p[0].x(), 2);
       ASSERT_FLOAT_EQUAL(p[0].y(), 2);
       ASSERT_FLOAT_EQUAL(p[1].x(), 3);
       ASSERT_FLOAT_EQUAL(p[1].y(), 3);
   }

   void testTwoEqual()
   {
       sad::Vector<sad::p2d::Point> p;
       p << sad::p2d::Point(2,2);
       p << sad::p2d::Point(2,2);
       sad::p2d::erase_equal_points(p);
       ASSERT_EQUAL(p.size(), 1);
       ASSERT_FLOAT_EQUAL(p[0].x(), 2);
       ASSERT_FLOAT_EQUAL(p[0].y(), 2);
   }

   void testFiveNonEqual()
   {
       sad::Vector<sad::p2d::Point> p;
       p << sad::p2d::Point(2,2);
       p << sad::p2d::Point(1,2);
       p << sad::p2d::Point(3,4);
       p << sad::p2d::Point(5,6);
       p << sad::p2d::Point(7,8);
       sad::p2d::erase_equal_points(p);
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
       sad::Vector<sad::p2d::Point> p;
       p << sad::p2d::Point(2,2);
       p << sad::p2d::Point(2,2);
       p << sad::p2d::Point(3,4);
       p << sad::p2d::Point(7,7);
       p << sad::p2d::Point(7,7);
       sad::p2d::erase_equal_points(p);
       ASSERT_EQUAL(p.size(), 3);
       ASSERT_FLOAT_EQUAL(p[0].x(), 2);
       ASSERT_FLOAT_EQUAL(p[0].y(), 2);
       ASSERT_FLOAT_EQUAL(p[1].x(), 3);
       ASSERT_FLOAT_EQUAL(p[1].y(), 4);
       ASSERT_FLOAT_EQUAL(p[2].x(), 7);
       ASSERT_FLOAT_EQUAL(p[2].y(), 7);
   }

} _test_graham_erase_equal;