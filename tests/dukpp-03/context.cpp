#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdio>
#include "dukpp-03/context.h"
#include "sadpoint.h"
#include "db/save.h"
#include "db/load.h"
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



sad::dukpp03::CompiledFunction func;

void set_func(const sad::dukpp03::CompiledFunction& f)
{
    func = f;
}


struct ContextTest : tpunit::TestFixture
{
public:
    ContextTest() : tpunit::TestFixture(
       TEST(ContextTest::testInitGet),
       TEST(ContextTest::testPushGet),
       TEST(ContextTest::testEvalNormal),
       TEST(ContextTest::testEvalFail),
       TEST(ContextTest::testEvalTimeout),
       TEST(ContextTest::testEvalFromFileNormal),
       TEST(ContextTest::testEvalFromFileFail),
       TEST(ContextTest::testCleanStack),
       TEST(ContextTest::testReset),
       TEST(ContextTest::testThrow),
       TEST(ContextTest::testRegisterGlobal),
       TEST(ContextTest::testRegisterCallable),
       TEST(ContextTest::testRegisterVoidFunctions),
       TEST(ContextTest::testRegisterReturnFunctions),
       TEST(ContextTest::testMethods),
       TEST(ContextTest::testPtrMethods),
       TEST(ContextTest::testCompiledFunction) 
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
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testPushGet()
    {
        sad::dukpp03::Context ctx(true);

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
            ::dukpp03::Maybe<sad::String> maybev = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(&ctx, test_number);
            ASSERT_TRUE( maybev.exists());
            ASSERT_TRUE( maybev.value() == "23");
        }
    }

