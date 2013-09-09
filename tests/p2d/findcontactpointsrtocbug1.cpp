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
struct TestFindContactPointsRtoCBug1 : tpunit::TestFixture
{
 public:
   TestFindContactPointsRtoCBug1() : tpunit::TestFixture(
	    TEST(TestFindContactPointsRtoCBug1::test)	   	
	) {}

   void test()
   {
	   p2d::Rectangle r1;
	   p2d::Circle r2;
	   r1.setRect(
			hRectF(hPointF(108, 190), hPointF(192, 190),
				   hPointF(192, 210), hPointF(108, 210))	  
	   );
	   p2d::Vector v1(0, 0);
	   
	   r2.setCenter(p2d::Point(166.878,178.977));
	   r2.setRadius(11);
	   p2d::Vector v2(20, 139.125);
	   
	   p2d::FindContactPoints m;
	   p2d::CollisionShape * s1 = &r1;
	   p2d::CollisionShape * s2 = &r2;
	   p2d::SetOfPointsPair pairs = m.invoke(s2, v2, s1, v1);
	   ASSERT_TRUE( pairs.size() == 1);
   }

   

} test_find_contact_points_rtoc_bug1;