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

static double getX(const sad::Point3I& x)
{
    return x.x();
}

struct Point3ITest : tpunit::TestFixture
{
public:
    Point3ITest() : tpunit::TestFixture(
       TEST(Point3ITest::testCommon),
       TEST(Point3ITest::testClone),
       TEST(Point3ITest::testTo2D),
       TEST(Point3ITest::testMath),
       TEST(Point3ITest::testToString),
       TEST(Point3ITest::testDistance),
       TEST(Point3ITest::testAutoCast)              
    ) {}

    /*! A common test for exposes API
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testCommon()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Point3I(); var c = new sad.Point3I(2,3,4); b.x * c.x + b.y * c.y + b.z * c.z", false, &error);
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

    /*! A common test for exposed API
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testClone()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Point3I(3,4,3); var c = b.clone(); b.x = 34; c.x", false, &error);
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

    /*! A common test for .to2D
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testTo2D()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Point3I(3,4,0); b.to2d().x", false, &error);
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
            bool eval_result = ctx.eval("var b = new sad.Point3I(3,4,0); b.plus(b).x", false, &error);
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
            bool eval_result = ctx.eval("var b = new sad.Point3I(3,4,0); b.minus(b).x", false, &error);
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
            bool eval_result = ctx.eval("var b = new sad.Point3I(3,4,0); b.multiply(b).x", false, &error);
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
            bool eval_result = ctx.eval("var b = new sad.Point3I(3,4,0); b.divide(b).x", false, &error);
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
        bool eval_result = ctx.eval("var b = new sad.Point3I(); b.toString()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<sad::String> result = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == "sad::Point3I(0,0,0)");      
    }

    /*! A common test for distance
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testDistance()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.Point3I(3,4,0); var c = new sad.Point3I(6,8,0); b.distance(c)", false, &error);
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
            bool eval_result = ctx.eval("getX(new sad.Point2I(5, 4))", false, &error);
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
            bool eval_result = ctx.eval("getX({\"x\" : 5, \"y\" : 4, \"z\" : 0})", false, &error);
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

} _point3i_test;