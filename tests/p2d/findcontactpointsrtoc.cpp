#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdio>
#include <3rdparty/tpunit++/tpunit++.hpp>
#include <sadvector.h>
#include <p2d/findcontactpoints.h>
#include <geometry2d.h>
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
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 1), sad::Point2D(1, 2),
                   sad::Point2D(2, 1), sad::Point2D(1, 0))	  
       );
       r2.setCenter(sad::p2d::Point(5,1));
       r2.setRadius(1);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 2, 1, 4, 1));
   }

   void test2()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 1), sad::Point2D(1, 2),
                   sad::Point2D(2, 1), sad::Point2D(1, 0))	  
       );
       r2.setCenter(sad::p2d::Point(2,1));
       r2.setRadius(1);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 2, 1, 1, 1));
   }

   void test3()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 1), sad::Point2D(1, 2),
                   sad::Point2D(2, 1), sad::Point2D(1, 0))	  
       );
       r2.setCenter(sad::p2d::Point(0,1));
       r2.setRadius(1);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 2, 1, -1, 1));
   }

   void test4()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 1), sad::Point2D(1, 2),
                   sad::Point2D(2, 1), sad::Point2D(1, 0))	  
       );
       r2.setCenter(sad::p2d::Point(-1,1));
       r2.setRadius(1);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 2, 1, -2, 1));
   }

   void test5()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(1, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 2), sad::Point2D(0, 4),
                   sad::Point2D(2, 4), sad::Point2D(2, 2))	  
       );
       r2.setCenter(sad::p2d::Point(4,0));
       r2.setRadius(M_SQRT2);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 2, 2, 3, 1));
   }

   void test6()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(1, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(2, 0), sad::Point2D(2, 2),
                   sad::Point2D(4, 2), sad::Point2D(4, 0))	  
       );
       r2.setCenter(sad::p2d::Point(4,0));
       r2.setRadius(M_SQRT2);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 4, 0, 3, 1));
   }

   void test7()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(1, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(4, -2), sad::Point2D(4, 0),
                   sad::Point2D(6, 0), sad::Point2D(6, -2))	  
       );
       r2.setCenter(sad::p2d::Point(4,0));
       r2.setRadius(M_SQRT2);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 6, -2, 3, 1));
   }

   void test8()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(1, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(6, -4), sad::Point2D(6, -2),
                   sad::Point2D(8, -2), sad::Point2D(8, -4))	  
       );
       r2.setCenter(sad::p2d::Point(4,0));
       r2.setRadius(M_SQRT2);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 8, -4, 3, 1));
   }

   void test9()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 5), sad::Point2D(1, 6),
                   sad::Point2D(2, 5), sad::Point2D(1, 4))	  
       );
       r2.setCenter(sad::p2d::Point(1,1));
       r2.setRadius(1);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 1, 4, 1, 2));
   }

   void test10()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 2), sad::Point2D(1, 3),
                   sad::Point2D(2, 2), sad::Point2D(1, 1))	  
       );
       r2.setCenter(sad::p2d::Point(1,1));
       r2.setRadius(1);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 1, 1, 1, 2));
   }

   void test11()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 0), sad::Point2D(1, 1),
                   sad::Point2D(2, 0), sad::Point2D(1, -1))	  
       );
       r2.setCenter(sad::p2d::Point(1,1));
       r2.setRadius(1);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 1, -1, 1, 2));
   }

   void test12()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, -3), sad::Point2D(1, -2),
                   sad::Point2D(2, -3), sad::Point2D(1, -4))	  
       );
       r2.setCenter(sad::p2d::Point(1,1));
       r2.setRadius(1);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 1, -4, 1, 2));
   }

   void test13()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 2), sad::Point2D(0, 4),
                   sad::Point2D(2, 4), sad::Point2D(2, 2))	  
       );
       r2.setCenter(sad::p2d::Point(4,1));
       r2.setRadius(M_SQRT2);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 2, 2, 3, 2));
   }

   void test14()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(2, 2), sad::Point2D(2, 4),
                   sad::Point2D(4, 4), sad::Point2D(4, 2))	  
       );
       r2.setCenter(sad::p2d::Point(4,1));
       r2.setRadius(M_SQRT2);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 4, 2, 3, 2));
   }

   void test15()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(4, 2), sad::Point2D(4, 4),
                   sad::Point2D(6, 4), sad::Point2D(6, 2))	  
       );
       r2.setCenter(sad::p2d::Point(4,1));
       r2.setRadius(M_SQRT2);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 6, 2, 3, 2));
   }

   void test16()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(1, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(6, 2), sad::Point2D(6, 4),
                   sad::Point2D(8, 4), sad::Point2D(8, 2))	  
       );
       r2.setCenter(sad::p2d::Point(4,1));
       r2.setRadius(M_SQRT2);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);
       sad::p2d::PointsPair pair = pairs[0];
       ASSERT_TRUE(sad::p2d::hasPair(pairs, 8, 2, 3, 2));
   }

   void test17()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(0, -1);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 0), sad::Point2D(0, 2),
                   sad::Point2D(2, 2), sad::Point2D(2, 0))	  
       );
       r2.setCenter(sad::p2d::Point(5,1));
       r2.setRadius(1);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 0);
    }

   void test18()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       sad::p2d::Vector v1(0, 0);
       sad::p2d::Vector v2(0, 0);
       r1.setRect(
            sad::Rect2D(sad::Point2D(0, 0), sad::Point2D(0, 2),
                   sad::Point2D(2, 2), sad::Point2D(2, 0))	  
       );
       r2.setCenter(sad::p2d::Point(5,1));
       r2.setRadius(1);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 0);
    }

} test_find_contact_points_rtoc;