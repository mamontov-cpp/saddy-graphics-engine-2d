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
 * Tests finding contact points between circle and line
 */
struct TestFindContactPointsCtoL : tpunit::TestFixture
{
 public:
   TestFindContactPointsCtoL() : tpunit::TestFixture(
       TEST(TestFindContactPointsCtoL::test1),
       TEST(TestFindContactPointsCtoL::test2),
       TEST(TestFindContactPointsCtoL::test3),
       TEST(TestFindContactPointsCtoL::test4),
       TEST(TestFindContactPointsCtoL::test5),
       TEST(TestFindContactPointsCtoL::test6),
       TEST(TestFindContactPointsCtoL::test7),
       TEST(TestFindContactPointsCtoL::test8),
       TEST(TestFindContactPointsCtoL::test9),
       TEST(TestFindContactPointsCtoL::test10),
       TEST(TestFindContactPointsCtoL::test11),
       TEST(TestFindContactPointsCtoL::test12),
       TEST(TestFindContactPointsCtoL::test13),
       TEST(TestFindContactPointsCtoL::test14),
       TEST(TestFindContactPointsCtoL::test15),
       TEST(TestFindContactPointsCtoL::test16),
       TEST(TestFindContactPointsCtoL::test17),
       TEST(TestFindContactPointsCtoL::test18)
   ) {}

   void test1()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(1, 1));
       r1.setRadius(1);
       r2.setCutter(sad::p2d::cutter(5, 2, 5, -1));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 2, 1, 5, 1));
   }

   void test2()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(4, 1));
       r1.setRadius(1);
       r2.setCutter(sad::p2d::cutter(5, 2, 5, -1));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 5, 1, 5, 1));
   }

   void test3()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(5, 1));
       r1.setRadius(1);
       r2.setCutter(sad::p2d::cutter(5, 2, 5, -1));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 6, 1, 5, 1));
   }

   void test4()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(8, 1));
       r1.setRadius(1);
       r2.setCutter(sad::p2d::cutter(5, 2, 5, -1));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 9, 1, 5, 1));
   }

   void test5()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(1, 2));
       r1.setRadius(1);
       r2.setCutter(sad::p2d::cutter(1, 0, 3, 0));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 1, 1, 1, 0));
   }

   void test6()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(1, 1));
       r1.setRadius(1);
       r2.setCutter(sad::p2d::cutter(1, 0, 3, 0));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 1, 0, 1, 0));
   }

   void test7()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(1, 0));
       r1.setRadius(1);
       r2.setCutter(sad::p2d::cutter(1, 0, 3, 0));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 1, -1, 1, 0));
   }

   void test8()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(1, -2));
       r1.setRadius(1);
       r2.setCutter(sad::p2d::cutter(1, 0, 3, 0));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 1, -3, 1, 0));
   }

   void test9()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(-1, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(2, 1));
       r1.setRadius(M_SQRT2);
       r2.setCutter(sad::p2d::cutter(0, 3, 0, 5));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 1, 2, 0, 3));
   }

   void test10()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(-1, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(1, 2));
       r1.setRadius(M_SQRT2);
       r2.setCutter(sad::p2d::cutter(0, 3, 0, 5));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 0, 3, 0, 3));
   }

   void test11()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(-1, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(0, 3));
       r1.setRadius(M_SQRT2);
       r2.setCutter(sad::p2d::cutter(0, 3, 0, 5));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, -1, 4, 0, 3));
   }

   void test12()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(-1, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(-1, 4));
       r1.setRadius(M_SQRT2);
       r2.setCutter(sad::p2d::cutter(0, 3, 0, 5));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, -2, 5, 0, 3));
   }


   void test13()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(-1, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(2, 1));
       r1.setRadius(M_SQRT2);
       r2.setCutter(sad::p2d::cutter(-1, 2, 1, 4));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 1, 2, 0, 3));
   }

   void test14()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(-1, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(1, 2));
       r1.setRadius(M_SQRT2);
       r2.setCutter(sad::p2d::cutter(-1, 2, 1, 4));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 0, 3, 0, 3));
   }

   void test15()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(-1, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(0, 3));
       r1.setRadius(M_SQRT2);
       r2.setCutter(sad::p2d::cutter(-1, 2, 1, 4));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, -1, 4, 0, 3));
   }

   void test16()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(-1, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(-1, 4));
       r1.setRadius(M_SQRT2);
       r2.setCutter(sad::p2d::cutter(-1, 2, 1, 4));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, -2, 5, 0, 3));
   }

   void test17()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(1, 1));
       r1.setRadius(1);
       r2.setCutter(sad::p2d::cutter(5, 2, 5, -1));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 0);
   }

   void test18()
   {
       sad::p2d::Circle r1;
       sad::p2d::Line r2;
       sad::p2d::Vector v1(0, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Vector(1, 1));
       r1.setRadius(1);
       r2.setCutter(sad::p2d::cutter(5, 2, 5, -1));
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 0);
   }

} /*test_find_contact_points_ctol*/;