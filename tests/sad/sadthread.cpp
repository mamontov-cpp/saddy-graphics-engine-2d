#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <sadthread.h>
#include <sadsleep.h>
#include <extra/geometry2d.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

bool vthreadvrunned = false;
void vthreadv()
{
	vthreadvrunned = true;
}

int vthreadi()
{
	return 1;
}

void vthreadav(bool f)
{
	vthreadvrunned = f;
}

int vthreadai(int f)
{
	return f;
}


/*!
 * Tests object logic
 */
struct sadThreadTest : tpunit::TestFixture
{
 public:
   sadThreadTest() : tpunit::TestFixture(
	   TEST(sadThreadTest::testConstructors)
   ) {}

   void threadmv()
   {
	   vthreadvrunned = true;
   }
   int threadmi()
   {
		return 1;
   }
   void threadmv1(bool flag)
   {
	   vthreadvrunned = flag;
   }
   int threadmi1(int flag)
   {
		return flag;
   }

   void sleepy()
   {
	   sad::sleep(2000);
   }

   /*! Tests all of constructors for thread
    */
   void testConstructors()
   {
	   {
	   sad::Thread thread;
	   thread.run();
	   ASSERT_TRUE( thread.exitCode() == 0 );
	   }
	   {
	   sad::Thread thread(vthreadv);
	   thread.run();
	   thread.wait(100);
	   ASSERT_TRUE( thread.exitCode() == 0 );
	   ASSERT_TRUE( vthreadvrunned == true );
	   }
	   {
	   sad::Thread thread(vthreadi);
	   thread.run();
	   ASSERT_TRUE( thread.exitCode() == 1 );
	   }
	   {
	   vthreadvrunned = false;
	   sad::Thread thread(vthreadav, true);
	   thread.run();
	   thread.wait(100);
	   ASSERT_TRUE( vthreadvrunned == true );
	   }
	   {
	   sad::Thread thread(vthreadai, 1);
	   thread.run();
	   ASSERT_TRUE( thread.exitCode() == 1 );
	   }
	   {
	   vthreadvrunned = false;
	   sad::Thread thread(this, &sadThreadTest::threadmv);
	   thread.run();
	   thread.wait(100);
	   ASSERT_TRUE( vthreadvrunned == true );
	   }
	   {
	   sad::Thread thread(this, &sadThreadTest::threadmi);
	   thread.run();
	   ASSERT_TRUE( thread.exitCode() == 1 );
	   }
	   {
	   vthreadvrunned = false;
	   sad::Thread thread(this, &sadThreadTest::threadmv1, true);
	   thread.run();
	   thread.wait(100);
	   ASSERT_TRUE( vthreadvrunned == true );
	   }
	   {
	   sad::Thread thread(this, &sadThreadTest::threadmi1, 1);
	   thread.run();
	   ASSERT_TRUE( thread.exitCode() == 1 );
	   }
   }
   /*! Tests termination of thread
    */
   void test_terminate()
   {
	  sad::Thread thread(this, &sadThreadTest::sleepy);
	  thread.run();
	  thread.stop();
	  ASSERT_TRUE( thread.exitCode() == sad::Thread::Cancelled );
   }

} sad_thread_test;