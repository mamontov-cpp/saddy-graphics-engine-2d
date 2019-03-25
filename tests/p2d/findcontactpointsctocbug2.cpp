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
 * Tests finding contact points between two circles
 */
struct TestFindContactPointsCtoCBug2 : tpunit::TestFixture
{
 public:
   TestFindContactPointsCtoCBug2() : tpunit::TestFixture(
       TEST(TestFindContactPointsCtoCBug2::test)
   ) {}

   void test()
   {
       sad::p2d::Circle r1;
       sad::p2d::Circle r2;
       r1.setCenter(sad::p2d::Vector(400.001, 351.674));
       sad::p2d::Vector v1(0.000783688,6.45358);
       r2.setCenter(sad::p2d::Vector(434.148, 350.032));
       sad::p2d::Vector v2(-76.5746,-147.73);
       r1.setRadius(18);
       r2.setRadius(18);
       sad::p2d::FindContactPoints m;
       sad::p2d::CollisionShape * s1 = &r1;
       sad::p2d::CollisionShape * s2 = &r2;
       sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
       ASSERT_TRUE( pairs.size() == 1);

       double toi1 = (pairs[0].p1().x() - pairs[0].p2().x())  / (v2.x() - v1.x());
       double toi2 = (pairs[0].p1().y() - pairs[0].p2().y())  / (v2.y() - v1.y());
       ASSERT_FLOAT_NEAR(toi1, toi2, 1.0E-4);

       sad::p2d::Point p1 = pairs[0].p1() + v1 * toi1;
       sad::p2d::Point p2 = pairs[0].p2() + v2 * toi2;

       ASSERT_FLOAT_NEAR(p1.x(), p2.x(), 1.0E-4);
       ASSERT_FLOAT_NEAR(p1.y(), p2.y(), 1.0E-4);

       r1.move(v1 * toi1);
       r2.move(v2 * toi2);

       double distance = r1.center().distance(r2.center());
       ASSERT_FLOAT_NEAR(distance, 36.0, 1.0E-4);
   }

} test_find_contact_points_ctoc_bug2;