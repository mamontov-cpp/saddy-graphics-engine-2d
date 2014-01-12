#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <sadthread.h>
#include <sadsemaphore.h>
#include <sadsleep.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)



/*!
 * Tests semaphore logic
 */
struct sadSemaphoreTest : tpunit::TestFixture
{
public:

	sadSemaphoreTest() : tpunit::TestFixture(
		TEST(sadSemaphoreTest::test)
	) {}

	int value;

	sad::Semaphore * semaphore;
	
	void increment()
	{
	   semaphore->consume(1);
	   sad::sleep(100);
	   ++value;
	   semaphore->release(1);
   }

   void test()
   {
	   value = 0;
	   semaphore = new sad::Semaphore(1);
	   sad::Thread thread1(this, &sadSemaphoreTest::increment);
	   sad::Thread thread2(this, &sadSemaphoreTest::increment);
	   thread1.run();
	   thread2.run();
	   thread1.wait();
	   thread2.wait();
	   ASSERT_TRUE(semaphore->value() == 1);
	   delete semaphore;

	   ASSERT_TRUE(value == 2);
   }

} sad_semaphore_test;
