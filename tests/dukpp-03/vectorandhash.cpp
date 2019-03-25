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


static sad::String getFirstVectorItem(const sad::Vector<sad::String>& x)
{
    return x[0];
}

sad::Vector<sad::String> makeVectorTest()
{
    sad::Vector<sad::String> a;
    a << "hello" << "world" << "c";
    return a;
}


static sad::String getFirstHashItem(const sad::Hash<sad::String, sad::String>& x)
{
    return x["1"];
}

sad::Hash<sad::String, sad::String> makeHashTest()
{
    sad::Hash<sad::String, sad::String> a;
    a.insert("1", "hello");
    a.insert("2", "world");
    return a;
}


struct VectorAndHashTest : tpunit::TestFixture
{
public:
    VectorAndHashTest() : tpunit::TestFixture(
       TEST(VectorAndHashTest::testVector),
       TEST(VectorAndHashTest::testHash),
       TEST(VectorAndHashTest::testHashBoolOnStack)
    ) {}

    /*! A common test for getting and pushing vector from stack
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testVector()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        ctx.registerCallable("makeVectorTest", sad::dukpp03::make_function::from(makeVectorTest));
        ctx.registerCallable("getFirstVectorItem", sad::dukpp03::make_function::from(getFirstVectorItem));

        bool eval_result = ctx.eval("var a = makeVectorTest(); getFirstVectorItem(a) + \" \" + a[1] ", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );
        ::dukpp03::Maybe<sad::String> result = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == "hello world");      
    }

    /*! A common test for getting and pushing hash from stack
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
     void testHash()
     {
        std::string error;
        sad::dukpp03::Context ctx;
        ctx.registerCallable("makeHashTest", sad::dukpp03::make_function::from(makeHashTest));
        ctx.registerCallable("getFirstHashItem", sad::dukpp03::make_function::from(getFirstHashItem));

        bool eval_result = ctx.eval("var a = makeHashTest(); getFirstHashItem(a) + \" \" + a[\"2\"] ", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.empty() );
        ::dukpp03::Maybe<sad::String> result = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(&ctx, -1);
        ASSERT_TRUE( result.exists() );
        ASSERT_TRUE( result.value() == "hello world");        
     }

     /*! A common test for getting and pushing hash from stack
      */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
     void testHashBoolOnStack()
     {
         std::string error;
         sad::dukpp03::Context ctx;
         
         bool eval_result = ctx.eval("var a = {\"a\": false}; a ", false, &error);
         if (!eval_result)
         {
             printf("%s\n", error.c_str());
         }
         ASSERT_TRUE(eval_result);
         ASSERT_TRUE(error.empty());
         ::dukpp03::Maybe<sad::Hash<sad::String, sad::db::Variant> > result = ::dukpp03::GetValue<sad::Hash<sad::String, sad::db::Variant>, sad::dukpp03::BasicContext>::perform(&ctx, -1);
         ASSERT_TRUE(result.exists());
         ASSERT_TRUE(result.mutableValue().contains("a"));
         sad::db::Variant value = result.mutableValue()["a"];
         ASSERT_TRUE(value.typeName() == "bool")
         ASSERT_TRUE(value.get<bool>().exists());
         ASSERT_TRUE(value.get<bool>().value() == false);
     }
} _vector_and_hash_test;