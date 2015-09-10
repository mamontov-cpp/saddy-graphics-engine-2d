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
	   TEST(SadDbVariantTest::testObject),
	   TEST(SadDbVariantTest::testVectorVectorAColor),
	   TEST(SadDbVariantTest::testConstChar)
   ) {}

	void test()
	{
		sad::db::Variant v((int)22);

		ASSERT_TRUE(v.get<int>().value() == 22);
		ASSERT_TRUE(v.get<unsigned int>().value() == 22);

		v.set<int>(25);

		ASSERT_TRUE(v.get<int>().value() == 25);
	}

	void testObject()
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

	void testVectorVectorAColor()
	{
		sad::Vector<sad::Vector<sad::AColor> > v;

		v.push_back(sad::Vector<sad::AColor>());
		v[0].push_back(sad::AColor(5, 5, 5, 0));
		v[0].push_back(sad::AColor(5, 5, 5, 0));
		v[0].push_back(sad::AColor(5, 5, 5, 0));

		v.push_back(sad::Vector<sad::AColor>());
		v[1].push_back(sad::AColor(5, 5, 5, 0));
		v[1].push_back(sad::AColor(5, 5, 5, 0));
		v[1].push_back(sad::AColor(5, 5, 5, 0));

		sad::db::Variant k(v);

		picojson::value saved;
		saved = k.save();

		sad::Vector<sad::Vector<sad::AColor> > tmp;
		sad::db::Variant r(tmp);
		
		ASSERT_TRUE( r.load(saved) );

		sad::Maybe<sad::Vector<sad::Vector<sad::AColor> > > vk = r.get<sad::Vector<sad::Vector<sad::AColor> > >();
		ASSERT_TRUE(vk.exists());
		
		sad::Vector<sad::Vector<sad::AColor> > vk_value = vk.value();
		ASSERT_TRUE(vk_value.size() == 2);
	}
	
	void testConstChar()
	{
		sad::db::Variant a("ab");
		ASSERT_TRUE( a.get<sad::String>().value() == "ab" );
	}

} _sad_db_variant;
