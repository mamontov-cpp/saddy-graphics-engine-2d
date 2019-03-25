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
struct TestFindContactPointsLtoL : tpunit::TestFixture
{
 public:
   TestFindContactPointsLtoL() : tpunit::TestFixture(
       TEST(TestFindContactPointsLtoL::test1),
       TEST(TestFindContactPointsLtoL::test2),
       TEST(TestFindContactPointsLtoL::test3),
       TEST(TestFindContactPointsLtoL::test4),
       TEST(TestFindContactPointsLtoL::test5),
       TEST(TestFindContactPointsLtoL::test6),
       TEST(TestFindContactPointsLtoL::test7),
       TEST(TestFindContactPointsLtoL::test8),
       TEST(TestFindContactPointsLtoL::test9),
       TEST(TestFindContactPointsLtoL::test10),
       TEST(TestFindContactPointsLtoL::test11),
       TEST(TestFindContactPointsLtoL::test12),
       TEST(TestFindContactPointsLtoL::test13),
       TEST(TestFindContactPointsLtoL::test14),
       TEST(TestFindContactPointsLtoL::test15),
       TEST(TestFindContactPointsLtoL::test16),
       TEST(TestFindContactPointsLtoL::test17),
       TEST(TestFindContactPointsLtoL::test18)
    ) {}

   void test1()
   {
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter(sad::p2d::cutter(1, 2, 2, 1));
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter(sad::p2d::cutter(3, 2, 4, 1));
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter(sad::p2d::cutter(5, 2, 6, 1));	   
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);	   
       r1.setCutter(sad::p2d::cutter(7, 2, 8, 1));
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter(sad::p2d::cutter(2, 4, 1, 3));
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter(sad::p2d::cutter(2, 2, 1, 1));
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter(sad::p2d::cutter(2, 0, 1, -1));	   
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter(sad::p2d::cutter(2, -2, 1, -3));	   
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter(sad::p2d::cutter(2, 2 , 2 , 0));
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter(sad::p2d::cutter(5, 2 , 5 , 0));	   
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter(sad::p2d::cutter(7, 0 , 7 , 2));	 
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter(sad::p2d::cutter(9, 0 , 9 , 2));	 
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter(sad::p2d::cutter(2, 2, 2, 0));
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter(sad::p2d::cutter(2, 2, 2, 0));	   
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r2.setCutter(sad::p2d::cutter(0, 0, 0, 2));
       r1.setCutter(sad::p2d::cutter(6, 1, 6, -1));
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r2.setCutter(sad::p2d::cutter(0, 0, 0, 2));
       r1.setCutter(sad::p2d::cutter(3, 1, 3, -1));
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
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r2.setCutter(sad::p2d::cutter(0, 0, 0, 2));
       r1.setCutter(sad::p2d::cutter(6, 2, 6, 0));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 0);
    }

   void test18()
   {
       sad::p2d::Line r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, 0);
       sad::p2d::Vector v2(0, 0);
       r2.setCutter(sad::p2d::cutter(0, 0, 0, 2));
       r1.setCutter(sad::p2d::cutter(6, 2, 6, 0));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 0);
    }


} test_find_contact_points_ltol;