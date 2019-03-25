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
 * Tests finding contact points between bound and Circle
 */
struct TestFindContactPointsCtoB : tpunit::TestFixture
{
 public:
   TestFindContactPointsCtoB() : tpunit::TestFixture(
       TEST(TestFindContactPointsCtoB::test1),
       TEST(TestFindContactPointsCtoB::test2),
       TEST(TestFindContactPointsCtoB::test3),
       TEST(TestFindContactPointsCtoB::test4),
       TEST(TestFindContactPointsCtoB::test5),
       TEST(TestFindContactPointsCtoB::test6),
       TEST(TestFindContactPointsCtoB::test7),
       TEST(TestFindContactPointsCtoB::test8),
       TEST(TestFindContactPointsCtoB::test9),
       TEST(TestFindContactPointsCtoB::test10),
       TEST(TestFindContactPointsCtoB::test11),
       TEST(TestFindContactPointsCtoB::test12)
   ) {}

   void test1()
   {
       sad::p2d::Circle r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Point(0,0));
       r1.setRadius(1.0);
       r2.setPosition(-2);
       r2.setType(sad::p2d::BT_LEFT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW(m.invoke(s1, v1, s2, v2), sad::p2d::CannotDetermineContactPoints);
   }


   void test2()
   {
       sad::p2d::Circle r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(-1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Point(0,0));
       r1.setRadius(1.0);
       r2.setPosition(3);
       r2.setType(sad::p2d::BT_RIGHT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW(m.invoke(s1, v1, s2, v2), sad::p2d::CannotDetermineContactPoints);
   }

   void test3()
   {
       sad::p2d::Circle r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Point(0,0));
       r1.setRadius(1.0);
       r2.setPosition(3);
       r2.setType(sad::p2d::BT_UP);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW(m.invoke(s1, v1, s2, v2), sad::p2d::CannotDetermineContactPoints);
   }


   void test4()
   {
       sad::p2d::Circle r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Point(0,0));
       r1.setRadius(1.0);
       r2.setPosition(0);
       r2.setType(sad::p2d::BT_DOWN);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       EXPECT_THROW(m.invoke(s1, v1, s2, v2), sad::p2d::CannotDetermineContactPoints);
   }


   void test5()
   {
       sad::p2d::Circle r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Point(0,0));
       r1.setRadius(1.0);
       r2.setPosition(-2);
       r2.setType(sad::p2d::BT_LEFT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.size() == 0);
   }


   void test6()
   {
       sad::p2d::Circle r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Point(0,0));
       r1.setRadius(1.0);
       r2.setPosition(3);
       r2.setType(sad::p2d::BT_RIGHT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.size() == 0);
   }

   void test7()
   {
       sad::p2d::Circle r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Point(0,0));
       r1.setRadius(1.0);
       r2.setPosition(3);
       r2.setType(sad::p2d::BT_UP);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.size() == 0);
   }


   void test8()
   {
       sad::p2d::Circle r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Point(0,0));
       r1.setRadius(1.0);
       r2.setPosition(0);
       r2.setType(sad::p2d::BT_DOWN);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE(pairs.size() == 0);
   }

   void test9()
   {
       sad::p2d::Circle r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(-1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Point(0,0));
       r1.setRadius(1.0);
       r2.setPosition(-2);
       r2.setType(sad::p2d::BT_LEFT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( sad::p2d::hasPair(pairs, -1, 0, -2, 0) );
   }

   void test10()
   {
       sad::p2d::Circle r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Point(0,0));
       r1.setRadius(1.0);
       r2.setPosition(3);
       r2.setType(sad::p2d::BT_RIGHT);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( sad::p2d::hasPair(pairs, 1, 0, 3, 0) );
   }

   void test11()
   {
       sad::p2d::Circle r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, 1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Point(0,0));
       r1.setRadius(1.0);
       r2.setPosition(3);
       r2.setType(sad::p2d::BT_UP);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( sad::p2d::hasPair(pairs, 0, 1, 0, 3) );
   }

   void test12()
   {
       sad::p2d::Circle r1;
       sad::p2d::Bound r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setCenter(sad::p2d::Point(0,0));
       r1.setRadius(1.0);
       r2.setPosition(-3);
       r2.setType(sad::p2d::BT_DOWN);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( sad::p2d::hasPair(pairs, 0, -1, 0, -3) );
   }

}  test_find_contact_points_ctob;

