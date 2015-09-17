#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#include <label.h>
#include <animations/animationsinstance.h>
#include <animations/animationsanimations.h>
#pragma warning(pop)


struct AnimationsTest : tpunit::TestFixture
{
 public:
   AnimationsTest() : tpunit::TestFixture(
       TEST(AnimationsTest::testQueryInstancesByObject)
   ) {}
   

   void testQueryInstancesByObject()
   {
        sad::Label a;
        sad::animations::Instance i1;
        sad::animations::Instance i2;
        a.addRef();
        i1.addRef();
        i2.addRef();
        i1.setObject(&a);
        i2.setObject(&a);

        sad::animations::Animations anims;
        anims.add(&i1);
        anims.add(&i2);
        sad::Vector<sad::animations::Process*> processes = anims.queryProcessesRelatedToObject(&a);
        ASSERT_TRUE( processes.size() == 2);

        processes = anims.queryProcessesRelatedToObject(NULL);
        ASSERT_TRUE( processes.size() == 0);
   }

} _animations_test;

