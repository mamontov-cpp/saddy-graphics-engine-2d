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
#pragma warning(pop)



/*!
 * Tests sad::Controls callbacks working and main workflow
 */
struct SadDbVariantTest : tpunit::TestFixture
{
 public:
   SadDbVariantTest() : tpunit::TestFixture(
	   TEST(SadDbVariantTest::test),
	   TEST(SadDbVariantTest::test_object)
   ) {}

	void test()
	{
		sad::db::Variant v((int)22);

		ASSERT_TRUE(v.get<int>().value() == 22);
		ASSERT_TRUE(v.get<unsigned int>().value() == 22);

		v.set<int>(25);

		ASSERT_TRUE(v.get<int>().value() == 25);
	}

	void test_object()
	{
		sad::ClassMetaData * meta = sad::TextureMappedFont::globalMetaData();
		ASSERT_TRUE(meta != NULL);

		sad::Font * font = new sad::TextureMappedFont();

		sad::db::Variant v(font);

		sad::Maybe<sad::Object *> test = v.get<sad::Object*>();
		ASSERT_TRUE(test.exists());
		ASSERT_TRUE(test.value() == (sad::Object *)font);

		ASSERT_TRUE(v.get<sad::Font*>().value() == font);
		delete font;
	}

} _sad_db_variant;
