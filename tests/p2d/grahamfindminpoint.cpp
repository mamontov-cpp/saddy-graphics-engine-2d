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
int find_min_point_on_y_axis(const sad::Vector<sad::p2d::Point> & set);
}
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
       sad::Vector<sad::p2d::Point> p;
       int i = sad::p2d::find_min_point_on_y_axis(p);
       ASSERT_EQUAL(i, -1);
   }

   void testOne()
   {
       sad::Vector<sad::p2d::Point> p;
       p << sad::p2d::Point(2,2);
       int i = sad::p2d::find_min_point_on_y_axis(p);
       ASSERT_EQUAL(i, 0);
   }

   void testTwoSequential()
   {
       sad::Vector<sad::p2d::Point> p;
       p << sad::p2d::Point(2,2);
       p << sad::p2d::Point(3,3);
       int i = sad::p2d::find_min_point_on_y_axis(p);
       ASSERT_EQUAL(i, 0);
   }

   void testTwoReversal()
   {
       sad::Vector<sad::p2d::Point> p;
       p << sad::p2d::Point(3,3);
       p << sad::p2d::Point(2,2);
       int i = sad::p2d::find_min_point_on_y_axis(p);
       ASSERT_EQUAL(i, 1);
   }

   void testThree()
   {
       sad::Vector<sad::p2d::Point> p;
       p << sad::p2d::Point(3,4);
       p << sad::p2d::Point(1,2);
       p << sad::p2d::Point(5,6);
       int i = sad::p2d::find_min_point_on_y_axis(p);
       ASSERT_EQUAL(i, 1);
   }

} test5;