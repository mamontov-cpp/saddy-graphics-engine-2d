#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "texturemappedfont.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "db/save.h"
#include "db/load.h"
#include "layouts/lengthvalue.h"
#include "fuzzyequal.h"
#pragma warning(pop)



/*!
 * Tests a features for sad::layouts::LengthValue
 */
struct SadLayoutsLengthValueTests : tpunit::TestFixture
{
 public:
   SadLayoutsLengthValueTests() : tpunit::TestFixture(
       TEST(SadLayoutsLengthValueTests::test)
   ) {}

    void test()
    {
		sad::layouts::LengthValue v;
		v.Unit = sad::layouts::LU_Pixels;
		v.Value = 232;
		sad::db::Variant k(v);
		
		picojson::value saved;
        saved = k.save();
		
        sad::db::Variant r(v);
		ASSERT_TRUE( r.load(saved) );
		
		sad::Maybe<sad::layouts::LengthValue > vk = r.get<sad::layouts::LengthValue>();
		
		ASSERT_TRUE(vk.exists());
		ASSERT_TRUE(vk.value().Unit == sad::layouts::LU_Pixels);
		ASSERT_TRUE( sad::is_fuzzy_equal(vk.value().Value, 232) );		
    }

} _sad_layouts_length_value_tests;