    /*! Test for normal evaluation process
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
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
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
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
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testEvalTimeout()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        ctx.setMaximumExecutionTime(1000);
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
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testEvalFromFileNormal()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.evalFromFile("tests/duktape/common.js", false, &error);
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 2);
    }
    /*! Tests evaluation from file on non-existing file
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testEvalFromFileFail()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        bool eval_result = ctx.evalFromFile("tests/duktape/notexists.js", false, &error);
        ASSERT_TRUE( !eval_result );
        ASSERT_TRUE( error.size() != 0 );
    }

    /*! Test cleaning  of a pool
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testCleanStack()
    {
        sad::dukpp03::Context ctx(true);
        sad::Point2D pts2d(3, 4);
        ::dukpp03::PushValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(&ctx, pts2d);
        ::dukpp03::Maybe<sad::Point2D> mbpts2d =
            ::dukpp03::GetValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( mbpts2d.exists() );
            
        ctx.cleanStack();

        mbpts2d = ::dukpp03::GetValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( mbpts2d.exists() == false );       
    }

    /*! Test full reset of context
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testReset()
    {
        sad::dukpp03::Context ctx(true);
        sad::Point2D pts2d(3, 4);
        ::dukpp03::PushValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(&ctx, pts2d);
        ::dukpp03::Maybe<sad::Point2D> mbpts2d =
            ::dukpp03::GetValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( mbpts2d.exists() );
            
        ctx.reset();

        mbpts2d = ::dukpp03::GetValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( mbpts2d.exists() == false );
    }

    /*! Tests throwing exception
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testThrow()
    {
        sad::dukpp03::Context ctx;
        ctx.throwError("Generic Error!");
        const char* s = duk_to_string(ctx.context(), -1);
        ASSERT_TRUE( s != nullptr );
        sad::String testvalue = s;
        ASSERT_TRUE(  testvalue.size() !=0 );
    }

    /*! Tests registering value as property of global object
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testRegisterGlobal()
    {
        sad::dukpp03::Context ctx(true);
        ctx.registerGlobal("value", true);
        bool eval_result = ctx.eval(" !value ", false);
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<bool> result = ::dukpp03::GetValue<bool, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == false );
    }

    /*! Tests registering callable function
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testRegisterCallable()
    {
        sad::dukpp03::Context ctx(true);
        ctx.registerCallable("f", new MockCallable());
        bool eval_result = ctx.eval(" (f() + f()) * (f() + f()) ; f() + f()*f() ", false);
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 2 );
    }

    /*! Tests registering functions
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testRegisterVoidFunctions()
    {
        std::string error;  

        sad::dukpp03::Context ctx;
        ctx.bind("f00", print_something);
        bool eval_result = ctx.eval(" f00(); f00(); ", true, &error);
        ASSERT_TRUE( eval_result );

        ctx.bind("f01", print_number_1);
        eval_result = ctx.eval(" f01(21); f01(32); ", true);
        ASSERT_TRUE( eval_result );

        ctx.bind("f03", print_number_3);
        eval_result = ctx.eval(" f03(21, 44, 56); f03(32, 88, 93); ", true);
        ASSERT_TRUE( eval_result );

        
        eval_result = ctx.eval(" f03(21) ", true, &error);
        ASSERT_TRUE( !eval_result );

        eval_result = ctx.eval(" f03(undefined, undefined, undefined) ", true, &error);
        ASSERT_TRUE( !eval_result );
    }

    /*! Tests functions, which return some values
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testRegisterReturnFunctions()
    {
        std::string error;  

        sad::dukpp03::Context ctx;
        ctx.bind("f00", return_something);
        bool eval_result = ctx.eval(" f00(); f00(); ", false, &error);
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<int> result = DUKPP03_FROM_STACK(int, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 32 );

        ctx.bind("f01", return_number_1);
        eval_result = ctx.eval(" f01(21); f01(32); ", false);
        ASSERT_TRUE( eval_result );
        result = DUKPP03_FROM_STACK(int, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 32 );

        ctx.bind("f03", return_number_3);
        eval_result = ctx.eval(" f03(21, 44, 56); f03(32, 88, 93); ", false);
        ASSERT_TRUE( eval_result );
        result = DUKPP03_FROM_STACK(int, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 32 - 88 - 93 );

        
        eval_result = ctx.eval(" f03(21) ", false, &error);
        ASSERT_TRUE( !eval_result );

        eval_result = ctx.eval(" f03(undefined, undefined, undefined) ", false, &error);
        ASSERT_TRUE( !eval_result );        
    }

    /*! Tests method calls
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testMethods()
    {
        std::string error;  
        
        sad::dukpp03::Context ctx(true);
        sad::dukpp03::ClassBinding* p2dbinding = new sad::dukpp03::ClassBinding();
        p2dbinding->addConstructor<sad::Point2D, double, double>("Point2D");
        p2dbinding->addMethod("x", sad::dukpp03::bind_method::from(&sad::Point2D::x));
        p2dbinding->addMethod("y", sad::dukpp03::bind_method::from(&sad::Point2D::y));
        p2dbinding->addMethod("setX", sad::dukpp03::bind_method::from(&sad::Point2D::setX));
        p2dbinding->addMethod("setY", sad::dukpp03::bind_method::from(&sad::Point2D::setY));

        ctx.addClassBinding("sad::Point2D", p2dbinding);

        bool eval_result = ctx.eval(" var f = new Point2D(3, 4);  f.setX(55); f.x()  ", false, &error);
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<double> result = DUKPP03_FROM_STACK(double, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( sad::is_fuzzy_equal(result.value(), 55) );
    }

    /*! Tests methods, which can be called from pointers
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testPtrMethods()
    {
        std::string error;  
        
        sad::dukpp03::Context ctx(true);
        
        sad::dukpp03::ClassBinding* p2dbinding = new sad::dukpp03::ClassBinding();
        p2dbinding->addConstructor<sad::Point2D, double, double>("Point2D");
        p2dbinding->addMethod("x", sad::dukpp03::bind_method::from(&sad::Point2D::x));
        p2dbinding->addMethod("y", sad::dukpp03::bind_method::from(&sad::Point2D::y));
        p2dbinding->addMethod("setX", sad::dukpp03::bind_method::from(&sad::Point2D::setX));
        p2dbinding->addMethod("setY", sad::dukpp03::bind_method::from(&sad::Point2D::setY));

        ctx.addClassBinding("sad::Point2D", p2dbinding);

        sad::Point2D p(5, 7);
        ctx.registerGlobal("pnt", &p);

        bool eval_result = ctx.eval(" pnt.setX(55); pnt.x() ", false, &error);
        ASSERT_TRUE( eval_result );
        ::dukpp03::Maybe<double> result =  DUKPP03_FROM_STACK(double, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( sad::is_fuzzy_equal(result.value(), 55) );
    }

    /*! Tests basic working with compiled functions
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testCompiledFunction()
    {
        sad::dukpp03::Context ctx;
        ctx.bind("setFunction", set_func);

        // Setting callback from JS code
        ctx.eval(" setFunction(function(a) { print(a); return a;  }); ", false);
        // Clean stack after evaluating
        ctx.cleanStack();

        // Push argument (22) on stack
        dukpp03::PushValue<int, sad::dukpp03::BasicContext>::perform(&ctx, 22);
        // Evaluate function - it will print 22 and return 22 on stack
        func.call(&ctx);
        ::dukpp03::Maybe<int> result =  DUKPP03_FROM_STACK(int, &ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == 22 );
    }

} _context_test;