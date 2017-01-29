#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "dukpp-03/context.h"
#include "db/save.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


struct VariantPoolTest : tpunit::TestFixture
{
public:
    VariantPoolTest() : tpunit::TestFixture(
       TEST(VariantPoolTest::testCopyConstructor),
       TEST(VariantPoolTest::testAssignment),
       TEST(VariantPoolTest::testInsert),
       TEST(VariantPoolTest::testGet),
       TEST(VariantPoolTest::testFree)
    ) {}
    
    void testCopyConstructor()
    {
        sad::dukpp03::VariantPool pool1;
        sad::String id = pool1.insert(new sad::db::Variant(1));
        ASSERT_TRUE(id == "0");

        sad::duktape::VariantPool pool2(pool1);

        pool1.get("0")->set(2);

        int pool2value = pool2.get("0")->get<int>().value();
        ASSERT_TRUE(pool2value == 1);
    }

    void testAssignment()
    {
        sad::duktape::VariantPool pool1;
        sad::String id = pool1.insert(new sad::db::Variant(1));
        ASSERT_TRUE(id == "0");

        sad::duktape::VariantPool pool2(pool1);

        pool1.get("0")->set(2);

        pool2 = pool1;

        int pool2value = pool2.get("0")->get<int>().value();
        ASSERT_TRUE(pool2value == 2);
    }

    void testInsert()
    {
        sad::duktape::VariantPool pool1;
        sad::String id = pool1.insert(new sad::db::Variant(1));

        int pool2value = pool1.get(id)->get<int>().value();
        ASSERT_TRUE(pool2value == 1);
    }

    void testGet()
    {
        sad::duktape::VariantPool pool1;
        sad::String id = pool1.insert(new sad::db::Variant(1));

        sad::db::Variant* variantptr = pool1.get(id);
        ASSERT_TRUE(variantptr != NULL);

        int pool2value = variantptr->get<int>().value();
        ASSERT_TRUE(pool2value == 1);

        sad::db::Variant* othervariantptr = pool1.get("22");
        ASSERT_TRUE(othervariantptr == NULL);
    }

    void testFree()
    {
        sad::duktape::VariantPool pool1;
        sad::String id = pool1.insert(new sad::db::Variant(1));
        pool1.free();

        sad::db::Variant* variantptr = pool1.get(id);
        ASSERT_TRUE(variantptr == NULL);
    }

} _variant_pool_test;