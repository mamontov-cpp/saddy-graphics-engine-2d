#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "db/dbobject.h"
#include "db/dbtable.h"
#include "db/dbdatabase.h"
#include "db/schema/schema.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbobjectfactory.h"


#include "renderer.h"

#include "mock3.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

/*!
 * Tests sad::db::custom::SchemaFile
 */
struct SadDbDatabaseTest : tpunit::TestFixture
{
public:
	SadDbDatabaseTest() : tpunit::TestFixture(		
		TEST(SadDbDatabaseTest::test_save),
		TEST(SadDbDatabaseTest::test_load_not_exists),
		TEST(SadDbDatabaseTest::test_load_empty),
		TEST(SadDbDatabaseTest::test_load_null),
		TEST(SadDbDatabaseTest::test_load_emptyobject),
		TEST(SadDbDatabaseTest::test_load_invalid_definitions),
		TEST(SadDbDatabaseTest::test_load_invalid_property),
		TEST(SadDbDatabaseTest::test_load_invalid_item),
		TEST(SadDbDatabaseTest::test_load_valid),
		TEST(SadDbDatabaseTest::test_load_valid2)
	) {}

	void test_save()
	{
		sad::db::Database db;
		sad::String data;
		db.save(data);
		ASSERT_TRUE(data.size() != 0);
		ASSERT_TRUE(db.load(data));
	}

	void test_load_not_exists()
	{
		sad::db::ObjectFactory* f = new sad::db::ObjectFactory();
		f->add<Mock3>("Mock3",  new sad::db::schema::Schema());

		sad::db::Database * db = new sad::db::Database();
		db->setFactory(f);

		sad::Renderer r;
		r.addDatabase("", db);

		bool result = r.database("")->loadFromFile("tests/db/database/notexists.json");
		ASSERT_FALSE( result );
	}

	void test_load_empty()
	{
		sad::db::ObjectFactory* f = new sad::db::ObjectFactory();
		f->add<Mock3>("Mock3",  new sad::db::schema::Schema());

		sad::db::Database * db = new sad::db::Database();
		db->setFactory(f);

		sad::Renderer r;
		r.addDatabase("", db);

		bool result = r.database("")->loadFromFile("tests/db/database/empty.json");
		ASSERT_FALSE( result );
	}

	void test_load_null()
	{
		sad::db::ObjectFactory* f = new sad::db::ObjectFactory();
		f->add<Mock3>("Mock3",  new sad::db::schema::Schema());

		sad::db::Database * db = new sad::db::Database();
		db->setFactory(f);

		sad::Renderer r;
		r.addDatabase("", db);

		bool result = r.database("")->loadFromFile("tests/db/database/null.json");
		ASSERT_FALSE( result );
	}

	void test_load_emptyobject()
	{
		sad::db::ObjectFactory* f = new sad::db::ObjectFactory();
		f->add<Mock3>("Mock3",  new sad::db::schema::Schema());

		sad::db::Database * db = new sad::db::Database();
		db->setFactory(f);

		sad::Renderer r;
		r.addDatabase("", db);

		bool result = r.database("")->loadFromFile("tests/db/database/eo.json");
		ASSERT_FALSE( result );
	}

	void test_load_invalid_definitions()
	{
		sad::db::ObjectFactory* f = new sad::db::ObjectFactory();
		f->add<Mock3>("Mock3",  new sad::db::schema::Schema());

		sad::db::Database * db = new sad::db::Database();
		db->setFactory(f);

		sad::Renderer r;
		r.addDatabase("", db);

		bool result = r.database("")->loadFromFile("tests/db/database/invaliddefs.json");
		ASSERT_FALSE( result );
	}

	void test_load_invalid_property()
	{
		sad::db::ObjectFactory* f = new sad::db::ObjectFactory();
		f->add<Mock3>("Mock3",  new sad::db::schema::Schema());

		sad::db::Database * db = new sad::db::Database();
		db->setFactory(f);

		sad::Renderer r;
		r.addDatabase("", db);

		bool result = r.database("")->loadFromFile("tests/db/database/invalidtype.json");
		ASSERT_FALSE( result );
	}

	void test_load_invalid_item()
	{
		sad::db::ObjectFactory* f = new sad::db::ObjectFactory();
		f->add<Mock3>("Mock3",  new sad::db::schema::Schema());

		sad::db::Database * db = new sad::db::Database();
		db->setFactory(f);

		sad::Renderer r;
		r.addDatabase("", db);

		bool result = r.database("")->loadFromFile("tests/db/database/invaliditem.json");
		ASSERT_FALSE( result );
	}

	void test_load_valid()
	{
		sad::db::ObjectFactory* f = new sad::db::ObjectFactory();
		f->add<Mock3>("Mock3",  new sad::db::schema::Schema());

		sad::db::Database * db = new sad::db::Database();
		db->setFactory(f);

		sad::Renderer r;
		r.addDatabase("", db);

		bool result = r.database("")->loadFromFile("tests/db/database/valid.json");
		ASSERT_TRUE( result );

		sad::db::Object * o = r.database("")->queryByMinorId(1)[0];
		ASSERT_TRUE(o->Name == "test");
	}

	void test_load_valid2()
	{
		sad::db::ObjectFactory* f = new sad::db::ObjectFactory();
		f->add<Mock3>("Mock3",  new sad::db::schema::Schema());

		sad::db::Database * db = new sad::db::Database();
		db->setFactory(f);

		sad::Renderer r;
		r.addDatabase("", db);

		bool result = r.database("")->loadFromFile("tests/db/database/valid2.json");
		ASSERT_TRUE( result );
	}

} _sad_db_database_test;