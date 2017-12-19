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

/*! Tests for Color class
*/
struct ColorAnimationTest : tpunit::TestFixture
{
public:
    ColorAnimationTest() : tpunit::TestFixture(
        TEST(ColorAnimationTest::testMinColor),
        TEST(ColorAnimationTest::testMaxColor),
        TEST(ColorAnimationTest::testLooped),
        TEST(ColorAnimationTest::testTime),
        TEST(ColorAnimationTest::testObjectName),
        TEST(ColorAnimationTest::testMajorId),
        TEST(ColorAnimationTest::testMinorId)
    ) {}
    
    /*! Test for getting and setting minColor property
    */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testMinColor() {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.animations.Color(); b.setMinColor(new sad.AColor(12,12,12)); b.minColor()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE(eval_result);
        ASSERT_TRUE(error.size() == 0);
        ::dukpp03::Maybe<sad::AColor> result = ::dukpp03::GetValue<sad::AColor, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE(result.exists());
        ASSERT_TRUE(result.value() == sad::AColor(12, 12, 12));
    }
    
    /*! Test for getting and setting maxColor property
    */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testMaxColor() {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.animations.Color(); b.setMaxColor(new sad.AColor(12,12,12)); b.maxColor()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE(eval_result);
        ASSERT_TRUE(error.size() == 0);
        ::dukpp03::Maybe<sad::AColor> result = ::dukpp03::GetValue<sad::AColor, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE(result.exists());
        ASSERT_TRUE(result.value() == sad::AColor(12, 12, 12));
    }
    
    /*! Test for getting and setting looped property
    */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testLooped() {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.animations.Color(); b.setLooped(false); b.looped()", false, &error);
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
        bool eval_result = ctx.eval("var b = new sad.animations.Color(); b.setTime(4.0); b.time()", false, &error);
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
    void testObjectName() {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.animations.Color(); b.setObjectName(\"name\"); b.objectName()", false, &error);
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
        bool eval_result = ctx.eval("var b = new sad.animations.Color(); b.MajorId=14; b.MajorId", false, &error);
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
        bool eval_result = ctx.eval("var b = new sad.animations.Color(); b.MinorId=11; b.MinorId", false, &error);
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
} _coloranimation_test;