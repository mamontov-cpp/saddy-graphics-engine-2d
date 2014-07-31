#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include <resource/tree.h>
#include <resource/link.h>
#include <db/custom/customschemafile.h>
#include <db/save.h>
#include <db/load.h>
#include <renderer.h>
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


/*!
 * Tests sad::db::custom::SchemaFile
 */
struct SadDbCustomSchemaFileTest : tpunit::TestFixture
{
 public:
   SadDbCustomSchemaFileTest() : tpunit::TestFixture(
		TEST(SadDbCustomSchemaFileTest::test_load_not_exists),
		TEST(SadDbCustomSchemaFileTest::test_load_parse_error),
		TEST(SadDbCustomSchemaFileTest::test_load_empty),
		TEST(SadDbCustomSchemaFileTest::test_load_null),
		TEST(SadDbCustomSchemaFileTest::test_load_invalid_format),
		TEST(SadDbCustomSchemaFileTest::test_load_empty_array),
		TEST(SadDbCustomSchemaFileTest::test_load_invalid_name),
		TEST(SadDbCustomSchemaFileTest::test_load_invalid_resource),
		TEST(SadDbCustomSchemaFileTest::test_load_invalid_schema),
		TEST(SadDbCustomSchemaFileTest::test_load_invalid_property_type),
		TEST(SadDbCustomSchemaFileTest::test_load_inherited_properties),
		TEST(SadDbCustomSchemaFileTest::test_load_duplicate_name),
		TEST(SadDbCustomSchemaFileTest::test_load_resource_name_exists),
		TEST(SadDbCustomSchemaFileTest::test_load_missing_resource),
		TEST(SadDbCustomSchemaFileTest::test_load_valid)
   ) {}

   void test_load_not_exists()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/not_exists.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count != 0);	   
   }

   void test_load_parse_error()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/parseerror.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count != 0);	   
   }

   void test_load_empty()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/empty.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count != 0);	   
   }

   void test_load_null()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/null.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count != 0);	   
   }

   void test_load_invalid_format()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/icons.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count != 0);	   
   }

   void test_load_empty_array()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/emptyarray.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);	   
   }

   void test_load_invalid_name()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/invalidname.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count != 0);	   
   }

   void test_load_invalid_resource()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/invalidresource.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count != 0);	   
   }

   void test_load_invalid_schema()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/invalidschema.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count != 0);	   
   }

   void test_load_invalid_property_type()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/invalidpropertytype.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count != 0);	   
   }

   void test_load_inherited_properties()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/inheritedproperty.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 4);	   
   }

   void test_load_duplicate_name()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/duplicatename.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count != 0);	   
   }

   void test_load_resource_name_exists()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/resourcenameexists.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count != 0);	   
   }

   void test_load_missing_resource()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/missingresource.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count != 0);	   
   }

   void test_load_valid()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/valid.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);

	   sad::resource::Link<sad::db::custom::Schema> link1;
	   link1.setTree(&tree);
	   link1.setPath("1");
	   ASSERT_TRUE(link1.get() == NULL);

	   link1.setPath("myschema");
	   ASSERT_TRUE(link1.get() != NULL);
   }

} _sad_db_custom_schema_file_test;



 