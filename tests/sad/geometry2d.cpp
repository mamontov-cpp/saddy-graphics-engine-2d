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
	   TEST(Geometry2DTest::testIsValid5),
	   TEST(Geometry2DTest::testGetBaseRect1),
	   TEST(Geometry2DTest::testGetBaseRect)
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

   void testGetBaseRect1()
   {
	   sad::Rect< sad::Point3D> r(
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(640, 0, 0),
		   sad::Point3D(640,480, 0),
		   sad::Point3D(0, 480, 0)
	   );

	   sad::Rect< sad::Point3D> br;
	   double alpha = 0;
	   double theta = 0;
	   sad::getBaseRect(r, br, alpha, theta);

	   ASSERT_TRUE( sad::is_fuzzy_equal(alpha, 0) );	
	   ASSERT_TRUE( sad::is_fuzzy_equal(theta, 0) );	
   }

   void testGetBaseRect()
   {
	   sad::Rect< sad::Point3D> r(
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(640, 0, 0),
		   sad::Point3D(640,480, 0),
		   sad::Point3D(0, 480, 0)
	   );

	   sad::Rect< sad::Point3D> dists(
		   sad::Point3D(-320.0, -240.0, 0),
		   sad::Point3D( 320.0, -240.0, 0),
		   sad::Point3D( 320.0,  240.0, 0),
		   sad::Point3D(-320.0,  240.0, 0)
	   );

	   sad::Rect< sad::Point3D> br;

	   sad::Point3D pivot(320, 240, 1.0);
	   for (double _alpha = 0; _alpha < M_PI; _alpha += 0.5)
	   {
		   for(double _theta = 0; _theta < M_PI; _theta +=0.5)
		   {
				double alpha = 0;
				double theta = 0;
				for(int k = 0; k < 4; k++)
				{
					sad::Point3D dist = dists[k];
					sad::Point3D result=dist;
				
					result.setX(dist.x() * cos(_alpha) - dist.y() * sin(_alpha));
					result.setY(dist.x() * sin(_alpha) * cos(_theta)
								+dist.y() * cos(_alpha) * cos(_theta)
								-dist.z() * sin(_theta));
					result.setZ(dist.x() * sin(_alpha) * sin(_theta)
								+dist.y() * cos(_alpha) * sin(_theta)
								+dist.z() * cos(_theta));
    
					r[k] = result + pivot;
				}
				sad::getBaseRect(r, br, alpha, theta);
				if (!sad::is_fuzzy_equal(alpha, _alpha) || !sad::is_fuzzy_equal(theta, _theta))
				{
					//theta *= 2;
				}
				ASSERT_TRUE( sad::is_fuzzy_equal(alpha, _alpha) );	
				ASSERT_TRUE( sad::is_fuzzy_equal(theta, _theta) );
		   }
	   }
   }

} _geometry2d_test; 