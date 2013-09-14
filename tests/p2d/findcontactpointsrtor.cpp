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
 * Tests finding contact points between two rectangles
 */
struct TestFindContactPointsRtoR : tpunit::TestFixture
{
 public:
   TestFindContactPointsRtoR() : tpunit::TestFixture(
	   TEST(TestFindContactPointsRtoR::test1),
	   TEST(TestFindContactPointsRtoR::test2),
	   TEST(TestFindContactPointsRtoR::test3),
	   TEST(TestFindContactPointsRtoR::test4),
	   TEST(TestFindContactPointsRtoR::test5),
	   TEST(TestFindContactPointsRtoR::test6),
	   TEST(TestFindContactPointsRtoR::test7),
	   TEST(TestFindContactPointsRtoR::test8),
	   TEST(TestFindContactPointsRtoR::test9),
	   TEST(TestFindContactPointsRtoR::test10),
	   TEST(TestFindContactPointsRtoR::test11),
	   TEST(TestFindContactPointsRtoR::test12),
	   TEST(TestFindContactPointsRtoR::test13),
	   TEST(TestFindContactPointsRtoR::test14),
	   TEST(TestFindContactPointsRtoR::test15),
	   TEST(TestFindContactPointsRtoR::test16),
	   TEST(TestFindContactPointsRtoR::test17),
	   TEST(TestFindContactPointsRtoR::test18)	
   ) {}

