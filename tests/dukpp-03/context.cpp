#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdio>
#include "dukpp-03//context.h"
#include "sadpoint.h"
#include "db/save.h"
#include "fuzzyequal.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


class MockCallable: public ::dukpp03::FunctionCallable<sad::dukpp03::BasicContext>
{
public:
    MockCallable()
    {
        
    }

    ::dukpp03::Callable<sad::dukpp03::BasicContext>* clone()
    {
        return new MockCallable();
    }

    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 0;
    }

    virtual std::pair<int, bool> canBeCalled(sad::dukpp03::BasicContext* c) 
    { 
       int required_args = this->requiredArguments(); 
       if (c->getTop() != required_args)
       {
           return std::make_pair(-1, false);
       }
       return std::make_pair(0, true);
    }

    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::dukpp03::BasicContext* c)
    {
        ::dukpp03::PushValue<int, sad::dukpp03::BasicContext>::perform(c, 1);
        return 1;
    }

};


void print_something()
{
    printf("print_something(): zero argments \n");
}

void print_number_1(int a)
{
    printf("print_number_1: %d\n", a);  
}

void print_number_3(int a, int b, int c)
{
    printf("print_number_3: %d %d %d\n", a, b, c ); 
}

int return_something()
{
    return 32;
}


int return_number_1(int a)
{
    return a;
}

int return_number_3(int a, int b, int c)
{
    return a - b - c;
}

int return_number_3_decay(const int& a, int& b, int c)
{
    return a - b - c;
}

struct ContextTest : tpunit::TestFixture
{
public:
    ContextTest() : tpunit::TestFixture(
       TEST(ContextTest::testInitGet),
       TEST(ContextTest::testPushGet),
       TEST(ContextTest::testEvalNormal),
       TEST(ContextTest::testEvalFail),
       TEST(ContextTest::testEvalTimeout)/*,
       TEST(ContextTest::testEvalFromFileNormal),
       TEST(ContextTest::testEvalFromFileFail),
       TEST(ContextTest::testClean),
       TEST(ContextTest::testReset),
       TEST(ContextTest::testThrow),
       TEST(ContextTest::testRegisterGlobal),
       TEST(ContextTest::testRegisterCallable),
       TEST(ContextTest::testRegisterVoidFunctions),
       TEST(ContextTest::testRegisterReturnFunctions),
       TEST(ContextTest::testMethods),
       TEST(ContextTest::testPtrMethods)*/
    ) {}


