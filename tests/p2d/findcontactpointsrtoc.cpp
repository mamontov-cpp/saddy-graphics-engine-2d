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
 * Tests finding contact points between rectangle and circle
 */
struct TestFindContactPointsRtoC : tpunit::TestFixture
{
 public:
   TestFindContactPointsRtoC() : tpunit::TestFixture(
	    TEST(TestFindContactPointsRtoC::test1),	   	
	    TEST(TestFindContactPointsRtoC::test2),	   	
	    TEST(TestFindContactPointsRtoC::test3),	   	
	    TEST(TestFindContactPointsRtoC::test4), 	   	
	    TEST(TestFindContactPointsRtoC::test5), 	   	
	    TEST(TestFindContactPointsRtoC::test6), 	   	
	    TEST(TestFindContactPointsRtoC::test7), 	   	
	    TEST(TestFindContactPointsRtoC::test8),
		TEST(TestFindContactPointsRtoC::test9),
		TEST(TestFindContactPointsRtoC::test10),
		TEST(TestFindContactPointsRtoC::test11),
		TEST(TestFindContactPointsRtoC::test12),
		TEST(TestFindContactPointsRtoC::test13),
		TEST(TestFindContactPointsRtoC::test14),
		TEST(TestFindContactPointsRtoC::test15),
		TEST(TestFindContactPointsRtoC::test16),
		TEST(TestFindContactPointsRtoC::test17),
		TEST(TestFindContactPointsRtoC::test18)
	) {}

   void test1()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 1), hPointF(1, 2),
				   hPointF(2, 1), hPointF(1, 0))	  
	   );
	   r2.setCenter(p2d::Point(5,1));
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 1, 4, 1));
   }

   void test2()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 1), hPointF(1, 2),
				   hPointF(2, 1), hPointF(1, 0))	  
	   );
	   r2.setCenter(p2d::Point(2,1));
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
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 1), hPointF(1, 2),
				   hPointF(2, 1), hPointF(1, 0))	  
	   );
	   r2.setCenter(p2d::Point(0,1));
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 1, -1, 1));
   }

   void test4()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 1), hPointF(1, 2),
				   hPointF(2, 1), hPointF(1, 0))	  
	   );
	   r2.setCenter(p2d::Point(-1,1));
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 1, -2, 1));
   }

   void test5()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, -1);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 2), hPointF(0, 4),
				   hPointF(2, 4), hPointF(2, 2))	  
	   );
	   r2.setCenter(p2d::Point(4,0));
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 2, 3, 1));
   }

   void test6()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, -1);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(2, 0), hPointF(2, 2),
				   hPointF(4, 2), hPointF(4, 0))	  
	   );
	   r2.setCenter(p2d::Point(4,0));
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 4, 0, 3, 1));
   }

   void test7()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, -1);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(4, -2), hPointF(4, 0),
				   hPointF(6, 0), hPointF(6, -2))	  
	   );
	   r2.setCenter(p2d::Point(4,0));
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 6, -2, 3, 1));
   }

   void test8()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, -1);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(6, -4), hPointF(6, -2),
				   hPointF(8, -2), hPointF(8, -4))	  
	   );
	   r2.setCenter(p2d::Point(4,0));
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 8, -4, 3, 1));
   }

   void test9()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 5), hPointF(1, 6),
				   hPointF(2, 5), hPointF(1, 4))	  
	   );
	   r2.setCenter(p2d::Point(1,1));
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 1, 4, 1, 2));
   }

   void test10()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 2), hPointF(1, 3),
				   hPointF(2, 2), hPointF(1, 1))	  
	   );
	   r2.setCenter(p2d::Point(1,1));
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 1, 1, 1, 2));
   }

   void test11()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 0), hPointF(1, 1),
				   hPointF(2, 0), hPointF(1, -1))	  
	   );
	   r2.setCenter(p2d::Point(1,1));
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 1, -1, 1, 2));
   }

   void test12()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, -3), hPointF(1, -2),
				   hPointF(2, -3), hPointF(1, -4))	  
	   );
	   r2.setCenter(p2d::Point(1,1));
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 1, -4, 1, 2));
   }

   void test13()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 2), hPointF(0, 4),
				   hPointF(2, 4), hPointF(2, 2))	  
	   );
	   r2.setCenter(p2d::Point(4,1));
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 2, 3, 2));
   }

   void test14()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(2, 2), hPointF(2, 4),
				   hPointF(4, 4), hPointF(4, 2))	  
	   );
	   r2.setCenter(p2d::Point(4,1));
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 4, 2, 3, 2));
   }

   void test15()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(4, 2), hPointF(4, 4),
				   hPointF(6, 4), hPointF(6, 2))	  
	   );
	   r2.setCenter(p2d::Point(4,1));
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 6, 2, 3, 2));
   }

   void test16()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(6, 2), hPointF(6, 4),
				   hPointF(8, 4), hPointF(8, 2))	  
	   );
	   r2.setCenter(p2d::Point(4,1));
	   r2.setRadius(M_SQRT2);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 8, 2, 3, 2));
   }

   void test17()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 0), hPointF(0, 2),
				   hPointF(2, 2), hPointF(2, 0))	  
	   );
	   r2.setCenter(p2d::Point(5,1));
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 0);
	}

   void test18()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   p2d::Vector v1(0, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 0), hPointF(0, 2),
				   hPointF(2, 2), hPointF(2, 0))	  
	   );
	   r2.setCenter(p2d::Point(5,1));
	   r2.setRadius(1);
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 0);
	}

} test_find_contact_points_rtoc;