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

sad::Vector<sad::p2d::Point> build_sorted_set(const sad::Vector<sad::p2d::Point> & set, int min_index);

}

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
       sad::Vector<sad::p2d::Point> p;
       p << sad::p2d::Point(1, 0);
       p << sad::p2d::Point(0, 0);
       p << sad::p2d::Point(0, 1);
       p << sad::p2d::Point(1, 1);
       sad::Vector<sad::p2d::Point> r  = sad::p2d::build_sorted_set(p, 1);
       ASSERT_TRUE( sad::equal(r[0], p[1]) );
       ASSERT_TRUE( sad::equal(r[1], p[0]) );
       ASSERT_TRUE( sad::equal(r[2], p[3]) );
       ASSERT_TRUE( sad::equal(r[3], p[2]) );
   }


} _test_graham_build_sorted_set;