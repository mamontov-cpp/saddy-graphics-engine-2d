#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <sadthread.h>
#include <sadsleep.h>
#include <geometry2d.h>
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
       TEST(sadThreadTest::testConstructors),
       TEST(sadThreadTest::testTerminate)
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

   int sleepy()
   {
       int i =  0, j = 0, k = 0;
       int r = 0;
       for(k = 0; k < 200000; k++)
       {
           for(j = 0; j < 200000; j++)
           {
               i = 0;
               while (i < 20000000)
               {
                   i++;
                   long long  large_value = j * static_cast<long long>(k / 200);
                   r += static_cast<int>(i + large_value);
               }
           }
       }
       return r;
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
       std::function<void()> vthreadstdv = vthreadv;
       sad::Thread thread(vthreadstdv);
       thread.run();
       thread.wait(100);
       ASSERT_TRUE( thread.exitCode() == 0 );
       ASSERT_TRUE( vthreadvrunned == true );
       }
       {
       std::function<int()> vthreadstdi = vthreadi;
       sad::Thread thread(vthreadstdi);
       thread.run();
       ASSERT_TRUE( thread.exitCode() == 1 );
       }
       {
       vthreadvrunned = false;
       sad::Thread thread(vthreadav, true);
       thread.run();
       thread.wait(2000);
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
   void testTerminate()
   {
      sad::Thread thread(this, &sadThreadTest::sleepy);
      thread.run();
      sad::sleep(100);
      thread.stop();
      ASSERT_TRUE( thread.exitCode() == sad::Thread::Cancelled );
   }

} sad_thread_test;
