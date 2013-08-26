#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#pragma warning(disable: 4101)
#include <stdio.h>
#include <3rdparty/tpunit++/tpunit++.hpp>
#include <templates/hlvector.hpp>
#include <p2d/findcontactpoints.h>
#include <extra/geometry2d.h>
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
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setPosition(0);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_LEFT);
	   r2.setType(p2d::BT_LEFT);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW( m.invoke(s1, v1, s2, v2) , p2d::CannotDetermineContactPoints);
   }

   void test2()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setPosition(0);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_RIGHT);
	   r2.setType(p2d::BT_RIGHT);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW( m.invoke(s1, v1, s2, v2) , p2d::CannotDetermineContactPoints);
   }

   void test3()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setPosition(0);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_UP);
	   r2.setType(p2d::BT_UP);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW( m.invoke(s1, v1, s2, v2) , p2d::CannotDetermineContactPoints);
   }

   void test4()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setPosition(0);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_DOWN);
	   r2.setType(p2d::BT_DOWN);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW( m.invoke(s1, v1, s2, v2) , p2d::CannotDetermineContactPoints);
   }

   void test5()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setPosition(0);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_LEFT);
	   r2.setType(p2d::BT_UP);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW( m.invoke(s1, v1, s2, v2) , p2d::CannotDetermineContactPoints);
   }

   void test6()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setPosition(0);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_LEFT);
	   r2.setType(p2d::BT_DOWN);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW( m.invoke(s1, v1, s2, v2) , p2d::CannotDetermineContactPoints);
   }

   void test7()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setPosition(0);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_RIGHT);
	   r2.setType(p2d::BT_UP);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW( m.invoke(s1, v1, s2, v2) , p2d::CannotDetermineContactPoints);
   }

   void test8()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setPosition(0);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_RIGHT);
	   r2.setType(p2d::BT_DOWN);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW( m.invoke(s1, v1, s2, v2) , p2d::CannotDetermineContactPoints);
   }


   void test9()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setPosition(0);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_UP);
	   r2.setType(p2d::BT_LEFT);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW( m.invoke(s1, v1, s2, v2) , p2d::CannotDetermineContactPoints);
   }

   void test10()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setPosition(0);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_UP);
	   r2.setType(p2d::BT_RIGHT);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW( m.invoke(s1, v1, s2, v2) , p2d::CannotDetermineContactPoints);
   }

   void test11()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setPosition(0);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_DOWN);
	   r2.setType(p2d::BT_LEFT);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW( m.invoke(s1, v1, s2, v2) , p2d::CannotDetermineContactPoints);
   }

   void test12()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setPosition(0);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_DOWN);
	   r2.setType(p2d::BT_RIGHT);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW( m.invoke(s1, v1, s2, v2) , p2d::CannotDetermineContactPoints);
   }


   void test13()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(0, 1);
	   p2d::Vector v2(0, -1);
	   r1.setPosition(1);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_LEFT);
	   r2.setType(p2d::BT_RIGHT);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE(pairs.count() == 0);
   }

   void test14()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(0, 1);
	   p2d::Vector v2(0, -1);
	   r1.setPosition(1);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_RIGHT);
	   r2.setType(p2d::BT_LEFT);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE(pairs.count() == 0);
   }


   void test15()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(-1, 0);
	   r1.setPosition(1);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_UP);
	   r2.setType(p2d::BT_DOWN);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE(pairs.count() == 0);
   }

   void test16()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(-1, 0);
	   r1.setPosition(1);
	   r2.setPosition(0);
	   r1.setType(p2d::BT_DOWN);
	   r2.setType(p2d::BT_UP);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE(pairs.count() == 0);
   }

   void test17()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(-1, 0);
	   r1.setPosition(2);
	   r2.setPosition(3);
	   r1.setType(p2d::BT_RIGHT);
	   r2.setType(p2d::BT_LEFT);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE(pairs.count() == 2);
	   ASSERT_TRUE(pairs[0].p1().x() == 2);
	   ASSERT_TRUE(pairs[0].p2().x() == 3);
	   ASSERT_TRUE(pairs[1].p1().x() == 2);
	   ASSERT_TRUE(pairs[1].p2().x() == 3);
   }

   void test18()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(-1, 0);
	   r1.setPosition(2);
	   r2.setPosition(3);
	   r1.setType(p2d::BT_LEFT);
	   r2.setType(p2d::BT_RIGHT);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE(pairs.count() == 2);
	   ASSERT_TRUE(pairs[0].p1().x() == 2);
	   ASSERT_TRUE(pairs[0].p2().x() == 3);
	   ASSERT_TRUE(pairs[1].p1().x() == 2);
	   ASSERT_TRUE(pairs[1].p2().x() == 3);
   }

   void test19()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(0, 1);
	   p2d::Vector v2(0, -1);
	   r1.setPosition(2);
	   r2.setPosition(3);
	   r1.setType(p2d::BT_UP);
	   r2.setType(p2d::BT_DOWN);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE(pairs.count() == 2);
	   ASSERT_TRUE(pairs[0].p1().y() == 2);
	   ASSERT_TRUE(pairs[0].p2().y() == 3);
	   ASSERT_TRUE(pairs[1].p1().y() == 2);
	   ASSERT_TRUE(pairs[1].p2().y() == 3);
   }

   void test20()
   {
	   p2d::Bound r1;
	   p2d::Bound r2;
	   p2d::Vector v1(0, 1);
	   p2d::Vector v2(0, -1);
	   r1.setPosition(2);
	   r2.setPosition(3);
	   r1.setType(p2d::BT_DOWN);
	   r2.setType(p2d::BT_UP);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE(pairs.count() == 2);
	   ASSERT_TRUE(pairs[0].p1().y() == 2);
	   ASSERT_TRUE(pairs[0].p2().y() == 3);
	   ASSERT_TRUE(pairs[1].p1().y() == 2);
	   ASSERT_TRUE(pairs[1].p2().y() == 3);
   }

} test_find_contact_points_btob;