#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdio>
#include "dukpp-03/context.h"
#include "animations/animationscolor.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

/*! Tests for Animation class
*/
struct AnimationAnimationTest : tpunit::TestFixture
{
public:
    AnimationAnimationTest() : tpunit::TestFixture(
        TEST(AnimationAnimationTest::testObjectName),
        TEST(AnimationAnimationTest::testMajorId),
        TEST(AnimationAnimationTest::testMinorId),
        TEST(AnimationAnimationTest::testLooped),
        TEST(AnimationAnimationTest::testTime),
        TEST(AnimationAnimationTest::testValid)
    ) {}
    
    /*! Test for getting and setting objectName property
    */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testObjectName() {
        std::string error;
        sad::dukpp03::Context ctx;
        ctx.registerGlobal("animationObject", static_cast <sad::animations::Animation*> (new sad::animations::Color()));
        bool eval_result = ctx.eval("animationObject.setObjectName(\"name\"); animationObject.objectName()", false, &error);
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
    void testMajorId() {
        std::string error;
        sad::dukpp03::Context ctx;
        ctx.registerGlobal("animationObject", static_cast <sad::animations::Animation*> (new sad::animations::Color()));
        bool eval_result = ctx.eval("animationObject.MajorId=14; animationObject.MajorId", false, &error);
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
    void testMinorId() {
        std::string error;
        sad::dukpp03::Context ctx;
        ctx.registerGlobal("animationObject", static_cast <sad::animations::Animation*> (new sad::animations::Color()));
        bool eval_result = ctx.eval("animationObject.MinorId=11; animationObject.MinorId", false, &error);
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
    
    /*! Test for getting and setting looped property
    */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testLooped() {
        std::string error;
        sad::dukpp03::Context ctx;
        ctx.registerGlobal("animationObject", static_cast <sad::animations::Animation*> (new sad::animations::Color()));
        bool eval_result = ctx.eval("animationObject.setLooped(false); animationObject.looped()", false, &error);
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
    void testTime() {
        std::string error;
        sad::dukpp03::Context ctx;
        ctx.registerGlobal("animationObject", static_cast <sad::animations::Animation*> (new sad::animations::Color()));
        bool eval_result = ctx.eval("animationObject.setTime(4.0); animationObject.time()", false, &error);
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

    /*! Test for valid property
    */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testValid() {
        std::string error;
        sad::dukpp03::Context ctx;
        ctx.registerGlobal("animationObject", static_cast <sad::animations::Animation*> (new sad::animations::Color()));
        bool eval_result = ctx.eval("animationObject.valid()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE(eval_result);
        ASSERT_TRUE(error.size() == 0);
        ::dukpp03::Maybe<bool> result = ::dukpp03::GetValue<bool, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE(result.exists());
        ASSERT_TRUE(result.value() == true);
    }
} _animationanimation_test;