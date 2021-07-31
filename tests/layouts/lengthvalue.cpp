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
#include "layouts/serializablecell.h"
#include "fuzzyequal.h"
#pragma warning(pop)



/*!
 * Tests a features for sad::layouts::LengthValue
 */
struct SadLayoutsLengthValueTests : tpunit::TestFixture
{
 public:
   SadLayoutsLengthValueTests() : tpunit::TestFixture(
       TEST(SadLayoutsLengthValueTests::test),
       TEST(SadLayoutsLengthValueTests::testSerializableCells)
   ) {}
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void test()
    {
        sad::layouts::LengthValue v;
        v.Unit = sad::layouts::Unit::LU_Pixels;
        v.Value = 232;
        sad::db::Variant k(v);
        
        picojson::value saved;
        saved = k.save();
        
        sad::db::Variant r(v);
        ASSERT_TRUE( r.load(saved) );
        
        sad::Maybe<sad::layouts::LengthValue > vk = r.get<sad::layouts::LengthValue>();
        
        ASSERT_TRUE(vk.exists());
        ASSERT_TRUE(vk.value().Unit == sad::layouts::Unit::LU_Pixels);
        ASSERT_TRUE( sad::is_fuzzy_equal(vk.value().Value, 232) );		
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testSerializableCells()
   {
       sad::Vector<sad::layouts::SerializableCell> cells;
       cells << sad::layouts::SerializableCell();
       cells << sad::layouts::SerializableCell();

       cells[0].RowSpan = 2;
       cells[0].ColSpan = 2;

       cells[0].Children << 1 << 2;

       cells[1].RowSpan = 1;
       cells[1].ColSpan = 3;

       cells[1].Children << 2 << 3;

       sad::db::Variant k(cells);
        
       picojson::value saved;
       saved = k.save();
        
       sad::db::Variant r(cells);
       ASSERT_TRUE( r.load(saved) );
        
       sad::Maybe<sad::Vector<sad::layouts::SerializableCell> > vk = r.get<sad::Vector<sad::layouts::SerializableCell> >();
        
       ASSERT_TRUE(vk.exists());

       const sad::Vector<sad::layouts::SerializableCell>& rcells = vk.mutableValue();
       ASSERT_TRUE(rcells.size() == 2);
       ASSERT_TRUE(rcells[0].RowSpan == 2);
       ASSERT_TRUE(rcells[0].ColSpan == 2);
       ASSERT_TRUE(rcells[0].Children.size() == 2);
       ASSERT_TRUE(rcells[0].Children[0] == 1);
       ASSERT_TRUE(rcells[0].Children[1] == 2);

       ASSERT_TRUE(rcells[1].RowSpan == 1);
       ASSERT_TRUE(rcells[1].ColSpan == 3);
       ASSERT_TRUE(rcells[1].Children.size() == 2);
       ASSERT_TRUE(rcells[1].Children[0] == 2);
       ASSERT_TRUE(rcells[1].Children[1] == 3);
   }

} _sad_layouts_length_value_tests;
