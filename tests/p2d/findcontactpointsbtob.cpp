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
 * Tests finding contact points between two bounds
 */
struct TestFindContactPointsBtoB : tpunit::TestFixture
{
 public:
   TestFindContactPointsBtoB() : tpunit::TestFixture(
       TEST(TestFindContactPointsBtoB::test1),
       TEST(TestFindContactPointsBtoB::test2),
       TEST(TestFindContactPointsBtoB::test3),
       TEST(TestFindContactPointsBtoB::test4),
       TEST(TestFindContactPointsBtoB::test5),
       TEST(TestFindContactPointsBtoB::test6),
       TEST(TestFindContactPointsBtoB::test7),
       TEST(TestFindContactPointsBtoB::test8),
       TEST(TestFindContactPointsBtoB::test9),
       TEST(TestFindContactPointsBtoB::test10),
       TEST(TestFindContactPointsBtoB::test11),
       TEST(TestFindContactPointsBtoB::test12),
       TEST(TestFindContactPointsBtoB::test13),
       TEST(TestFindContactPointsBtoB::test14),
       TEST(TestFindContactPointsBtoB::test15),
       TEST(TestFindContactPointsBtoB::test16),
       TEST(TestFindContactPointsBtoB::test17),
       TEST(TestFindContactPointsBtoB::test18),
       TEST(TestFindContactPointsBtoB::test19)
   ) {}

   void test1()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setPosition(0);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_LEFT);
       r2.setType(sad::p2d::BT_LEFT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW( m.invoke(s1, v1, s2, v2) , sad::p2d::CannotDetermineContactPoints);
   }

   void test2()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setPosition(0);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_RIGHT);
       r2.setType(sad::p2d::BT_RIGHT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW( m.invoke(s1, v1, s2, v2) , sad::p2d::CannotDetermineContactPoints);
   }

   void test3()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setPosition(0);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_UP);
       r2.setType(sad::p2d::BT_UP);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW( m.invoke(s1, v1, s2, v2) , sad::p2d::CannotDetermineContactPoints);
   }

   void test4()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setPosition(0);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_DOWN);
       r2.setType(sad::p2d::BT_DOWN);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW( m.invoke(s1, v1, s2, v2) , sad::p2d::CannotDetermineContactPoints);
   }

   void test5()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setPosition(0);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_LEFT);
       r2.setType(sad::p2d::BT_UP);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW( m.invoke(s1, v1, s2, v2) , sad::p2d::CannotDetermineContactPoints);
   }

   void test6()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setPosition(0);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_LEFT);
       r2.setType(sad::p2d::BT_DOWN);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW( m.invoke(s1, v1, s2, v2) , sad::p2d::CannotDetermineContactPoints);
   }

   void test7()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setPosition(0);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_RIGHT);
       r2.setType(sad::p2d::BT_UP);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW( m.invoke(s1, v1, s2, v2) , sad::p2d::CannotDetermineContactPoints);
   }

   void test8()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setPosition(0);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_RIGHT);
       r2.setType(sad::p2d::BT_DOWN);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW( m.invoke(s1, v1, s2, v2) , sad::p2d::CannotDetermineContactPoints);
   }


   void test9()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setPosition(0);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_UP);
       r2.setType(sad::p2d::BT_LEFT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW( m.invoke(s1, v1, s2, v2) , sad::p2d::CannotDetermineContactPoints);
   }

   void test10()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setPosition(0);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_UP);
       r2.setType(sad::p2d::BT_RIGHT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW( m.invoke(s1, v1, s2, v2) , sad::p2d::CannotDetermineContactPoints);
   }

   void test11()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setPosition(0);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_DOWN);
       r2.setType(sad::p2d::BT_LEFT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW( m.invoke(s1, v1, s2, v2) , sad::p2d::CannotDetermineContactPoints);
   }

   void test12()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setPosition(0);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_DOWN);
       r2.setType(sad::p2d::BT_RIGHT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW( m.invoke(s1, v1, s2, v2) , sad::p2d::CannotDetermineContactPoints);
   }


   void test13()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, 1);
       sad::p2d::Vector v2(0, -1);
       r1.setPosition(1);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_LEFT);
       r2.setType(sad::p2d::BT_RIGHT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.count() == 0);
   }

   void test14()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, 1);
       sad::p2d::Vector v2(0, -1);
       r1.setPosition(1);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_RIGHT);
       r2.setType(sad::p2d::BT_LEFT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.count() == 0);
   }


   void test15()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(-1, 0);
       r1.setPosition(1);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_UP);
       r2.setType(sad::p2d::BT_DOWN);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.count() == 0);
   }

   void test16()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(-1, 0);
       r1.setPosition(1);
       r2.setPosition(0);
       r1.setType(sad::p2d::BT_DOWN);
       r2.setType(sad::p2d::BT_UP);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.count() == 0);
   }

   void test17()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(-1, 0);
       r1.setPosition(2);
       r2.setPosition(3);
       r1.setType(sad::p2d::BT_RIGHT);
       r2.setType(sad::p2d::BT_LEFT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.count() == 2);
       ASSERT_TRUE(pairs[0].p1().x() == 2);
       ASSERT_TRUE(pairs[0].p2().x() == 3);
       ASSERT_TRUE(pairs[1].p1().x() == 2);
       ASSERT_TRUE(pairs[1].p2().x() == 3);
   }

   void test18()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(-1, 0);
       r1.setPosition(2);
       r2.setPosition(3);
       r1.setType(sad::p2d::BT_LEFT);
       r2.setType(sad::p2d::BT_RIGHT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.count() == 2);
       ASSERT_TRUE(pairs[0].p1().x() == 2);
       ASSERT_TRUE(pairs[0].p2().x() == 3);
       ASSERT_TRUE(pairs[1].p1().x() == 2);
       ASSERT_TRUE(pairs[1].p2().x() == 3);
   }

   void test19()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, 1);
       sad::p2d::Vector v2(0, -1);
       r1.setPosition(2);
       r2.setPosition(3);
       r1.setType(sad::p2d::BT_UP);
       r2.setType(sad::p2d::BT_DOWN);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.count() == 2);
       ASSERT_TRUE(pairs[0].p1().y() == 2);
       ASSERT_TRUE(pairs[0].p2().y() == 3);
       ASSERT_TRUE(pairs[1].p1().y() == 2);
       ASSERT_TRUE(pairs[1].p2().y() == 3);
   }

   void test20()
   {
       sad::p2d::Bound r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, 1);
       sad::p2d::Vector v2(0, -1);
       r1.setPosition(2);
       r2.setPosition(3);
       r1.setType(sad::p2d::BT_DOWN);
       r2.setType(sad::p2d::BT_UP);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.count() == 2);
       ASSERT_TRUE(pairs[0].p1().y() == 2);
       ASSERT_TRUE(pairs[0].p2().y() == 3);
       ASSERT_TRUE(pairs[1].p1().y() == 2);
       ASSERT_TRUE(pairs[1].p2().y() == 3);
   }

} test_find_contact_points_btob;