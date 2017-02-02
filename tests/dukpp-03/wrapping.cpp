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
#include "sprite2d.h"
#include "animations/animationscolor.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

inline bool toIntAndEqual(int a, int b)
{
    return a == b;
}

struct WrappingTest : tpunit::TestFixture
{
public:
    WrappingTest() : tpunit::TestFixture(
       TEST(WrappingTest::testWrapSadDbObjectWithoutBinding),
       TEST(WrappingTest::testWrapSadDbObjectWithoutBindingFromVariant),
       TEST(WrappingTest::testWrapSadDbObjectWithBinding),
       TEST(WrappingTest::testWrapSadDbObjectWithBindingFromVariant),
       TEST(WrappingTest::testWrapObjectWithoutBinding),
       TEST(WrappingTest::testWrapObjectWithoutBindingFromVariant),
       TEST(WrappingTest::testWrapObjectWithBinding),
       TEST(WrappingTest::testWrapObjectWithBindingFromVariant)
    ) {}

    /*! Tests automatical wrapping for sad::db::Object without binding
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapSadDbObjectWithoutBinding()
    {
        
    }

    /*! Tests automatical wrapping for sad::db::Object without binding with functions, which returns it as variant
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapSadDbObjectWithoutBindingFromVariant()
    {
        
    }

    /*! Tests automatical wrapping for sad::db::Object with bindings
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapSadDbObjectWithBinding()
    {
        
    }

    /*! Tests automatical wrapping for sad::db::Object with binding with functions, which returns it as variant
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapSadDbObjectWithBindingFromVariant()
    {
        
    }
    
    /*! Tests automatical wrapping for object without binding
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapObjectWithoutBinding()
    {
        
    }

    /*! Tests automatical wrapping for object without binding with functions, which returns it as variant
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapObjectWithoutBindingFromVariant()
    {
        
    }

    /*! Tests automatical wrapping for object with bindings
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapObjectWithBinding()
    {
        
    }

    /*! Tests automatical wrapping for object with binding with functions, which returns it as variant
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testWrapObjectWithBindingFromVariant()
    {
        
    }
} _wrapping_test;