#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include <3rdparty/tpunit++/tpunit++.hpp>
#include <sadvector.h>
#include <p2d/findcontactpoints.h>
#include <geometry2d.h>
#pragma warning(pop)


/*!
 * Tests finding contact points between two rectangles
 */
struct TestFindContactPointsRtoL : tpunit::TestFixture
{
 public:
   TestFindContactPointsRtoL() : tpunit::TestFixture(
       TEST(TestFindContactPointsRtoL::test1),
       TEST(TestFindContactPointsRtoL::test2),
       TEST(TestFindContactPointsRtoL::test3),
       TEST(TestFindContactPointsRtoL::test4),
       TEST(TestFindContactPointsRtoL::test5),
       TEST(TestFindContactPointsRtoL::test6),
       TEST(TestFindContactPointsRtoL::test7),
       TEST(TestFindContactPointsRtoL::test8),
       TEST(TestFindContactPointsRtoL::test9),
       TEST(TestFindContactPointsRtoL::test10),
       TEST(TestFindContactPointsRtoL::test11),
       TEST(TestFindContactPointsRtoL::test12),
       TEST(TestFindContactPointsRtoL::test13),
       TEST(TestFindContactPointsRtoL::test14),
       TEST(TestFindContactPointsRtoL::test15),
       TEST(TestFindContactPointsRtoL::test16),
       TEST(TestFindContactPointsRtoL::test17),
       TEST(TestFindContactPointsRtoL::test18)
    ) {}

   void test1()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 1), sad::Point2D(1, 2),
                   sad::Point2D(2, 1), sad::Point2D(1, 0))	  
       );
       r2.setCutter(sad::p2d::cutter(3, 0, 3, 2));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 2, 1, 3, 1));
   }

   void test2()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(2, 1), sad::Point2D(3, 2),
                   sad::Point2D(4, 1), sad::Point2D(3, 0))	  
       );
       r2.setCutter(sad::p2d::cutter(3, 0, 3, 2));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 4, 1, 3, 1));
   }

   void test3()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(4, 1), sad::Point2D(5, 2),
                   sad::Point2D(6, 1), sad::Point2D(5, 0))	  
       );
       r2.setCutter(sad::p2d::cutter(3, 0, 3, 2));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 6, 1, 3, 1));
   }

   void test4()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(6, 1), sad::Point2D(7, 2),
                   sad::Point2D(8, 1), sad::Point2D(7, 0))	  
       );
       r2.setCutter(sad::p2d::cutter(3, 0, 3, 2));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 8, 1, 3, 1));
   }

   void test5()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 4), sad::Point2D(1, 5),
                   sad::Point2D(2, 4), sad::Point2D(1, 3))	  
       );
       r2.setCutter(sad::p2d::cutter(0, 2, 2, 2));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 1, 3, 1, 2));
   }

   void test6()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 2), sad::Point2D(1, 3),
                   sad::Point2D(2, 2), sad::Point2D(1, 1))	  
       );
       r2.setCutter(sad::p2d::cutter(0, 2, 2, 2));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 1, 1, 1, 2));
   }

   void test7()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 0), sad::Point2D(1, 1),
                   sad::Point2D(2, 0), sad::Point2D(1, -1))	  
       );
       r2.setCutter(sad::p2d::cutter(0, 2, 2, 2));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 1, -1, 1, 2));
   }

   void test8()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, -2), sad::Point2D(1, -1),
                   sad::Point2D(2, -2), sad::Point2D(1, -3))	  
       );
       r2.setCutter(sad::p2d::cutter(0, 2, 2, 2));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 1, -3, 1, 2));
   }

   void test9()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 0), sad::Point2D(0, 2),
                   sad::Point2D(2, 2), sad::Point2D(2, 0))	  
       );
       r2.setCutter(sad::p2d::cutter(4, 0 , 4 , 2));	   
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 2);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 2, 2, 4, 2));
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 2, 0, 4, 0));
   }

   void test10()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(3, 0), sad::Point2D(3, 2),
                   sad::Point2D(5, 2), sad::Point2D(5, 0))	  
       );
       r2.setCutter(sad::p2d::cutter(4, 0 , 4 , 2));	   
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 2);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 5, 2, 4, 2));
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 5, 0, 4, 0));
   }

   void test11()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(5, 0), sad::Point2D(5, 2),
                   sad::Point2D(7, 2), sad::Point2D(7, 0))	  
       );
       r2.setCutter(sad::p2d::cutter(4, 0 , 4 , 2));	   
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 2);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 7, 2, 4, 2));
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 7, 0, 4, 0));
   }

   void test12()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(7, 0), sad::Point2D(7, 2),
                   sad::Point2D(9, 2), sad::Point2D(9, 0))	  
       );
       r2.setCutter(sad::p2d::cutter(4, 0 , 4 , 2));	   
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 2);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 9, 2, 4, 2));
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 9, 0, 4, 0));
   }

   
   void test13()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 0), sad::Point2D(0, 2),
                   sad::Point2D(2, 2), sad::Point2D(2, 0))	  
       );
       r2.setCutter(sad::p2d::cutter(4, -1, 4, 1));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 2);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 2, 0, 4, 0));
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 2, 1, 4, 1));
   }

   void test14()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 0), sad::Point2D(0, 2),
                   sad::Point2D(2, 2), sad::Point2D(2, 0))	  
       );
       r2.setCutter(sad::p2d::cutter(1, -1, 1, 1));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 2);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 2, 1, 1, 1));
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 2, 0, 1, 0));
   }

   void test15()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r2.setCutter(sad::p2d::cutter(0, 0, 0, 2));
       r1.setRect(
            sad::Rect2D(sad::Point2D(4, -1), sad::Point2D(4, 1),
                   sad::Point2D(6, 1), sad::Point2D(6, -1))
       );
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 2);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 6, 0, 0, 0));
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 6, 1, 0, 1));
   }


   void test16()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r2.setCutter(sad::p2d::cutter(0, 0, 0, 2));
       r1.setRect(
            sad::Rect2D(sad::Point2D(1, -1), sad::Point2D(1, 1),
                   sad::Point2D(3, 1), sad::Point2D(3, -1))
       );
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 2);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 3, 1, 0, 1));
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 3, 0, 0, 0));
   }

   void test17()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r2.setCutter(sad::p2d::cutter(0, 0, 0, 2));
       r1.setRect(
            sad::Rect2D(sad::Point2D(4, 0), sad::Point2D(4, 2),
                   sad::Point2D(6, 2), sad::Point2D(6, 0))
       );
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 0);
    }

   void test18()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, 0);
       sad::p2d::Vector v2(0, 0);
       r2.setCutter(sad::p2d::cutter(0, 0, 0, 2));
       r1.setRect(
            sad::Rect2D(sad::Point2D(4, 0), sad::Point2D(4, 2),
                   sad::Point2D(6, 2), sad::Point2D(6, 0))
       );
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 0);
    }


} test_find_contact_points_rtol;
