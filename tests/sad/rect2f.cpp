#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <primitives/hrect.h>
#include <primitives/hpoint.h>
#include <extra/geometry2d.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "sadptrhash.h"
#include "sadstring.h"
#pragma warning(pop)



/*!
 * Tests object logic
 */
struct HRectTest : tpunit::TestFixture
{
 public:
   HRectTest() : tpunit::TestFixture(
	   TEST(HRectTest::test1),
	   TEST(HRectTest::test2)
   ) {}
   
   void test1()
   {
		hRectF r(hPointF(-12,-12), hPointF(12, 12));
		ASSERT_TRUE(sad::equal(r[0], hPointF(-12, -12)));
		ASSERT_TRUE(sad::equal(r[1], hPointF(12, -12)));
		ASSERT_TRUE(sad::equal(r[2], hPointF(12, 12)));
		ASSERT_TRUE(sad::equal(r[3], hPointF(-12, 12)));
   }

   void test2()
   {
		hRectF r(hPointF(-24,-6), hPointF(25, 7));
		ASSERT_TRUE(sad::equal(r[0], hPointF(-24, -6)));
		ASSERT_TRUE(sad::equal(r[1], hPointF(25, -6)));
		ASSERT_TRUE(sad::equal(r[2], hPointF(25, 7)));
		ASSERT_TRUE(sad::equal(r[3], hPointF(-24, 7)));
   }



} test_rect2f;
