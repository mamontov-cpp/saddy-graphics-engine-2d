#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#define _INC_STDIO
#include "geometry2d.h"
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

/*!
 * Tests sad::Geometry2DTest
 */
struct Geometry2DTest : tpunit::TestFixture
{
 public:
   Geometry2DTest() : tpunit::TestFixture(
	   TEST(Geometry2DTest::testIsOnSamePlain1),
	   TEST(Geometry2DTest::testIsOnSamePlain2),
	   TEST(Geometry2DTest::testIsOnSamePlain3),
	   TEST(Geometry2DTest::testIsOnSamePlain4),
	   TEST(Geometry2DTest::testIsValid1),
	   TEST(Geometry2DTest::testIsValid2),
	   TEST(Geometry2DTest::testIsValid3),
	   TEST(Geometry2DTest::testIsValid4),
	   TEST(Geometry2DTest::testIsValid5)
   ) {}

   void testIsOnSamePlain1()
   {
	   sad::Rect< sad::Point3D> r(
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(0, 0, 0)
	   );
	   ASSERT_TRUE( sad::isOnSamePlane(r) );
   }

   void testIsOnSamePlain2()
   {
	   sad::Rect< sad::Point3D> r(
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(640, 0, 0),
		   sad::Point3D(640,480, 0),
		   sad::Point3D(0, 480, 0)
	   );
	   ASSERT_TRUE( sad::isOnSamePlane(r) );	   
   }

   void testIsOnSamePlain3()
   {
	   sad::Rect< sad::Point3D> r(
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(640, 0, 0),
		   sad::Point3D(640,480, 0),
		   sad::Point3D(0, 480, 1)
	   );
	   ASSERT_FALSE( sad::isOnSamePlane(r) );	   
   }

   void testIsOnSamePlain4()
   {
	   sad::Rect< sad::Point3D> r(
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(640, 0, 0),
		   sad::Point3D(640,480, 1),
		   sad::Point3D(0, 480, 1)
	   );
	   ASSERT_TRUE( sad::isOnSamePlane(r) );	   
   }

   void testIsValid1()
   {
	   sad::Rect< sad::Point3D> r(
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(0, 0, 0)
	   );
	   ASSERT_TRUE( sad::isValid(r) );
   }

   void testIsValid2()
   {
	   sad::Rect< sad::Point3D> r(
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(640, 0, 0),
		   sad::Point3D(640,480, 0),
		   sad::Point3D(0, 480, 0)
	   );
	   ASSERT_TRUE( sad::isValid(r) );	   
   }

   void testIsValid3()
   {
	   sad::Rect< sad::Point3D> r(
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(640, 0, 0),
		   sad::Point3D(640,480, 0),
		   sad::Point3D(0, 480, 1)
	   );
	   ASSERT_FALSE( sad::isValid(r) );	   
   }

   void testIsValid4()
   {
	   sad::Rect< sad::Point3D> r(
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(640, 0, 0),
		   sad::Point3D(640,480, 1),
		   sad::Point3D(0, 480, 1)
	   );
	   ASSERT_TRUE( sad::isValid(r) );	   
   }

   void testIsValid5()
   {
	   sad::Rect< sad::Point3D> r(
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(640, 0, 0),
		   sad::Point3D(640,480, 1),
		   sad::Point3D(0, 481, 1)
	   );
	   ASSERT_FALSE( sad::isValid(r) );	   
   }

} _geometry2d_test; 