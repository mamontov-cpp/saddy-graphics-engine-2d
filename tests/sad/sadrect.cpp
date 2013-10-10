#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <sadrect.h>
#include <sadpoint.h>
#include <extra/geometry2d.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "sadptrhash.h"
#include "sadstring.h"
#pragma warning(pop)



/*!
 * Tests object logic
 */
struct sadRect2DTest : tpunit::TestFixture
{
 public:
   sadRect2DTest() : tpunit::TestFixture(
	   TEST(sadRect2DTest::test1),
	   TEST(sadRect2DTest::test2)
   ) {}
   
   void test1()
   {
		sad::Rect2D r(sad::Point2D(-12,-12), sad::Point2D(12, 12));
		ASSERT_TRUE(sad::equal(r[0], sad::Point2D(-12, -12)));
		ASSERT_TRUE(sad::equal(r[1], sad::Point2D(12, -12)));
		ASSERT_TRUE(sad::equal(r[2], sad::Point2D(12, 12)));
		ASSERT_TRUE(sad::equal(r[3], sad::Point2D(-12, 12)));
   }

   void test2()
   {
		sad::Rect2D r(sad::Point2D(-24,-6), sad::Point2D(25, 7));
		ASSERT_TRUE(sad::equal(r[0], sad::Point2D(-24, -6)));
		ASSERT_TRUE(sad::equal(r[1], sad::Point2D(25, -6)));
		ASSERT_TRUE(sad::equal(r[2], sad::Point2D(25, 7)));
		ASSERT_TRUE(sad::equal(r[3], sad::Point2D(-24, 7)));
   }



} test_sad_rect_2d;
