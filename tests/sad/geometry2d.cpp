#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdio>
#define _INC_STDIO  // NOLINT(clang-diagnostic-reserved-id-macro)
#include "geometry2d.h"
// ReSharper disable once CppUnusedIncludeDirective
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
       TEST(Geometry2DTest::testRotate1),
       TEST(Geometry2DTest::testEllipticProperties1),
       TEST(Geometry2DTest::testEllipticProperties2),
       TEST(Geometry2DTest::testEllipticProperties3),
       TEST(Geometry2DTest::testEllipticProperties4)
    ) {}

   // ReSharper disable once CppMemberFunctionMayBeConst
   void testGetBaseRect()
   {
       const sad::Rect2D r(
           sad::Point2D(0, 0),
           sad::Point2D(640, 0),
           sad::Point2D(640,480),
           sad::Point2D(0, 480)
       );

       sad::Rect2D base_rect;
       double alpha = 0;

       for (double _alpha = 0; _alpha < M_PI * 2.0 ; _alpha += 0.1)  // NOLINT(cert-flp30-c)
       {
            sad::Rect2D target = r;
            sad::rotate(target, static_cast<float>(_alpha));
            sad::getBaseRect(target, base_rect, alpha);
            sad::Rect2D test_rect = base_rect;
            sad::rotate(test_rect, static_cast<float>(alpha));
            ASSERT_TRUE( sad::equal(target, test_rect) )
       }
   }
   // ReSharper disable once CppMemberFunctionMayBeConst
   void testRotate1()
   {
       sad::Rect2D r(
           sad::Point2D(0, 0),
           sad::Point2D(2, 0),
           sad::Point2D(2, 2),
           sad::Point2D(0, 2)
       );

       sad::rotate(r,  static_cast<float>((M_PI / 2.0)));

       ASSERT_TRUE(sad::equal(r[0], sad::Point2D(2, 0)))
       ASSERT_TRUE(sad::equal(r[1], sad::Point2D(2, 2)))
       ASSERT_TRUE(sad::equal(r[2], sad::Point2D(0, 2)))
       ASSERT_TRUE(sad::equal(r[3], sad::Point2D(0, 0)))
   }
   // ReSharper disable once CppMemberFunctionMayBeConst
   // ReSharper disable once CppMemberFunctionMayBeStatic
   void testEllipticProperties1()
   {
       {
            const sad::Point2D c(0, 0), p1(1, 1), p2(3, 0);

            const sad::EllipticMovementProperties props = sad::computeEllipticProperties(c, p1, p2);

            const sad::Point2D pp1 = props.compute(0);
            const double dist1 = pp1.distance(p1);
            ASSERT_TRUE(sad::is_fuzzy_zero(dist1))

            const sad::Point2D pp2 = props.compute(props.phi );
            const double dist2 = pp2.distance(p2);
            ASSERT_TRUE(sad::is_fuzzy_zero(dist2))


            const sad::Point2D pp3 = props.compute(props.phi / 2.0);
            const sad::Point2D p3(2.1647844005847880, 0.54119610014619701);
            const double dist3 = pp3.distance(p3);
            ASSERT_TRUE(sad::is_fuzzy_zero(dist3))
       }
   }
   // ReSharper disable once CppMemberFunctionMayBeConst
   // ReSharper disable once CppMemberFunctionMayBeStatic
    void testEllipticProperties2()
    {
        {
            const sad::Point2D c(0, 0), p1(0, 0), p2(0, 0);

            const sad::EllipticMovementProperties props = sad::computeEllipticProperties(c, p1, p2);

            const sad::Point2D pp1 = props.compute(0);
            const double dist1 = pp1.distance(p1);
            ASSERT_TRUE(sad::is_fuzzy_zero(dist1))

            const sad::Point2D pp2 = props.compute(props.phi);
            const double dist2 = pp2.distance(p2);
            ASSERT_TRUE(sad::is_fuzzy_zero(dist2))


            const sad::Point2D pp3 = props.compute(props.phi / 2.0);
            const sad::Point2D p3(0, 0);
            const double dist3 = pp3.distance(p3);
            ASSERT_TRUE(sad::is_fuzzy_zero(dist3))
        }
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    // ReSharper disable once CppMemberFunctionMayBeStatic
    void testEllipticProperties3()
    {
        {
            const sad::Point2D c(0, 0), p1(0, 0), p2(3, 0);

            const sad::EllipticMovementProperties props = sad::computeEllipticProperties(c, p1, p2);

            const sad::Point2D pp1 = props.compute(0);
            const double dist1 = pp1.distance(p1);
            ASSERT_TRUE(sad::is_fuzzy_zero(dist1))

            const sad::Point2D pp2 = props.compute(props.phi);
            const double dist2 = pp2.distance(p2);
            ASSERT_TRUE(sad::is_fuzzy_zero(dist2))


            const sad::Point2D pp3 = props.compute(props.phi / 2.0);
            const sad::Point2D p3(2.1213203435596424, 0);
            const double dist3 = pp3.distance(p3);
            ASSERT_TRUE(sad::is_fuzzy_zero(dist3))
        }
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    // ReSharper disable once CppMemberFunctionMayBeStatic
    void testEllipticProperties4()
    {
        {
            const sad::Point2D c(0, 0), p1(3, 3), p2(0, 0);

            const sad::EllipticMovementProperties props = sad::computeEllipticProperties(c, p1, p2);

            const sad::Point2D pp1 = props.compute(0);
            const double dist1 = pp1.distance(p1);
            ASSERT_TRUE(sad::is_fuzzy_zero(dist1))

            const sad::Point2D pp2 = props.compute(props.phi);
            const double dist2 = pp2.distance(p2);
            ASSERT_TRUE(sad::is_fuzzy_zero(dist2))
        }
    }
    // ReSharper disable once CppMemberFunctionMayBeConst
    // ReSharper disable once CppMemberFunctionMayBeStatic
    void testEllipticProperties5()
    {
        {
            const sad::Point2D c(0, 0), p1(3, 3), p2(4 , 4);

            try 
            {
                sad::computeEllipticProperties(c, p1, p2);
                ASSERT_TRUE(false)
            }
            catch (...)
            {
                ASSERT_TRUE(true)
            }
        }
    }


} _geometry2d_test; 