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



struct P2DVectorTest : tpunit::TestFixture
{
public:
    P2DVectorTest() : tpunit::TestFixture(
       TEST(P2DVectorTest::testCommon),
       TEST(P2DVectorTest::testClone),
       TEST(P2DVectorTest::testTo3D),
       TEST(P2DVectorTest::testMath),
       TEST(P2DVectorTest::testToString),
       TEST(P2DVectorTest::testDistance),
       TEST(P2DVectorTest::testModulo),
       TEST(P2DVectorTest::testBasis),           
       TEST(P2DVectorTest::testUnit),
       TEST(P2DVectorTest::testScalar),
       TEST(P2DVectorTest::testOrtho)
    ) {}

    /*! A common test for exposed API
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testCommon()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.p2d.Vector(); var c = new sad.p2d.Vector(2,3); b.x = 5; b.y = 6; b.x + b.y + c.x + c.y", false, &error);
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
        bool eval_result = ctx.eval("var b = new sad.p2d.Vector(3,4); var c = b.clone(); b.x = 34; c.x", false, &error);
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
        bool eval_result = ctx.eval("var b = new sad.p2d.Vector(3,4); b.to3d().x", false, &error);
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
            bool eval_result = ctx.eval("var b = new sad.p2d.Vector(3,4); b.plus(b).x", false, &error);
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
            bool eval_result = ctx.eval("var b = new sad.p2d.Vector(3,4); b.minus(b).x", false, &error);
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
            bool eval_result = ctx.eval("var b = new sad.p2d.Vector(3,4); b.multiply(b).x", false, &error);
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
            bool eval_result = ctx.eval("var b = new sad.p2d.Vector(3,4); b.divide(b).x", false, &error);
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
        bool eval_result = ctx.eval("var b = new sad.p2d.Vector(); b.toString()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<sad::String> result = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == "sad::p2d::Vector(0,0)");        
    }

    /*! A common test for distance
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testDistance()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.p2d.Vector(3,4); var c = new sad.p2d.Vector(6,8); b.distance(c)", false, &error);
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


    /*! A common test for modulo
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testModulo()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.p2d.Vector(3,4); sad.p2d.modulo(b)", false, &error);
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

    /*! A common test for basis
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testBasis()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.p2d.Vector(3,4); sad.p2d.basis().x", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<double> result = ::dukpp03::GetValue<double, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( fabs(result.value()) > 0.0001 );          
    }

    /*! A common test for unit-vector
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testUnit()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.p2d.Vector(1,0); sad.p2d.unit(b).x", false, &error);
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

    /*! A common test for scalar
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testScalar()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.p2d.Vector(1,0); sad.p2d.scalar(b, b)", false, &error);
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

    /*! A common test for ortho
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testOrtho()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("var b = new sad.p2d.Vector(1,0); sad.p2d.ortho(b, sad.p2d.OVI_DEG_90).x", false, &error);
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

} _p2d_vector_test;