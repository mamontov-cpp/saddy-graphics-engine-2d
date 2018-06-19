#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdio>
#include "dukpp-03/context.h"
#include "sprite2d.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

/*! Tests for CameraShaking class
 */
struct CameraShakingTest : tpunit::TestFixture
{
public:
    CameraShakingTest() : tpunit::TestFixture(
        TEST(CameraShakingTest::testFrequency),
        TEST(CameraShakingTest::testOffset)
    ) {}
    
    /*! Test for getting and setting frequency property
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testFrequency() 
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.animations.CameraShaking(); b.setFrequency(1); b.frequency()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE(eval_result);
        ASSERT_TRUE(error.size() == 0);
        ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE(result.exists());
        ASSERT_TRUE(result.value() == 1);
    }
    /*! Test for getting and setting frequency property
    */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testOffset()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.animations.CameraShaking(); b.setOffset(new sad.Point2D(1,1)); b.offset().x", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE(eval_result);
        ASSERT_TRUE(error.size() == 0);
        ::dukpp03::Maybe<double> result = ::dukpp03::GetValue<double, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE(result.exists());
        ASSERT_TRUE(result.value() == 1);
    }

   
 
} _camera_shaking_test;