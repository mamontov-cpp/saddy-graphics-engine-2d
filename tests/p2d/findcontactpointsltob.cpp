#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#pragma warning(disable: 4101)
#include <cstdio>
#include <3rdparty/tpunit++/tpunit++.hpp>
#include <sadvector.h>
#include <p2d/findcontactpoints.h>
#include <geometry2d.h>
#pragma warning(pop)

/*!
 * Tests finding contact points between bound and line
 */
struct TestFindContactPointsLtoB : tpunit::TestFixture
{
 public:
   TestFindContactPointsLtoB() : tpunit::TestFixture(
       TEST(TestFindContactPointsLtoB::test1),
       TEST(TestFindContactPointsLtoB::test2),
       TEST(TestFindContactPointsLtoB::test3),
       TEST(TestFindContactPointsLtoB::test4),
       TEST(TestFindContactPointsLtoB::test5),
       TEST(TestFindContactPointsLtoB::test6),
       TEST(TestFindContactPointsLtoB::test7),
       TEST(TestFindContactPointsLtoB::test8),
       TEST(TestFindContactPointsLtoB::test9),
       TEST(TestFindContactPointsLtoB::test10),
       TEST(TestFindContactPointsLtoB::test11),
       TEST(TestFindContactPointsLtoB::test12)
   ) {}

   void test1()
   {
       sad::p2d::Line r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter( sad::p2d::cutter(1, 1, 1, 2) );
       r2.setPosition(-2);
       r2.setType(sad::p2d::BoundType::BT_LEFT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW(m.invoke(s1, v1, s2, v2), sad::p2d::CannotDetermineContactPoints);
   }


   void test2()
   {
       sad::p2d::Line r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(-1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter( sad::p2d::cutter(1, 1, 1, 2) );
       r2.setPosition(3);
       r2.setType(sad::p2d::BoundType::BT_RIGHT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW(m.invoke(s1, v1, s2, v2), sad::p2d::CannotDetermineContactPoints);
   }

   void test3()
   {
       sad::p2d::Line r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter( sad::p2d::cutter(1, 1, 1, 2) );
       r2.setPosition(3);
       r2.setType(sad::p2d::BoundType::BT_UP);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW(m.invoke(s1, v1, s2, v2), sad::p2d::CannotDetermineContactPoints);
   }


   void test4()
   {
       sad::p2d::Line r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter( sad::p2d::cutter(1, 1, 1, 2) );
       r2.setPosition(0);
       r2.setType(sad::p2d::BoundType::BT_DOWN);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW(m.invoke(s1, v1, s2, v2), sad::p2d::CannotDetermineContactPoints);
   }


   void test5()
   {
       sad::p2d::Line r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter( sad::p2d::cutter(1, 1, 1, 2) );
       r2.setPosition(-2);
       r2.setType(sad::p2d::BoundType::BT_LEFT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.size() == 0);
   }


   void test6()
   {
       sad::p2d::Line r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter( sad::p2d::cutter(1, 1, 1, 2) );
       r2.setPosition(3);
       r2.setType(sad::p2d::BoundType::BT_RIGHT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.size() == 0);
   }

   void test7()
   {
       sad::p2d::Line r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter( sad::p2d::cutter(1, 1, 1, 2) );
       r2.setPosition(3);
       r2.setType(sad::p2d::BoundType::BT_UP);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.size() == 0);
   }


   void test8()
   {
       sad::p2d::Line r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter( sad::p2d::cutter(1, 1, 1, 2) );
       r2.setPosition(0);
       r2.setType(sad::p2d::BoundType::BT_DOWN);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.size() == 0);
   }

   void test9()
   {
       sad::p2d::Line r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(-1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter( sad::p2d::cutter(1, 1, 1, 2) );
       r2.setPosition(-2);
       r2.setType(sad::p2d::BoundType::BT_LEFT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( sad::p2d::hasPair(pairs, 1, 1, -2, 1) );
       ASSERT_TRUE( sad::p2d::hasPair(pairs, 1, 2, -2, 2) );
   }

   void test10()
   {
       sad::p2d::Line r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter( sad::p2d::cutter(2, 1, 2, 2) );
       r2.setPosition(3);
       r2.setType(sad::p2d::BoundType::BT_RIGHT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( sad::p2d::hasPair(pairs, 2, 1, 3, 1) );
       ASSERT_TRUE( sad::p2d::hasPair(pairs, 2, 2, 3, 2) );
   }

   void test11()
   {
       sad::p2d::Line r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter( sad::p2d::cutter(1, 2, 2, 2) );
       r2.setPosition(3);
       r2.setType(sad::p2d::BoundType::BT_UP);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( sad::p2d::hasPair(pairs, 1, 2, 1, 3) );
       ASSERT_TRUE( sad::p2d::hasPair(pairs, 2, 2, 2, 3) );
   }

   void test12()
   {
       sad::p2d::Line r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setCutter( sad::p2d::cutter(1, 1, 2, 1) );
       r2.setPosition(-3);
       r2.setType(sad::p2d::BoundType::BT_DOWN);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( sad::p2d::hasPair(pairs, 1, 1, 1, -3) );
       ASSERT_TRUE( sad::p2d::hasPair(pairs, 2, 1, 2, -3) );
   }

}  test_find_contact_points_ltob;

