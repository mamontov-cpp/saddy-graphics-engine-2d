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
        TEST(AnimationAnimationTest::testObjectName)
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
} _animationanimation_test;