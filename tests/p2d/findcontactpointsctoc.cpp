#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <3rdparty/tpunit++/tpunit++.hpp>
#include <templates/hlvector.hpp>
#include <p2d/findcontactpoints.h>
#include <extra/geometry2d.h>
#pragma warning(pop)

/*!
 * Tests finding contact points between two circles
 */
struct TestFindContactPointsCtoC : tpunit::TestFixture
{
 public:
   TestFindContactPointsCtoC() : tpunit::TestFixture(
	   TEST(TestFindContactPointsCtoC::test1),
	   TEST(TestFindContactPointsCtoC::test2),
	   TEST(TestFindContactPointsCtoC::test3),
	   TEST(TestFindContactPointsCtoC::test4),
	   TEST(TestFindContactPointsCtoC::test5),
	   TEST(TestFindContactPointsCtoC::test6),
	   TEST(TestFindContactPointsCtoC::test7),
	   TEST(TestFindContactPointsCtoC::test8),
	   TEST(TestFindContactPointsCtoC::test9),
	   TEST(TestFindContactPointsCtoC::test10),
	   TEST(TestFindContactPointsCtoC::test11),
	   TEST(TestFindContactPointsCtoC::test12),
	   TEST(TestFindContactPointsCtoC::test13),
	   TEST(TestFindContactPointsCtoC::test14),
	   TEST(TestFindContactPointsCtoC::test15),
	   TEST(TestFindContactPointsCtoC::test16),
 	   TEST(TestFindContactPointsCtoC::test17),
 	   TEST(TestFindContactPointsCtoC::test18),
 	   TEST(TestFindContactPointsCtoC::test19), 
 	   TEST(TestFindContactPointsCtoC::test20), 
 	   TEST(TestFindContactPointsCtoC::test21) 
   ) {}

   void test1()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(1, 1));
	   r2.setCenter(p2d::Vector(4, 1));
	   r1.setRadius(1);
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 1, 3, 1));
   }

   void test2()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(1, 1));
	   r2.setCenter(p2d::Vector(2, 1));
	   r1.setRadius(1);
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 1, 1, 1));
   }

   void test3()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(2, 1));
	   r2.setCenter(p2d::Vector(1, 1));
	   r1.setRadius(1);
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 3, 1, 0, 1));
   }

   void test4()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(4, 1));
	   r2.setCenter(p2d::Vector(1, 1));
	   r1.setRadius(1);
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 5, 1, 0, 1));
   }

   void test5()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(1, 1));
	   r2.setCenter(p2d::Vector(1, 1));
	   r1.setRadius(1);
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 1, 0, 1));
   }

   void test6()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(1, 4));
	   r2.setCenter(p2d::Vector(1, 1));
	   r1.setRadius(1);
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 1, 3, 1, 2));
   }

   void test7()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(1, 2));
	   r2.setCenter(p2d::Vector(1, 1));
	   r1.setRadius(1);
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 1, 1, 1, 2));
   }

   void test8()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(1, 1));
	   r2.setCenter(p2d::Vector(1, 2));
	   r1.setRadius(1);
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 1, 0, 1, 3));
   }

   void test9()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(1, 1));
	   r2.setCenter(p2d::Vector(1, 4));
	   r1.setRadius(1);
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 1, 0, 1, 5));
   }

   void test10()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(1, 1));
	   r2.setCenter(p2d::Vector(1, 1));
	   r1.setRadius(1);
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 1, 0, 1, 2));
   }

   void test11()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, -1);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(1, 4));
	   r2.setCenter(p2d::Vector(4, 1));
	   r1.setRadius(M_SQRT2);
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 3, 3, 2));
   }

   void test12()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, -1);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(2, 3));
	   r2.setCenter(p2d::Vector(4, 1));
	   r1.setRadius(M_SQRT2);
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 3, 2, 3, 2));
   }

   void test13()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, -1);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(3, 2));
	   r2.setCenter(p2d::Vector(4, 1));
	   r1.setRadius(M_SQRT2);
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 4, 1, 3, 2));
   }

   void test14()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, -1);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(4, 1));
	   r2.setCenter(p2d::Vector(4, 1));
	   r1.setRadius(M_SQRT2);
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 5, 0, 3, 2));
   }

   void test15()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, -1);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(5, 0));
	   r2.setCenter(p2d::Vector(4, 1));
	   r1.setRadius(M_SQRT2);
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 6, -1, 3, 2));
   }

   void test16()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(1, 1));
	   r2.setCenter(p2d::Vector(4, 3));
	   r1.setRadius(M_SQRT2);
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 2, 3, 2));
   }

   void test17()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(2, 1));
	   r2.setCenter(p2d::Vector(4, 3));
	   r1.setRadius(M_SQRT2);
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 3, 2, 3, 2));
   }

   void test18()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(3, 1));
	   r2.setCenter(p2d::Vector(4, 3));
	   r1.setRadius(M_SQRT2);
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 4, 2, 3, 2));
   }

   void test19()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(4, 1));
	   r2.setCenter(p2d::Vector(4, 3));
	   r1.setRadius(M_SQRT2);
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 5, 2, 3, 2));
   }


   void test20()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(0, 1);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(1, 1));
	   r2.setCenter(p2d::Vector(4, 1));
	   r1.setRadius(1);
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 0);
   }

   void test21()
   {
	   p2d::Circle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(0, 0);
	   p2d::Vector v2(0, 0);
	   r1.setCenter(p2d::Vector(1, 1));
	   r2.setCenter(p2d::Vector(4, 1));
	   r1.setRadius(1);
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 0);
   }

} test_find_contact_points_ctoc;