   void test1()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 1), hPointF(1, 2),
				   hPointF(2, 1), hPointF(1, 0))	  
	   );
	   r2.setRect(
		    hRectF(hPointF(3, 2), hPointF(5, 2),
			       hPointF(5, 0), hPointF(3, 0))
	   );
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
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(2, 1), hPointF(3, 2),
				   hPointF(4, 1), hPointF(3, 0))	  
	   );
	   r2.setRect(
		    hRectF(hPointF(3, 2), hPointF(5, 2),
			       hPointF(5, 0), hPointF(3, 0))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 4, 1, 3, 1));
   }

   void test3()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(4, 1), hPointF(5, 2),
				   hPointF(6, 1), hPointF(5, 0))	  
	   );
	   r2.setRect(
		    hRectF(hPointF(3, 2), hPointF(5, 2),
			       hPointF(5, 0), hPointF(3, 0))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 6, 1, 3, 1));
   }

   void test4()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(6, 1), hPointF(7, 2),
				   hPointF(8, 1), hPointF(7, 0))	  
	   );
	   r2.setRect(
		    hRectF(hPointF(3, 2), hPointF(5, 2),
			       hPointF(5, 0), hPointF(3, 0))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 8, 1, 3, 1));
   }


   void test5()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 4), hPointF(1, 5),
				   hPointF(2, 4), hPointF(1, 3))	  
	   );
	   r2.setRect(
		    hRectF(hPointF(0, 2), hPointF(2, 2),
			       hPointF(2, 0), hPointF(0, 0))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 1, 3, 1, 2));
   }

   void test6()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 2), hPointF(1, 3),
				   hPointF(2, 2), hPointF(1, 1))	  
	   );
	   r2.setRect(
		    hRectF(hPointF(0, 2), hPointF(2, 2),
			       hPointF(2, 0), hPointF(0, 0))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 1, 1, 1, 2));
   }

   void test7()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 0), hPointF(1, 1),
				   hPointF(2, 0), hPointF(1, -1))	  
	   );
	   r2.setRect(
		    hRectF(hPointF(0, 2), hPointF(2, 2),
			       hPointF(2, 0), hPointF(0, 0))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 1, -1, 1, 2));
   }

   void test8()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, -2), hPointF(1, -1),
				   hPointF(2, -2), hPointF(1, -3))	  
	   );
	   r2.setRect(
		    hRectF(hPointF(0, 2), hPointF(2, 2),
			       hPointF(2, 0), hPointF(0, 0))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 1);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 1, -3, 1, 2));
   }

   void test9()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 0), hPointF(0, 2),
				   hPointF(2, 2), hPointF(2, 0))	  
	   );
	   r2.setRect(
		    hRectF(hPointF(4, 0), hPointF(4, 2),
			       hPointF(6, 2), hPointF(6, 0))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 2);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 2, 4, 2));
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 0, 4, 0));
   }

   void test10()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(3, 0), hPointF(3, 2),
				   hPointF(5, 2), hPointF(5, 0))	  
	   );
	   r2.setRect(
		    hRectF(hPointF(4, 0), hPointF(4, 2),
			       hPointF(6, 2), hPointF(6, 0))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 2);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 5, 2, 4, 2));
	   ASSERT_TRUE(p2d::hasPair(pairs, 5, 0, 4, 0));
   }

   void test11()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(5, 0), hPointF(5, 2),
				   hPointF(7, 2), hPointF(7, 0))	  
	   );
	   r2.setRect(
		    hRectF(hPointF(4, 0), hPointF(4, 2),
			       hPointF(6, 2), hPointF(6, 0))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 2);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 7, 2, 4, 2));
	   ASSERT_TRUE(p2d::hasPair(pairs, 7, 0, 4, 0));
   }

   void test12()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(7, 0), hPointF(7, 2),
				   hPointF(9, 2), hPointF(9, 0))	  
	   );
	   r2.setRect(
		    hRectF(hPointF(4, 0), hPointF(4, 2),
			       hPointF(6, 2), hPointF(6, 0))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 2);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 9, 2, 4, 2));
	   ASSERT_TRUE(p2d::hasPair(pairs, 9, 0, 4, 0));
   }

   void test13()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 0), hPointF(0, 2),
				   hPointF(2, 2), hPointF(2, 0))	  
	   );
	   r2.setRect(
		    hRectF(hPointF(4, -1), hPointF(4, 1),
			       hPointF(6, 1), hPointF(6, -1))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 2);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 0, 4, 0));
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 1, 4, 1));
   }

   void test14()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r1.setRect(
			hRectF(hPointF(0, 0), hPointF(0, 2),
				   hPointF(2, 2), hPointF(2, 0))	  
	   );
	   r2.setRect(
		    hRectF(hPointF(1, -1), hPointF(1, 1),
			       hPointF(3, 1), hPointF(3, -1))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 2);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 1, 1, 1));
	   ASSERT_TRUE(p2d::hasPair(pairs, 2, 0, 1, 0));
   }

   void test15()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r2.setRect(
			hRectF(hPointF(0, 0), hPointF(0, 2),
				   hPointF(2, 2), hPointF(2, 0))	  
	   );
	   r1.setRect(
		    hRectF(hPointF(4, -1), hPointF(4, 1),
			       hPointF(6, 1), hPointF(6, -1))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 2);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 6, 0, 0, 0));
	   ASSERT_TRUE(p2d::hasPair(pairs, 6, 1, 0, 1));
   }


   void test16()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(1, 0);
	   p2d::Vector v2(0, 0);
	   r2.setRect(
			hRectF(hPointF(0, 0), hPointF(0, 2),
				   hPointF(2, 2), hPointF(2, 0))	  
	   );
	   r1.setRect(
		    hRectF(hPointF(1, -1), hPointF(1, 1),
			       hPointF(3, 1), hPointF(3, -1))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 2);
	   p2d::PointsPair pair = pairs[0];
	   ASSERT_TRUE(p2d::hasPair(pairs, 3, 1, 0, 1));
	   ASSERT_TRUE(p2d::hasPair(pairs, 3, 0, 0, 0));
   }

   void test17()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(0, -1);
	   p2d::Vector v2(0, 0);
	   r2.setRect(
			hRectF(hPointF(0, 0), hPointF(0, 2),
				   hPointF(2, 2), hPointF(2, 0))	  
	   );
	   r1.setRect(
		    hRectF(hPointF(4, 0), hPointF(4, 2),
			       hPointF(6, 2), hPointF(6, 0))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 0);
	}

   void test18()
   {
	   p2d::Rectangle r1;
	   p2d::Rectangle r2;
	   p2d::Vector v1(0, 0);
	   p2d::Vector v2(0, 0);
	   r2.setRect(
			hRectF(hPointF(0, 0), hPointF(0, 2),
				   hPointF(2, 2), hPointF(2, 0))	  
	   );
	   r1.setRect(
		    hRectF(hPointF(4, 0), hPointF(4, 2),
			       hPointF(6, 2), hPointF(6, 0))
	   );
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( pairs.size() == 0);
	}



} test_find_contact_points_rtor;