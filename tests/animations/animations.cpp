#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


struct AnimationsTest : tpunit::TestFixture
{
 public:
   AnimationsTest() : tpunit::TestFixture(
	   TEST(AnimationsTest::testQueryInstancesByObject)
   ) {}
   

   void testQueryInstancesByObject()
   {
	   
   }

} _animations_test;

