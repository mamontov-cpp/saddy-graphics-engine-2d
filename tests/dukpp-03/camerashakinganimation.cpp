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
        TEST(CameraShakingTest::testMinFrequency),
        TEST(CameraShakingTest::testMaxFrequency),
        TEST(CameraShakingTest::testPivot),
        TEST(CameraShakingTest::testLooped),
        TEST(CameraShakingTest::testTime),
        TEST(CameraShakingTest::testObjectName),
        TEST(CameraShakingTest::testMajorId),
        TEST(CameraShakingTest::testMinorId)
    ) {}
    
    /*! Test for getting and setting minAngle property
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testMinFrequency() 
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

    /*! Test for getting and setting maxAngle property
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testMaxFrequency() 
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.animations.CameraShaking(); b.setFrequency(255); b.frequency()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE(eval_result);
        ASSERT_TRUE(error.size() == 0);
        ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE(result.exists());
        ASSERT_TRUE(result.value() == 255);
    }

   
 
    /*! Test for getting and setting looped property
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testLooped() 
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.animations.CameraShaking(); b.setLooped(false); b.looped()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE(eval_result);
        ASSERT_TRUE(error.size() == 0);
        ::dukpp03::Maybe<bool> result = ::dukpp03::GetValue<bool, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE(result.exists());
        ASSERT_TRUE(result.value() == false);
    }
    

    /*! Test for getting and setting time property
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testTime() 
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.animations.CameraShaking(); b.setTime(4.0); b.time()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE(eval_result);
        ASSERT_TRUE(error.size() == 0);
        ::dukpp03::Maybe<double> result = ::dukpp03::GetValue<double, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE(result.exists());
        ASSERT_TRUE(result.value() == 4.0);
    }

     /*! Test for getting and setting objectName property
      */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testObjectName() 
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.animations.CameraShaking(); b.setObjectName(\"name\"); b.objectName()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE(eval_result);
        ASSERT_TRUE(error.size() == 0);
        ::dukpp03::Maybe<sad::String> result = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE(result.exists());
        ASSERT_TRUE(result.value() == "name");
    }

    /*! Test for getting and setting majorId property
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testMajorId() 
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.animations.CameraShaking(); b.MajorId=14; b.MajorId", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE(eval_result);
        ASSERT_TRUE(error.size() == 0);
        ::dukpp03::Maybe<long> result = ::dukpp03::GetValue<long, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE(result.exists());
        ASSERT_TRUE(result.value() == 14);
    }
    
    /*! Test for getting and setting minorId property
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testMinorId() 
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.animations.CameraShaking(); b.MinorId=11; b.MinorId", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE(eval_result);
        ASSERT_TRUE(error.size() == 0);
        ::dukpp03::Maybe<long> result = ::dukpp03::GetValue<long, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE(result.exists());
        ASSERT_TRUE(result.value() == 11);
    }
} _CameraShakinganimation_test;