#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include <3rdparty/tpunit++/tpunit++.hpp>
#include <sadvector.h>
#include <p2d/point.h>
#include <geometry2d.h>
#pragma warning(pop)

namespace sad
{

namespace p2d
{

sad::Vector<sad::p2d::Point> graham_scan(const sad::Vector<sad::p2d::Point> & set);

}

}
/*!
 * Tests graham find_min_point_on_y_axis function
 */
struct GrahamScanTest : tpunit::TestFixture
{
 public:
   GrahamScanTest() : tpunit::TestFixture(
       TEST(GrahamScanTest::testOnePoint),
       TEST(GrahamScanTest::testTwoPoints),
       TEST(GrahamScanTest::testThreePoints),
       TEST(GrahamScanTest::testLine),
       TEST(GrahamScanTest::testPseudoTriangle),
       TEST(GrahamScanTest::testRectMovingUp),
       TEST(GrahamScanTest::testRectMovingDown), 
       TEST(GrahamScanTest::testOctagonMovingUp)
   ) {}

   bool pointIsInSet(const sad::Vector<sad::p2d::Point> & set, const sad::Point2D & p)
   {
       for(unsigned int i = 0; i < set.size(); i++)
       {
           if (sad::equal(set[i], p))
               return true;
       }
       return false;
   }
   
   void testOnePoint()
   {
       sad::Vector<sad::p2d::Point> src;
       src << sad::p2d::Point(0, 0);
       sad::Vector<sad::p2d::Point> res  = sad::p2d::graham_scan(src);
       ASSERT_EQUAL( res.size(), 1 ); 
       ASSERT_TRUE( this->pointIsInSet(res, src[0]) );
   }

   void testTwoPoints()
   {
       sad::Vector<sad::p2d::Point> src;
       src << sad::p2d::Point(0, 0);
       src << sad::p2d::Point(2, 2);
       sad::Vector<sad::p2d::Point> res  = sad::p2d::graham_scan(src);
       ASSERT_EQUAL( res.size(), 2 ); 
       ASSERT_TRUE( this->pointIsInSet(res, src[0]) );
       ASSERT_TRUE( this->pointIsInSet(res, src[1]) );
   }

   void testThreePoints()
   {
       sad::Vector<sad::p2d::Point> src;
       src << sad::p2d::Point(0, 0);
       src << sad::p2d::Point(2, 2);
       src << sad::p2d::Point(4, 0);
       sad::Vector<sad::p2d::Point> res  = sad::p2d::graham_scan(src);
       ASSERT_EQUAL( res.size(), 3 );
       ASSERT_TRUE( this->pointIsInSet(res, src[0]) );
       ASSERT_TRUE( this->pointIsInSet(res, src[1]) );
       ASSERT_TRUE( this->pointIsInSet(res, src[2]) );
   }

   void testLine()
   {
       sad::Vector<sad::p2d::Point> src;
       src << sad::p2d::Point(0, 0);
       src << sad::p2d::Point(2, 2);
       src << sad::p2d::Point(4, 4);
       sad::Vector<sad::p2d::Point> res  = sad::p2d::graham_scan(src);
       ASSERT_EQUAL( res.size(), 2 );
       ASSERT_TRUE( this->pointIsInSet(res, src[0]) );
       ASSERT_TRUE( this->pointIsInSet(res, src[2]) );   
   }

   void testPseudoTriangle()
   {
       sad::Vector<sad::p2d::Point> src;
       src << sad::p2d::Point(5, 0);	
       src << sad::p2d::Point(0, 0);
       src << sad::p2d::Point(2, 2);
       src << sad::p2d::Point(2, 5);
       sad::Vector<sad::p2d::Point> res  = sad::p2d::graham_scan(src);   
       ASSERT_EQUAL( res.size(), 3 );
       ASSERT_TRUE( this->pointIsInSet(res, src[0]) );
       ASSERT_TRUE( this->pointIsInSet(res, src[1]) );
       ASSERT_TRUE( this->pointIsInSet(res, src[3]) );
   }

   void testRectMovingUp()
   {
       sad::Vector<sad::p2d::Point> src;
       src << sad::p2d::Point(0, 0);	
       src << sad::p2d::Point(2, 0);
       src << sad::p2d::Point(2, 2);
       src << sad::p2d::Point(0, 2);
       
       src << sad::p2d::Point(4, 4);
       src << sad::p2d::Point(6, 4);
       src << sad::p2d::Point(6, 6);
       src << sad::p2d::Point(4, 6);

       sad::Vector<sad::p2d::Point> res  = sad::p2d::graham_scan(src);   
       ASSERT_EQUAL( res.size(), 6 );
       ASSERT_TRUE( this->pointIsInSet(res, src[2]) == false );
       ASSERT_TRUE( this->pointIsInSet(res, src[4]) == false );
   }

   void testRectMovingDown()
   {
       sad::Vector<sad::p2d::Point> src;
       src << sad::p2d::Point(0, 6);	 // 0
       src << sad::p2d::Point(2, 6);  // 1
       src << sad::p2d::Point(2, 4);  // 2
       src << sad::p2d::Point(0, 4);  // 3
       
       src << sad::p2d::Point(3, 0);  // 4
       src << sad::p2d::Point(3, 2);  // 5
       src << sad::p2d::Point(5, 2);  // 6
       src << sad::p2d::Point(5, 0);  // 7

       sad::Vector<sad::p2d::Point> res  = sad::p2d::graham_scan(src);   
       ASSERT_EQUAL( res.size(), 6 );
       ASSERT_TRUE( this->pointIsInSet(res, src[2]) == false );
       ASSERT_TRUE( this->pointIsInSet(res, src[5]) == false );
   }

   void testOctagonMovingUp()
   {
       sad::Vector<sad::p2d::Point> src;
       src << sad::p2d::Point(1, 0);	 // 0
       src << sad::p2d::Point(2, 0);  // 1
       src << sad::p2d::Point(3, 1);  // 2
       src << sad::p2d::Point(3, 2);  // 3
       src << sad::p2d::Point(2, 3);  // 4
       src << sad::p2d::Point(1, 3);  // 5
       src << sad::p2d::Point(0, 2);  // 6
       src << sad::p2d::Point(0, 1);  // 7
       
       src << sad::p2d::Point(4, 3);  // 8
       src << sad::p2d::Point(5, 3);  // 9
       src << sad::p2d::Point(6, 4);  // 10
       src << sad::p2d::Point(6, 5);  // 11
       src << sad::p2d::Point(5, 6);  // 12
       src << sad::p2d::Point(4, 6);  // 13
       src << sad::p2d::Point(3, 5);  // 14
       src << sad::p2d::Point(3, 4);  // 15

       sad::Vector<sad::p2d::Point> res  = sad::p2d::graham_scan(src);	   
       ASSERT_EQUAL( res.size(), 8 );
       ASSERT_TRUE( this->pointIsInSet(res, src[2]) == false );
       ASSERT_TRUE( this->pointIsInSet(res, src[3]) == false );
       ASSERT_TRUE( this->pointIsInSet(res, src[4]) == false );
       ASSERT_TRUE( this->pointIsInSet(res, src[5]) == false );

       ASSERT_TRUE( this->pointIsInSet(res, src[8]) == false );
       ASSERT_TRUE( this->pointIsInSet(res, src[9]) == false );
       ASSERT_TRUE( this->pointIsInSet(res, src[14]) == false );
       ASSERT_TRUE( this->pointIsInSet(res, src[15]) == false );
   }


} _graham_scan_test;