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



struct Rect2DTest : tpunit::TestFixture
{
public:
    Rect2DTest() : tpunit::TestFixture(
       TEST(Rect2DTest::testCommon),
       TEST(Rect2DTest::testClone),
       TEST(Rect2DTest::testTo2I),
       TEST(Rect2DTest::testToString)
    ) {}

    /*! A common test for exposed API
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testCommon()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var a = new sad.Rect2D(0, 0, 8, 8); a.p0().x + a.p1().x + a.p2().x + a.p3().x + a.width() + a.height()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 32);      
    }

    /*! A common test for cloning
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testClone()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Rect2D(); var c = b.clone(); c.p0().x", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 0);      
    }

    /*! A common test for .to2I
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testTo2I()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Rect2D(); b.to2i().p0().x", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 0);      
    }

    
    /*! A common test for math
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testToString()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Rect2D(); b.toString()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<sad::String> result = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value().size() != 0);        
    }

} _rect2d_test;