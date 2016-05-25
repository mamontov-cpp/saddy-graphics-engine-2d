#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <sadthread.h>
#include <sadmutex.h>
#include <sadsleep.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)



/*!
 * Tests semaphore logic
 */
struct sadMutexTest : tpunit::TestFixture
{
public:

    sadMutexTest() : tpunit::TestFixture(
        TEST(sadMutexTest::test)
    ) {}

   void test()
   {
       sad::Mutex m;
        // There should be no errors with multiple locking and unlocking mutex
       m.lock();
       m.lock();

       m.unlock();
       m.unlock();
   }

} _sad_mutex_test;
