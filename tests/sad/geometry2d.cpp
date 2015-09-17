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
       TEST(Geometry2DTest::testGetBaseRect),
       TEST(Geometry2DTest::testRotate1)
    ) {}

   void testGetBaseRect()
   {
       sad::Rect2D r(
           sad::Point2D(0, 0),
           sad::Point2D(640, 0),
           sad::Point2D(640,480),
           sad::Point2D(0, 480)
       );

       sad::Rect2D target;
       sad::Rect2D baserect;
       sad::Rect2D testrect;
       double alpha = 0;

       for (double _alpha = 0; _alpha < M_PI * 2.0 ; _alpha += 0.1)
       {
            target = r;
            sad::rotate(target, (float)_alpha);
            sad::getBaseRect(target, baserect, alpha);
            testrect = baserect;
            sad::rotate(testrect, (float)alpha);
            ASSERT_TRUE( sad::equal(target, testrect) );	
       }
   }

   void testRotate1()
   {
       sad::Rect2D r(
           sad::Point2D(0, 0),
           sad::Point2D(2, 0),
           sad::Point2D(2, 2),
           sad::Point2D(0, 2)
       );

       sad::rotate(r,  (float)(M_PI / 2.0));

       ASSERT_TRUE(sad::equal(r[0], sad::Point2D(2, 0)));
       ASSERT_TRUE(sad::equal(r[1], sad::Point2D(2, 2)));
       ASSERT_TRUE(sad::equal(r[2], sad::Point2D(0, 2)));
       ASSERT_TRUE(sad::equal(r[3], sad::Point2D(0, 0)));
   }

} _geometry2d_test; 