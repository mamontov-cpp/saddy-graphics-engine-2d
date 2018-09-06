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
struct TestFindContactPointsCtoCBug3 : tpunit::TestFixture
{
 public:
   TestFindContactPointsCtoCBug3() : tpunit::TestFixture(
       TEST(TestFindContactPointsCtoCBug3::test)
   ) {}

   void test()
   {
       // Reserved for other tests
   }

} test_find_contact_points_ctoc_bug3;