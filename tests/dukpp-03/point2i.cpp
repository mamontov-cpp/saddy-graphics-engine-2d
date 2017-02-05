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


static double getX(const sad::Point2I& x)
{
    return x.x();
}

struct Point2ITest : tpunit::TestFixture
{
public:
    Point2ITest() : tpunit::TestFixture(
       TEST(Point2ITest::testCommon),
       TEST(Point2ITest::testClone),
       TEST(Point2ITest::testTo3D),
       TEST(Point2ITest::testMath),
       TEST(Point2ITest::testToString),
       TEST(Point2ITest::testDistance),
       TEST(Point2ITest::testAutoCast)           
    ) {}

    /*! A common test for exposes API
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testCommon()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Point2I(); var c = new sad.Point2I(2,3); b.x = 5; b.y = 6; b.x + b.y + c.x + c.y", false, &error);
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
        bool eval_result = ctx.eval("var b = new sad.Point2I(3,4); var c = b.clone(); b.x = 34; c.x", false, &error);
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

    /*! A common test for .to3D
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testTo3D()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Point2I(3,4); b.to3i().x", false, &error);
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
    void testMath()
    {
        {
            std::string error;
            sad::dukpp03::Context ctx;
            bool eval_result = ctx.eval("var b = new sad.Point2I(3,4); b.plus(b).x", false, &error);
            if (!eval_result)
            {
                printf("%s\n", error.c_str());
            }
            ASSERT_TRUE( eval_result );
            ASSERT_TRUE( error.size() == 0 );
            ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
            ASSERT_TRUE( result.exists() );
            ASSERT_TRUE( result.value() == 6);  
        }
        {
            std::string error;
            sad::dukpp03::Context ctx;
            bool eval_result = ctx.eval("var b = new sad.Point2I(3,4); b.minus(b).x", false, &error);
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
        {
            std::string error;
            sad::dukpp03::Context ctx;
            bool eval_result = ctx.eval("var b = new sad.Point2I(3,4); b.multiply(b).x", false, &error);
            if (!eval_result)
            {
                printf("%s\n", error.c_str());
            }
            ASSERT_TRUE( eval_result );
            ASSERT_TRUE( error.size() == 0 );
            ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
            ASSERT_TRUE( result.exists() );
            ASSERT_TRUE( result.value() == 9);  
        }
        {
            std::string error;
            sad::dukpp03::Context ctx;
            bool eval_result = ctx.eval("var b = new sad.Point2I(3,4); b.divide(b).x", false, &error);
            if (!eval_result)
            {
                printf("%s\n", error.c_str());
            }
            ASSERT_TRUE( eval_result );
            ASSERT_TRUE( error.size() == 0 );
            ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
            ASSERT_TRUE( result.exists() );
            ASSERT_TRUE( result.value() == 1);  
        }
    }

    /*! A common test for math
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testToString()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Point2I(); b.toString()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<sad::String> result = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == "sad::Point2I(0,0)");        
    }

    /*! A common test for distance
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testDistance()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Point2I(3,4); var c = new sad.Point2I(6,8); b.distance(c)", false, &error);
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


    /*! A an automatical cast test
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testAutoCast()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        ctx.registerCallable("getX", sad::dukpp03::make_function::from(getX));

        {
            bool eval_result = ctx.eval("getX(new sad.Point3I(5, 4, 0))", false, &error);
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
            bool eval_result = ctx.eval("getX({\"x\" : 5, \"y\" : 4})", false, &error);
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

} _point2i_test;