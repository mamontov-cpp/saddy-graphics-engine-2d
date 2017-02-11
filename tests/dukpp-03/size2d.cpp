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


static double getWidth(const sad::Size2D& x)
{
    return x.Width;
}

struct Size2DTest : tpunit::TestFixture
{
public:
    Size2DTest() : tpunit::TestFixture(
       TEST(Size2DTest::testCommon),
       TEST(Size2DTest::testClone),
       TEST(Size2DTest::testTo2I),
       TEST(Size2DTest::testToString),
       TEST(Size2DTest::testAutoCast)           
    ) {}

    /*! A common test for exposed API
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testCommon()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Size2D(); var c = new sad.Size2D(2,3); b.width = 5; b.height = 6; b.width + b.height + c.width + c.height", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 16);      
    }

    /*! A common test for cloning
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testClone()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Size2D(3,4); var c = b.clone(); b.width = 34; c.width", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 3);      
    }

    /*! A common test for .to2i
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testTo2I()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Size2D(3,4); b.to2i().width", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 3);      
    }

    
    /*! A common test for math
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testToString()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Size2D(); b.toString()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<sad::String> result = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == "sad::Size2D(0,0)");        
    }

    /*! A an automatical cast test
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testAutoCast()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        ctx.registerCallable("getWidth", sad::dukpp03::make_function::from(getWidth));

        {
            bool eval_result = ctx.eval("getWidth(new sad.Size2I(5, 4))", false, &error);
            if (!eval_result)
            {
                printf("%s\n", error.c_str());
            }
            ASSERT_TRUE( eval_result );
            ASSERT_TRUE( error.size() == 0 );
            ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
            ASSERT_TRUE( result.exists() );
            ASSERT_TRUE( result.value() == 5);  
        }

        {
            bool eval_result = ctx.eval("getWidth({\"width\" : 5, \"height\" : 4})", false, &error);
            if (!eval_result)
            {
                printf("%s\n", error.c_str());
            }
            ASSERT_TRUE( eval_result );
            ASSERT_TRUE( error.size() == 0 );
            ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
            ASSERT_TRUE( result.exists() );
            ASSERT_TRUE( result.value() == 5);  
        }
    }

} _size2d_test;