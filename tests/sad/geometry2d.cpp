#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#define _INC_STDIO
#include "geometry3d.h"
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
	   TEST(Geometry2DTest::testGetBaseRect2),
	   TEST(Geometry2DTest::testGetBaseRect3),
	   TEST(Geometry2DTest::testGetBaseRect4),
	   TEST(Geometry2DTest::testGetBaseRect5),
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

   // Bug-reproducing case, inferred from testGetBaseRect
   void testGetBaseRect2()
   {
		sad::Rect< sad::Point3D> r(
			sad::Point3D(0, 0, 0),
			sad::Point3D(640, 0, 0),
			sad::Point3D(640,480, 0),
			sad::Point3D(0, 480, 0)
		);

		sad::Rect< sad::Point3D> br;

		double _alpha = 0.5;
		double _theta = 0.0;

		double alpha = 0;
		double theta = 0;
		sad::rotate(r, r, _alpha, _theta);		
		sad::getBaseRect(r, br, alpha, theta);
		ASSERT_TRUE( sad::is_fuzzy_equal(alpha, _alpha) );	
		ASSERT_TRUE( sad::is_fuzzy_equal(theta, _theta) );
   }

   void testGetBaseRect3()
   {
		sad::Rect< sad::Point3D> r(
			sad::Point3D(0, 0, 0),
			sad::Point3D(640, 0, 0),
			sad::Point3D(640,480, 0),
			sad::Point3D(0, 480, 0)
		);

		sad::Rect< sad::Point3D> br;

		sad::Point3D pivot(320, 240, 1.0);
		double _alpha = 0.5;
		double _theta = 0.5;

		double alpha = 0;
		double theta = 0;
		sad::rotate(r, r, _alpha, _theta);
		sad::getBaseRect(r, br, alpha, theta);
		ASSERT_TRUE( sad::is_fuzzy_equal(alpha, _alpha) );	
		ASSERT_TRUE( sad::is_fuzzy_equal(theta, _theta) );	   
   }

   void testGetBaseRect4()
   {
		sad::Rect< sad::Point3D> r(
			sad::Point3D(0, 0, 0),
			sad::Point3D(640, 0, 0),
			sad::Point3D(640,480, 0),
			sad::Point3D(0, 480, 0)
		);

		sad::Rect< sad::Point3D> br;

		sad::Point3D pivot(320, 240, 1.0);
		double _alpha = 0.0;
		double _theta = 2.0;

		double alpha = 0;
		double theta = 0;
		sad::rotate(r, r, _alpha, _theta);
		sad::getBaseRect(r, br, alpha, theta);
		ASSERT_TRUE( sad::is_fuzzy_equal(alpha, _alpha) );	
		ASSERT_TRUE( sad::is_fuzzy_equal(theta, _theta) );		   
   }

   /*! Some not cool bug taken from IFace Editor. 
    */
   void testGetBaseRect5()
   {
		sad::Rect< sad::Point3D> r(
			sad::Point3D(346.74905395507812, 88.210494995117187, 0), 
			sad::Point3D(576.20172119140625,281.47579956054687,  0),
			sad::Point3D(366.83096313476562, 530.04949951171875, 0),
			sad::Point3D(137.37829589843750, 336.78421020507812, 0)
		);

		sad::Rect< sad::Point3D> br;

		double alpha = 0;
		double theta = 0;
		sad::getBaseRect(r, br, alpha, theta);
		ASSERT_FALSE (sad::equal(r, br) );	   
		sad::rotate(br, br, alpha, theta);
		ASSERT_TRUE (sad::equal(r, br, 0.001) );	   
   }

   void testGetBaseRect()
   {
	   sad::Rect< sad::Point3D> r(
		   sad::Point3D(0, 0, 0),
		   sad::Point3D(640, 0, 0),
		   sad::Point3D(640,480, 0),
		   sad::Point3D(0, 480, 0)
	   );

	   sad::Rect< sad::Point3D> target;
	   sad::Rect< sad::Point3D> baserect;
	   sad::Rect< sad::Point3D> testrect;
	   double alpha = 0;
	   double theta = 0;

	   for (double _alpha = 0; _alpha < M_PI * 2.0 ; _alpha += 0.1)
	   {
		   for(double _theta = 0; _theta < M_PI * 2.0 ; _theta +=0.1)
		   {
				sad::rotate(r, target, _alpha, _theta);
				sad::getBaseRect(target, baserect, alpha, theta);
				sad::rotate(baserect, testrect, alpha, theta);
				ASSERT_TRUE( sad::equal(target, testrect) );	

		   }
	   }
   }

} _geometry2d_test; 