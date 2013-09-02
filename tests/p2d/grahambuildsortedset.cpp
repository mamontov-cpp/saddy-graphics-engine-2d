#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <3rdparty/tpunit++/tpunit++.hpp>
#include <templates/hlvector.hpp>
#include <p2d/point.h>
#include <extra/geometry2d.h>
#pragma warning(pop)

namespace p2d
{
hst::vector<p2d::Point> build_sorted_set(const hst::vector<p2d::Point> & set, int min_index);
}


/*!
 * Tests graham find_min_point_on_y_axis function
 */
struct GrahamBuildSortedSet : tpunit::TestFixture
{
 public:
   GrahamBuildSortedSet() : tpunit::TestFixture(
	   TEST(GrahamBuildSortedSet::test1)
   ) {}
   
   void test1()
   {
	   hst::vector<p2d::Point> p;
	   p << p2d::Point(1, 0);
	   p << p2d::Point(0, 0);
	   p << p2d::Point(0, 1);
	   p << p2d::Point(1, 1);
	   hst::vector<p2d::Point> r  = p2d::build_sorted_set(p, 1);
	   ASSERT_TRUE( equal(r[0], p[1]) );
	   ASSERT_TRUE( equal(r[1], p[0]) );
	   ASSERT_TRUE( equal(r[2], p[3]) );
	   ASSERT_TRUE( equal(r[3], p[2]) );
   }


} test6;