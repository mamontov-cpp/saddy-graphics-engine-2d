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

struct FSTest : tpunit::TestFixture
{
public:
    FSTest() : tpunit::TestFixture(
       TEST(FSTest::testPathDelimiter),
       TEST(FSTest::testCanonicalizePath),
       TEST(FSTest::testIsAbsolutePath),
       TEST(FSTest::testConcatPaths),
       TEST(FSTest::testFolder),
       TEST(FSTest::testFileExists)
    ) {}

    /*! Test for sad.util.pathDelimiter()
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testPathDelimiter()
    {
        std::string error;
        sad::dukpp03::Context ctx;

        bool eval_result = ctx.eval("sad.util.pathDelimiter()", false, &error);
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

    /*! Test for sad.util.canonicalizePath()
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testCanonicalizePath()
    {
        std::string error;
        sad::dukpp03::Context ctx;

        bool eval_result = ctx.eval("sad.util.canonicalizePath(\"C:\\\\1\\\\2\\\\..\\\\..\\\\3.txt\")", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<sad::String> result = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == "C:\\3.txt");        
    }

    /*! Test for sad.util.isAbsolutePath()
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testIsAbsolutePath()
    {
        std::string error;
        sad::dukpp03::Context ctx;

        bool eval_result = ctx.eval("sad.util.isAbsolutePath(\"1/2/3.txt\")", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<bool> result = ::dukpp03::GetValue<bool, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() ==  false);      
    }
    
    /*! Test for sad.util.concatPaths()
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testConcatPaths()
    {
        std::string error;
        sad::dukpp03::Context ctx;

        bool eval_result = ctx.eval("sad.util.concatPaths(\"/1/\",\"2/\")", false, &error);
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
    
    /*! Test for sad.util.folder()
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testFolder()
    {
        std::string error;
        sad::dukpp03::Context ctx;

        bool eval_result = ctx.eval("sad.util.folder(\"/1/2.txt\")", false, &error);
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
    
    /*! Test for sad.util.fileExists()
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testFileExists()
    {
        std::string error;
        sad::dukpp03::Context ctx;

        bool eval_result = ctx.eval("sad.util.fileExists(\"1/2/3.txt\")", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<bool> result = ::dukpp03::GetValue<bool, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() ==  false);    
    }
} _fs_test;