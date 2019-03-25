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
 * Tests finding contact points between rectangle and circle
 */
struct TestFindContactPointsRtoCBug1 : tpunit::TestFixture
{
 public:
   TestFindContactPointsRtoCBug1() : tpunit::TestFixture(
        TEST(TestFindContactPointsRtoCBug1::test)	   	
    ) {}

   void test()
   {
       sad::p2d::Rectangle r1;
       sad::p2d::Circle r2;
       r1.setRect(
            sad::Rect2D(sad::Point2D(108, 190), sad::Point2D(192, 190),
                   sad::Point2D(192, 210), sad::Point2D(108, 210))	  
       );
       sad::p2d::Vector v1(0, 0);
       
       r2.setCenter(sad::p2d::Point(166.878,178.977));
       r2.setRadius(11);
       sad::p2d::Vector v2(20, 139.125);
       
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s2, v2, s1, v1);
       ASSERT_TRUE( pairs.size() == 1);
   }

   

} test_find_contact_points_rtoc_bug1;