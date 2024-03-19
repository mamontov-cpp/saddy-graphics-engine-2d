#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include <pausabletimer.h>
#include <sadsleep.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)



/*!
 * Tests pausable timer logic
 */
struct SadPausableTimer : tpunit::TestFixture
{
public:
    SadPausableTimer() : tpunit::TestFixture(
        TEST(SadPausableTimer::test),
        TEST(SadPausableTimer::testMultiplePause),
        TEST(SadPausableTimer::testMultiplePauseMultipleResume)
    ) {}

    // ReSharper disable once CppMemberFunctionMayBeStatic
   void test()
   {
       sad::PausableTimer t;
       t.start();
       sad::sleep(200);
       t.pause();
       sad::sleep(200);
       t.resume();
       sad::sleep(200);
       const double before_stop = t.elapsed();
       t.stop();
       sad::sleep(200);	
       const double after_stop = t.elapsed();

       ASSERT_TRUE(before_stop >= 395.0 && before_stop <= 600.0)
       ASSERT_TRUE(after_stop >= 395 && after_stop <= 600.0)
   }

   // ReSharper disable once CppMemberFunctionMayBeStatic
    void testMultiplePause()
    {
        sad::PausableTimer t;
        t.start();
        sad::sleep(100);
        t.pause();
        sad::sleep(200);
        t.pause();
        sad::sleep(200);
        t.resume();
        sad::sleep(200);
        const double before_stop = t.elapsed();
        t.stop();
        sad::sleep(200);
        const double after_stop = t.elapsed();

        ASSERT_TRUE(before_stop >= 295.0 && before_stop <= 400.0)
        ASSERT_TRUE(after_stop >= 295.0 && after_stop <= 400.0)
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    void testMultiplePauseMultipleResume()
    {
        sad::PausableTimer t;
        t.start();
        sad::sleep(100);
        t.pause();
        sad::sleep(200);
        t.pause();
        sad::sleep(200);
        t.resume();
        sad::sleep(200);
        t.resume();
        sad::sleep(200);
        const double before_stop = t.elapsed();
        t.stop();
        sad::sleep(200);
        const double after_stop = t.elapsed();

        ASSERT_TRUE(before_stop >= 495.0 && before_stop <= 600.0);
        ASSERT_TRUE(after_stop >= 495.0 && after_stop <= 600.0);
    }

} _pausable_timer_test;
