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
 * Tests sad::db::Database
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
		TEST(SadDbDatabaseTest::test_load_valid2),
		TEST(SadDbDatabaseTest::test_add),
		TEST(SadDbDatabaseTest::test_remove),
		TEST(SadDbDatabaseTest::test_properties),
		TEST(SadDbDatabaseTest::test_query_by_majorid),
		TEST(SadDbDatabaseTest::test_query_by_minorid),
		TEST(SadDbDatabaseTest::test_query_by_name)	
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
		ASSERT_TRUE(o->objectName() == "test");
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
	
	void test_add()
	{
		sad::db::Database db;
		ASSERT_TRUE( db.addTable("table", new sad::db::Table()) );
		
		sad::db::Table* t2 = new sad::db::Table();
		ASSERT_FALSE( db.addTable("table", t2) );
		delete t2;
		
		ASSERT_TRUE( db.table("table") != NULL );
		ASSERT_TRUE( db.table("table2") == NULL );		
	}
	
	void test_remove()
	{
		sad::db::Database db;
		ASSERT_TRUE( db.addTable("table", new sad::db::Table()) );
		
		ASSERT_TRUE( db.table("table") != NULL );
		
		db.removeTable("table");
		db.removeTable("table2");
		
		ASSERT_TRUE( db.table("table") == NULL );		
	}
	
	void test_properties()
	{
		sad::db::Database db;
		
		db.addProperty("test", new sad::db::StoredProperty<int>());
		
		ASSERT_TRUE( db.propertyByName("test") != NULL );
		
		ASSERT_TRUE( db.setProperty("test", 3) );
		
		ASSERT_TRUE( db.getProperty<int>("test").value() == 3);
	
		db.removeProperty("test");
		ASSERT_TRUE( db.propertyByName("test") == NULL );		
	}
	
	void test_query_by_majorid()
	{
		sad::db::Database db;
		db.addTable("table", new sad::db::Table());
		
		Mock3 * mock = new Mock3();
		mock->setObjectName("test");
		mock->MajorId = 1;
		mock->MinorId = 1;
		db.table("table")->add(mock);
	
		ASSERT_TRUE( db.queryByMajorId(1)->objectName() == "test");
		ASSERT_TRUE( db.queryByMajorId(22) == NULL );
	}
	
	void test_query_by_minorid()
	{
		sad::db::Database db;
		db.addTable("table", new sad::db::Table());
		
		Mock3 * mock = new Mock3();
		mock->setObjectName("test");
		mock->MajorId = 1;
		mock->MinorId = 1;
		db.table("table")->add(mock);
	
		ASSERT_TRUE( db.queryByMinorId(1)[0]->objectName() == "test");
		ASSERT_TRUE( db.queryByMinorId(22).size() == 0 );
	}
	
	void test_query_by_name()
	{
		sad::db::Database db;
		db.addTable("table", new sad::db::Table());
		
		Mock3 * mock = new Mock3();
		mock->setObjectName("test");
		mock->MajorId = 1;
		mock->MinorId = 1;
		db.table("table")->add(mock);
	
		ASSERT_TRUE( db.queryByName("test")[0]->MinorId == 1);
		ASSERT_TRUE( db.queryByName("test2").size() == 0 );
	}
				
} _sad_db_database_test;