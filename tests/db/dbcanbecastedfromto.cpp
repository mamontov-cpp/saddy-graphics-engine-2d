#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "db/dbcanbecastedfromto.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "font.h"
#pragma warning(pop)



/*!
 * Tests sad::Controls callbacks working and main workflow
 */
struct SadCanBeCastedFromToTest : tpunit::TestFixture
{
 public:
   SadCanBeCastedFromToTest() : tpunit::TestFixture(
       TEST(SadCanBeCastedFromToTest::test)
   ) {}

    void test()
    {
        ASSERT_TRUE( sad::db::can_be_casted_from_to("int", false, 0,  "int", false, 0) );
        ASSERT_TRUE( sad::db::can_be_casted_from_to("int", false, 0, "unsigned int", false, 0) );

        ASSERT_FALSE( sad::db::can_be_casted_from_to("float", false, 0, "sad::String", false, 0) );

        sad::ClassMetaData * meta = sad::Font::globalMetaData();
        ASSERT_TRUE(meta != nullptr);

        ASSERT_FALSE( sad::db::can_be_casted_from_to("sad::Font", true, 0, "sad::Object", true, 0) );
        ASSERT_FALSE( sad::db::can_be_casted_from_to("sad::Font", true, 1, "sad::Object", true, 0) );
        ASSERT_FALSE( sad::db::can_be_casted_from_to("sad::Font", true, 0, "sad::Object", true, 1) );
        ASSERT_TRUE( sad::db::can_be_casted_from_to("sad::Font", true, 1, "sad::Object", true, 1) );
    }

} _sad_can_be_casted_from_to_test;
