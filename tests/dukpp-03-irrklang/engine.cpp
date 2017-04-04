#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdio>
#include "dukpp-03/context.h"
#include "dukpp-03-irrklang/dukpp-03-irrklang.h"
#include "sprite2d.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


struct EngineTest : tpunit::TestFixture
{
public:
    EngineTest() : tpunit::TestFixture(
       TEST(EngineTest::test)          
    ) {}

    /*! A common test for exposed API
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void test()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        sad::dukpp03irrklang::init(&ctx);
        bool eval_result = ctx.eval("sad.irrklang.Engine.ref()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );

        eval_result = ctx.eval("sad.irrklang.Engine.eref()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );

        eval_result = ctx.eval("sad.irrklang.Engine.ref().get()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );

        eval_result = ctx.eval("sad.irrklang.Engine.ref().tryLoad(\"1.ogg\")", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
    }

} _engine_test;