    /*! Tests getting and setting reference data
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testInitGet()
    {
        sad::dukpp03::Context ctx;
        ASSERT_TRUE( sad::dukpp03::Context::getContext(ctx.context()) == &ctx );
        ASSERT_TRUE( sad::dukpp03::Context::getContext(ctx.context()) == &ctx );
    }

    /*! Tests pushing and getting values
     */
    void testPushGet()
    {
        sad::dukpp03::Context ctx;

        // Common case
        int test_number = 0;
        {
            sad::Point2D pts2d(3, 4);
            ::dukpp03::PushValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(&ctx, pts2d);
            ::dukpp03::Maybe<sad::Point2D> mbpts2d =
                ::dukpp03::GetValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( mbpts2d.exists() );
            ASSERT_TRUE( sad::is_fuzzy_equal(mbpts2d.value().x(), 3));
            ASSERT_TRUE( sad::is_fuzzy_equal(mbpts2d.value().y(), 4));
        }

        // char
        {
            char c = 121;
            ::dukpp03::PushValue<char, sad::dukpp03::BasicContext>::perform(&ctx, c);
            ::dukpp03::Maybe<char> maybev = ::dukpp03::GetValue<char, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( maybev.value() == 121);
        }

        // unsigned char
        {
            unsigned char c = 121;
            ::dukpp03::PushValue<unsigned char, sad::dukpp03::BasicContext>::perform(&ctx, c);
            ::dukpp03::Maybe<unsigned char> maybev = ::dukpp03::GetValue<unsigned char, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( maybev.value() == 121);
        }

        // int
        {
            int c = 121;
            ::dukpp03::PushValue<int, sad::dukpp03::BasicContext>::perform(&ctx, c);
            ::dukpp03::Maybe<int> maybev = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( maybev.value() == 121);
        }

        // unsigned int
        {
            unsigned int c = 121;
            ::dukpp03::PushValue<unsigned int, sad::dukpp03::BasicContext>::perform(&ctx, c);
            ::dukpp03::Maybe<unsigned int> maybev = ::dukpp03::GetValue<unsigned int, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( maybev.value() == 121);
        }

        // long
        {
            long c = 121;
            ::dukpp03::PushValue<long, sad::dukpp03::BasicContext>::perform(&ctx, c);
            ::dukpp03::Maybe<long> maybev = ::dukpp03::GetValue<long, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( maybev.value() == 121);
        }

        // unsigned long
        {
            unsigned long c = 121;
            ::dukpp03::PushValue<unsigned long, sad::dukpp03::BasicContext>::perform(&ctx, c);
            ::dukpp03::Maybe<unsigned long> maybev = ::dukpp03::GetValue<unsigned long, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( maybev.value() == 121);
        }

        // long long
        {
            long long c = 121;
            ::dukpp03::PushValue<long long, sad::dukpp03::BasicContext>::perform(&ctx, c);
            ::dukpp03::Maybe<long long> maybev = ::dukpp03::GetValue<long long, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( maybev.value() == 121);
        }

        // unsigned long
        {
            unsigned long long c = 121;
            ::dukpp03::PushValue<unsigned long long, sad::dukpp03::BasicContext>::perform(&ctx, c);
            ::dukpp03::Maybe<unsigned long long> maybev = ::dukpp03::GetValue<unsigned long long, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( maybev.value() == 121);
        }

        // bool
        {
            bool c = false;
            ::dukpp03::PushValue<bool, sad::dukpp03::BasicContext>::perform(&ctx, c);
            ::dukpp03::Maybe<bool> maybev = ::dukpp03::GetValue<bool, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( maybev.value() == false);
        }

        // float
        {
            float c = 1.5;
            ::dukpp03::PushValue<float, sad::dukpp03::BasicContext>::perform(&ctx, c);
            ::dukpp03::Maybe<float> maybev = ::dukpp03::GetValue<float, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( sad::is_fuzzy_equal(maybev.value(), c));
        }

        // double
        {
            double c = 1.5;
            ::dukpp03::PushValue<double, sad::dukpp03::BasicContext>::perform(&ctx, c);
            ::dukpp03::Maybe<double> maybev = ::dukpp03::GetValue<double, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( sad::is_fuzzy_equal(maybev.value(), c));
        }

        // long double
        {
            long double c = 1.5;
            ::dukpp03::PushValue<long double, sad::dukpp03::BasicContext>::perform(&ctx, c);
            ::dukpp03::Maybe<long double> maybev = ::dukpp03::GetValue<long double, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( sad::is_fuzzy_equal(maybev.value(), c));
        }

        // const char*
        {
            const char* c = "22";
            ::dukpp03::PushValue<const char*, sad::dukpp03::BasicContext>::perform(&ctx, c);
            ::dukpp03::Maybe<const char*> maybev = ::dukpp03::GetValue<const char*, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( sad::String(maybev.value()) == "22");
        }

        // sad::String
        {
            const char* c = "23";
            ::dukpp03::PushValue<sad::String, sad::dukpp03::BasicContext>::perform(&ctx, c);
            ::dukpp03::Maybe<sad::String> maybev = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(&ctx, test_number++);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( maybev.value() == "23");
        }
    }

