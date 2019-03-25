#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include <3rdparty/tpunit++/tpunit++.hpp>
#include <sadvector.h>
#include <p2d/convexhull.h>
#include <geometry2d.h>
#pragma warning(pop)



/*!
 * Tests convex hull test
 */
struct ConvexHullTest : tpunit::TestFixture
{
 public:
   ConvexHullTest() : tpunit::TestFixture(
       TEST(ConvexHullTest::testNormalsOfTriangle),
       TEST(ConvexHullTest::testNormalsOfSquare),
       TEST(ConvexHullTest::testHardColllides),
       TEST(ConvexHullTest::testCollidingRectangles),
       TEST(ConvexHullTest::testLineTouchesRectangle),
       TEST(ConvexHullTest::testPointOnRectangleSide),
       TEST(ConvexHullTest::testNearestNormals),
       TEST(ConvexHullTest::testQuadrangle1),
       TEST(ConvexHullTest::testSixSides1)
   ) {}
   
   void testNormalsOfTriangle()
   {
       sad::Vector<sad::p2d::Point> set;
       set << sad::p2d::Point(0,0);
       set << sad::p2d::Point(4,0);
       set << sad::p2d::Point(4,4);
       sad::p2d::ConvexHull h = sad::p2d::ConvexHull::uncheckedCreate(set);
       {
           sad::p2d::Vector normal = h.normal(0);
           ASSERT_TRUE( sad::equal(normal, sad::p2d::Point(0, -1) )  );
       }
       {
           sad::p2d::Vector normal = h.normal(1);
           ASSERT_TRUE( sad::equal(normal, sad::p2d::Point(1, 0) )  );
       }
       {
           sad::p2d::Vector normal = h.normal(2);
           ASSERT_TRUE( sad::equal(normal, sad::p2d::Point(-M_SQRT1_2, M_SQRT1_2) )  );
       }
   }

   void testNormalsOfSquare()
   {
       sad::Vector<sad::p2d::Point> set;
       set << sad::p2d::Point(0,0);
       set << sad::p2d::Point(4,0);
       set << sad::p2d::Point(4,4);
       set << sad::p2d::Point(0,4);
       sad::p2d::ConvexHull h = sad::p2d::ConvexHull::uncheckedCreate(set);
       {
           sad::p2d::Vector normal = h.normal(0);
           ASSERT_TRUE( sad::equal(normal, sad::p2d::Point(0, -1) )  );
       }
       {
           sad::p2d::Vector normal = h.normal(1);
           ASSERT_TRUE( sad::equal(normal, sad::p2d::Point(1, 0) )  );
       }
       {
           sad::p2d::Vector normal = h.normal(2);
           ASSERT_TRUE( sad::equal(normal, sad::p2d::Point(0, 1) )  );
       }
       {
           sad::p2d::Vector normal = h.normal(3);
           ASSERT_TRUE( sad::equal(normal, sad::p2d::Point(-1, 0) )  );
       }
   }

   void testHardColllides()
   {
       sad::Vector<sad::p2d::Point> set1;
       set1 << sad::p2d::Point(0, 0);
       set1 << sad::p2d::Point(1, 3);
       set1 << sad::p2d::Point(4, 3);
       set1 << sad::p2d::Point(3, 0);
       sad::p2d::ConvexHull hull1(set1);

       sad::Vector<sad::p2d::Point> set2;
       set2 << sad::p2d::Point(3.5, 0);
       set2 << sad::p2d::Point(5, 2);
       set2 << sad::p2d::Point(7, 0);
       set2 << sad::p2d::Point(5, -2);
       sad::p2d::ConvexHull hull2(set2);

       ASSERT_FALSE( hull1.collides(hull2) );
       ASSERT_FALSE( hull2.collides(hull1) );
   }

   void testCollidingRectangles()
   {
       sad::Vector<sad::p2d::Point> set1;
       set1 << sad::p2d::Point(0, 2);
       set1 << sad::p2d::Point(0, 5);
       set1 << sad::p2d::Point(3, 5);
       set1 << sad::p2d::Point(3, 2);
       sad::p2d::ConvexHull hull1(set1);

       sad::Vector<sad::p2d::Point> set2;
       set2 << sad::p2d::Point(0, 3);
       set2 << sad::p2d::Point(2, 5);
       set2 << sad::p2d::Point(5, 2);
       set2 << sad::p2d::Point(3, 0);
       sad::p2d::ConvexHull hull2(set2);

       ASSERT_TRUE( hull1.collides(hull2) );
       ASSERT_TRUE( hull2.collides(hull1) );
   }

