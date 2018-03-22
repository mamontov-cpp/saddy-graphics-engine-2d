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

/*! Tests for CameraRotation class
*/
struct CameraRotationAnimationTest : tpunit::TestFixture
{
public:
    CameraRotationAnimationTest() : tpunit::TestFixture(
        TEST(CameraRotationAnimationTest::testMinAngle),
        TEST(CameraRotationAnimationTest::testMaxAngle),
        TEST(CameraRotationAnimationTest::testPivot),
        TEST(CameraRotationAnimationTest::testLooped),
        TEST(CameraRotationAnimationTest::testTime),
        TEST(CameraRotationAnimationTest::testObjectName),
        TEST(CameraRotationAnimationTest::testMajorId),
        TEST(CameraRotationAnimationTest::testMinorId)
    ) {}
    
    /*! Test for getting and setting minAngle property
    */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testMinAngle() {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.animations.CameraRotation(); b.setMinAngle(1); b.minAngle()", false, &error);
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
    