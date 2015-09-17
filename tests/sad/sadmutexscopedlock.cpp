#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <sadthread.h>
#include <sadmutex.h>
#include <sadscopedlock.h>
#include <sadsleep.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)



/*!
 * Tests semaphore logic
 */
struct sadMutexScopedLockTest : tpunit::TestFixture
{
public:

    sadMutexScopedLockTest() : tpunit::TestFixture(
        TEST(sadMutexScopedLockTest::test)
    ) {}

    int value;

    sad::Mutex * mutex;
    
    int increment()
    {
       sad::ScopedLock lock(mutex);
       sad::sleep(100);
       ++value;
       return 0;
    }

   void test()
   {
       value = 0;
       mutex = new sad::Mutex();
       sad::Thread thread1(this, &sadMutexScopedLockTest::increment);
       sad::Thread thread2(this, &sadMutexScopedLockTest::increment);
       thread1.run();
       thread2.run();
       thread1.wait();
       thread2.wait();
       delete mutex;

       ASSERT_TRUE(value == 2);
   }

} sad_mutex_scoped_lock_test;