   void testLineTouchesRectangle()
   {
       sad::Vector<sad::p2d::Point> set1;
       set1 << sad::p2d::Point(0, 0);
       set1 << sad::p2d::Point(0, 6);
       sad::p2d::ConvexHull hull1(set1);

       sad::Vector<sad::p2d::Point> set2;
       set2 << sad::p2d::Point(0, 3);
       set2 << sad::p2d::Point(0, 6);
       set2 << sad::p2d::Point(3, 6);
       set2 << sad::p2d::Point(3, 3);
       sad::p2d::ConvexHull hull2(set2);

       ASSERT_TRUE( hull1.collides(hull2) );
       ASSERT_TRUE( hull2.collides(hull1) );
   }

   void testPointOnRectangleSide()
   {
       sad::Vector<sad::p2d::Point> set1;
       set1 << sad::p2d::Point(0, 2);
       sad::p2d::ConvexHull hull1(set1);

       sad::Vector<sad::p2d::Point> set2;
       set2 << sad::p2d::Point(0, 0);
       set2 << sad::p2d::Point(0, 3);
       set2 << sad::p2d::Point(3, 3);
       set2 << sad::p2d::Point(3, 0);
       sad::p2d::ConvexHull hull2(set2);

       ASSERT_TRUE( hull1.collides(hull2) );
       ASSERT_TRUE( hull2.collides(hull1) );
   }

   void testNearestNormals()
   {
       sad::Vector<sad::p2d::Point> set;
       set << sad::p2d::Point(0, 0);
       set << sad::p2d::Point(2, 0);
       set << sad::p2d::Point(2, 2);
       set << sad::p2d::Point(0, 2);
       sad::p2d::ConvexHull hull(set);
       ASSERT_TRUE(
                sad::equal(
                    hull.getSumOfNormalsFor(sad::p2d::Point(-1, 1)),
                    sad::p2d::Vector(-1, 0)
                )	
           );
       ASSERT_TRUE(
                sad::equal(
                    hull.getSumOfNormalsFor(sad::p2d::Point(-2, 4)),
                    sad::p2d::Vector(-M_SQRT1_2, M_SQRT1_2)
                )	
           );
       sad::p2d::Vector v = hull.getSumOfNormalsFor(sad::p2d::Point(1, 4));
       ASSERT_TRUE(
                sad::equal(
                    v,
                    sad::p2d::Vector(0, 1)
                )	
           );
       ASSERT_TRUE(
                sad::equal(
                    hull.getSumOfNormalsFor(sad::p2d::Point(4, 4)),
                    sad::p2d::Vector(M_SQRT1_2, M_SQRT1_2)
                )	
           );
       sad::p2d::Vector v41 = hull.getSumOfNormalsFor(sad::p2d::Point(4, 1));
       ASSERT_TRUE(
                sad::equal(
                    v41,
                    sad::p2d::Vector(1, 0)
                )	
           );
       ASSERT_TRUE(
                sad::equal(
                    hull.getSumOfNormalsFor(sad::p2d::Point(4, -2)),
                    sad::p2d::Vector(M_SQRT1_2, -M_SQRT1_2)
                )	
           );
       ASSERT_TRUE(
                sad::equal(
                    hull.getSumOfNormalsFor(sad::p2d::Point(1, -2)),
                    sad::p2d::Vector(0, -1)
                )	
           );
       ASSERT_TRUE(
                sad::equal(
                    hull.getSumOfNormalsFor(sad::p2d::Point(-2, -2)),
                    sad::p2d::Vector(-M_SQRT1_2, -M_SQRT1_2)
                )	
           );
   }

   void testQuadrangle1()
   {
       sad::Vector<sad::p2d::Point> set;
       set << sad::p2d::Point(0, 0);
       set << sad::p2d::Point(4, 4);
       set << sad::p2d::Point(8, 4);
       set << sad::p2d::Point(4, 0);
       sad::p2d::ConvexHull hull(set);
       sad::p2d::Point center = hull.center();
       ASSERT_TRUE( sad::equal(center, sad::p2d::Point(4,2)) );
   }

   void testSixSides1()
   {
       sad::Vector<sad::p2d::Point> set;
       set << sad::p2d::Point(2, 0);
       set << sad::p2d::Point(4, 0);
       set << sad::p2d::Point(6, 2);
       set << sad::p2d::Point(4, 4);
       set << sad::p2d::Point(2, 4);
       set << sad::p2d::Point(0, 2);
       sad::p2d::ConvexHull hull(set);
       sad::p2d::Point center = hull.center();
       ASSERT_TRUE( sad::equal(center, sad::p2d::Point(3,2)) );
   }

} _convex_hull_test;