    /*! Test for normal evaluation process
     */
    void testEvalNormal()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("1 + 1", false, &error);
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 2);
    }
    /*! Test for non-compilable code
     */
    void testEvalFail()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.eval("1 + ;", true, &error);
        ASSERT_TRUE( !eval_result );
        ASSERT_TRUE( error.size() != 0 );
    }
    /*! Test for timeout
     */
    void testEvalTimeout()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        ctx.setMaximumExecutionTime(5000);
        bool eval_result = ctx.eval("while(true) {}", true, &error);
        ASSERT_TRUE( !eval_result );
        ASSERT_TRUE( error.size() != 0 );
        printf("Passed first stage\n");
        eval_result = ctx.eval("while(true) {}", true, &error);
        ASSERT_TRUE( !eval_result );
        ASSERT_TRUE( error.size() != 0 );
        printf("Passed second stage\n");
    }
    /*! Tests evaluation from file which just sums two numbers
     */
    /*void testEvalFromFileNormal()
    {
        sad::String error;
        sad::duktape::Context ctx;
        bool eval_result = ctx.evalFromFile("tests/duktape/common.js", false, &error);
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        sad::Maybe<int> result = sad::duktape::GetValue<int>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 2);
    }*/
    /*! Tests evaluation from file on non-existing file
     */
    /*void testEvalFromFileFail()
    {
        sad::String error;
        sad::duktape::Context ctx;
        bool eval_result = ctx.evalFromFile("tests/duktape/notexists.js", false, &error);
        ASSERT_TRUE( !eval_result );
        ASSERT_TRUE( error.size() != 0 );
    }*/
    /*! Test cleaning  of a pool
     */
    /*void testClean()
    {
        sad::duktape::Context ctx;
        sad::Point2D pts2d(3, 4);
        sad::duktape::PushValue<sad::Point2D>::perform(&ctx, pts2d, false);
        sad::Maybe<sad::Point2D> mbpts2d =
            sad::duktape::GetValue<sad::Point2D>::perform(&ctx, -1);
        ASSERT_TRUE( mbpts2d.exists() );
            
        ctx.clean();

        mbpts2d = sad::duktape::GetValue<sad::Point2D>::perform(&ctx, -1);
        ASSERT_TRUE( mbpts2d.exists() == false );       
    }*/
    /*! Test cleaning both pools and full reset of context
     */
    /*void testReset()
    {
        sad::duktape::Context ctx;
        sad::Point2D pts2d(3, 4);
        sad::duktape::PushValue<sad::Point2D>::perform(&ctx, pts2d, true);
        sad::Maybe<sad::Point2D> mbpts2d =
            sad::duktape::GetValue<sad::Point2D>::perform(&ctx, -1);
        ASSERT_TRUE( mbpts2d.exists() );
            
        ctx.reset();

        mbpts2d = sad::duktape::GetValue<sad::Point2D>::perform(&ctx, -1);
        ASSERT_TRUE( mbpts2d.exists() == false );   
        ASSERT_TRUE( sad::duktape::Context::getContext(ctx.context()) == &ctx );
    }*/
    /*! Tests throwing for object
     */
    /*void testThrow()
    {
        sad::duktape::Context ctx;
        ctx.throwError("Generic Error!");
        const char* s = duk_to_string(ctx.context(), -1);
        ASSERT_TRUE( s != NULL );
        sad::String testvalue = s;
        ASSERT_TRUE(  testvalue.size() !=0 );
    }*/
    /*! Tests registering value as property of global object
     */
    /*void testRegisterGlobal()
    {
        sad::duktape::Context ctx;
        ctx.registerGlobal("value", true);
        bool eval_result = ctx.eval(" !value ", false);
        ASSERT_TRUE( eval_result );
        sad::Maybe<bool> result = sad::duktape::GetValue<bool>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == false );
    }*/
    /*! Tests registering callable function
     */
    /*void testRegisterCallable()
    {
        sad::duktape::Context ctx;
        ctx.registerCallable("f", new MockCallable());
        bool eval_result = ctx.eval(" (f() + f()) * (f() + f()) ; f() + f()*f() ", false);
        ASSERT_TRUE( eval_result );
        sad::Maybe<int> result = sad::duktape::GetValue<int>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 2 );
    }*/
    /*! Tests registering functions
     */
    /*void testRegisterVoidFunctions()
    {
        sad::String error;  

        sad::duktape::Context ctx;
        sad::duktape::register_callable(&ctx, "f00", print_something);
        bool eval_result = ctx.eval(" f00(); f00(); ", true, &error);
        ASSERT_TRUE( eval_result );

        sad::duktape::register_callable(&ctx, "f01", print_number_1);
        eval_result = ctx.eval(" f01(21); f01(32); ", true);
        ASSERT_TRUE( eval_result );

        sad::duktape::register_callable(&ctx, "f03", print_number_3);
        eval_result = ctx.eval(" f03(21, 44, 56); f03(32, 88, 93); ", true);
        ASSERT_TRUE( eval_result );

        
        eval_result = ctx.eval(" f03(21) ", true, &error);
        ASSERT_TRUE( !eval_result );

        eval_result = ctx.eval(" f03(undefined, undefined, undefined) ", true, &error);
        ASSERT_TRUE( !eval_result );
    }

    void testRegisterReturnFunctions()
    {
        sad::String error;  

        sad::duktape::Context ctx;
        sad::duktape::register_callable(&ctx, "f00", return_something);
        bool eval_result = ctx.eval(" f00(); f00(); ", false, &error);
        ASSERT_TRUE( eval_result );
        sad::Maybe<int> result = sad::duktape::GetValue<int>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 32 );

        sad::duktape::register_callable(&ctx, "f01", return_number_1);
        eval_result = ctx.eval(" f01(21); f01(32); ", false);
        ASSERT_TRUE( eval_result );
        result = sad::duktape::GetValue<int>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 32 );

        sad::duktape::register_callable(&ctx, "f03", return_number_3);
        eval_result = ctx.eval(" f03(21, 44, 56); f03(32, 88, 93); ", false);
        ASSERT_TRUE( eval_result );
        result = sad::duktape::GetValue<int>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 32 - 88 - 93 );

        sad::duktape::register_callable(&ctx, "f04", return_number_3_decay);
        eval_result = ctx.eval(" f04(21, 44, 56); f04(32, 88, 93); ", false);
        ASSERT_TRUE( eval_result );
        result = sad::duktape::GetValue<int>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 32 - 88 - 93 );

        
        eval_result = ctx.eval(" f03(21) ", false, &error);
        ASSERT_TRUE( !eval_result );

        eval_result = ctx.eval(" f03(undefined, undefined, undefined) ", false, &error);
        ASSERT_TRUE( !eval_result );        
    }

    void testMethods()
    {
        sad::String error;  
        
        sad::duktape::Context ctx;
        sad::duktape::register_constructor<sad::Point2D, double, double>(&ctx, "pnt");
        sad::duktape::register_callable(&ctx, "x", &sad::Point2D::x);
        sad::duktape::register_callable(&ctx, "y", &sad::Point2D::y);

        sad::duktape::register_callable(&ctx, "setX", &sad::Point2D::setX);
        sad::duktape::register_callable(&ctx, "setY", &sad::Point2D::setY);

        bool eval_result = ctx.eval(" var f = pnt(3, 4);  setX(f, 55); x(f) ", false);
        ASSERT_TRUE( eval_result );
        sad::Maybe<double> result = sad::duktape::GetValue<double>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( sad::is_fuzzy_equal(result.value(), 55) );
    }

    void testPtrMethods()
    {
        sad::String error;  

        sad::Point2D p(5, 7);
        
        sad::duktape::Context ctx;
        ctx.registerGlobal("pnt", &p);

        sad::duktape::register_ptr_method(&ctx, "x", &sad::Point2D::x);
        sad::duktape::register_ptr_method(&ctx, "y", &sad::Point2D::y);

        sad::duktape::register_ptr_method(&ctx, "setX", &sad::Point2D::setX);
        sad::duktape::register_ptr_method(&ctx, "setY", &sad::Point2D::setY);

        bool eval_result = ctx.eval(" setX(pnt, 55); x(pnt) ", false);
        ASSERT_TRUE( eval_result );
        sad::Maybe<double> result = sad::duktape::GetValue<double>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( sad::is_fuzzy_equal(result.value(), 55) );
    }*/

} _context_test;