#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "db/dbobject.h"
#include "db/dbtable.h"
#include "db/dbdatabase.h"
#include "db/schema/schema.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "db/save.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "db/load.h"
#include "db/dbobjectfactory.h"

#include "animations/animationscolor.h"
#include "animations/easing/easingfunction.h"

#include "fuzzyequal.h"

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
        TEST(SadDbDatabaseTest::test_load_nullptr),
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
        TEST(SadDbDatabaseTest::test_query_by_name),
        TEST(SadDbDatabaseTest::test_snapshot_change_table),
        TEST(SadDbDatabaseTest::test_snapshot_reset_table),
        TEST(SadDbDatabaseTest::test_snapshot_change_objects),
        TEST(SadDbDatabaseTest::test_snapshot_reset_objects),
        TEST(SadDbDatabaseTest::test_animation_easing_functions)
    ) {}

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void test_save()
    {
        sad::db::Database db;
        sad::String data;
        db.save(data);
        ASSERT_TRUE(data.size() != 0);
        ASSERT_TRUE(db.load(data));
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
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

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
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
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void test_load_nullptr()
    {
        sad::db::ObjectFactory* f = new sad::db::ObjectFactory();
        f->add<Mock3>("Mock3",  new sad::db::schema::Schema());

        sad::db::Database * db = new sad::db::Database();
        db->setFactory(f);

        sad::Renderer r;
        r.addDatabase("", db);

        bool result = r.database("")->loadFromFile("tests/db/database/nullptr.json");
        ASSERT_FALSE( result );
    }
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
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
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
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
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
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
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
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
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
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
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
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
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void test_add()
    {
        sad::db::Database db;
        ASSERT_TRUE( db.addTable("table", new sad::db::Table()) );
        
        sad::db::Table* t2 = new sad::db::Table();
        ASSERT_FALSE( db.addTable("table", t2) );
        delete t2;
        
        ASSERT_TRUE( db.table("table") != nullptr );
        ASSERT_TRUE( db.table("table2") == nullptr );		
    }
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void test_remove()
    {
        sad::db::Database db;
        ASSERT_TRUE( db.addTable("table", new sad::db::Table()) );
        
        ASSERT_TRUE( db.table("table") != nullptr );
        
        db.removeTable("table");
        db.removeTable("table2");
        
        ASSERT_TRUE( db.table("table") == nullptr );		
    }
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void test_properties()
    {
        sad::db::Database db;
        
        db.addProperty("test", new sad::db::StoredProperty<int>());
        
        ASSERT_TRUE( db.propertyByName("test") != nullptr );
        
        ASSERT_TRUE( db.setProperty("test", 3) );
        
        ASSERT_TRUE( db.getProperty<int>("test").value() == 3);
    
        db.removeProperty("test");
        ASSERT_TRUE( db.propertyByName("test") == nullptr );		
    }
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
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
        ASSERT_TRUE( db.queryByMajorId(22) == nullptr );

        ASSERT_TRUE( db.objectByMajorId<Mock3>(1)->objectName() == "test");
    }
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
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

        ASSERT_TRUE( db.objectsByMinorId<Mock3>(1)[0]->objectName() == "test");
        ASSERT_TRUE( db.objectByMinorId<Mock3>(1)->objectName() == "test");
    }
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
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
        ASSERT_TRUE( db.objectsByName<Mock3>("test")[0]->objectName() == "test");
        ASSERT_TRUE( db.objectByName<Mock3>("test")->objectName() == "test");

    }
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void test_snapshot_change_table()
    {
        sad::db::ObjectFactory* f = new sad::db::ObjectFactory();
        f->add<Mock3>("Mock3",  new sad::db::schema::Schema());

        sad::db::Database * db = new sad::db::Database();
        db->setFactory(f);

        sad::Renderer r;
        r.addDatabase("", db);

        // Fill initial data for snapshots
        sad::db::Table* tbl = new sad::db::Table();
        db->addTable("table", tbl);

        Mock3* mock1 = new Mock3();
        mock1->setObjectName("m1");

        Mock3* mock2 = new Mock3();
        mock2->setObjectName("m2");
        tbl->add(mock1);
        tbl->add(mock2);
        db->saveSnapshot();

        ASSERT_TRUE( db->objectByName<Mock3>("m1") == mock1 );
        ASSERT_TRUE( db->objectByName<Mock3>("m2") == mock2 );

        // Change database, altering tables. Check that objects will not exists
        db->removeTable("table");
        ASSERT_TRUE( db->table("table") == nullptr );
        
        sad::db::Table* tbl1 = new sad::db::Table();
        db->addTable("table1", tbl1);
        ASSERT_TRUE( db->table("table1") != nullptr );

        sad::db::Table* tbl2 = new sad::db::Table();
        db->addTable("table2", tbl2);
        ASSERT_TRUE( db->table("table2") != nullptr );

        ASSERT_TRUE( db->objectByName<Mock3>("m1") == nullptr );
        ASSERT_TRUE( db->objectByName<Mock3>("m2") == nullptr );

        // Restore database and check, that everything is fine
        db->restoreSnapshot();

        ASSERT_TRUE( db->table("table") != nullptr );
        ASSERT_TRUE( db->table("table1") == nullptr );
        ASSERT_TRUE( db->table("table2") == nullptr );        
        ASSERT_TRUE( db->objectByName<Mock3>("m1") != nullptr );
        ASSERT_TRUE( db->objectByName<Mock3>("m2") != nullptr );
    }
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void test_snapshot_reset_table()
    {
        sad::db::ObjectFactory* f = new sad::db::ObjectFactory();
        f->add<Mock3>("Mock3",  new sad::db::schema::Schema());

        sad::db::Database * db = new sad::db::Database();
        db->setFactory(f);

        sad::Renderer r;
        r.addDatabase("", db);

        // Fill initial data for snapshots
        sad::db::Table* tbl = new sad::db::Table();
        db->addTable("table", tbl);

        Mock3* mock1 = new Mock3();
        mock1->setObjectName("m1");

        Mock3* mock2 = new Mock3();
        mock2->setObjectName("m2");
        tbl->add(mock1);
        tbl->add(mock2);
        db->saveSnapshot();

        ASSERT_TRUE( db->objectByName<Mock3>("m1") == mock1 );
        ASSERT_TRUE( db->objectByName<Mock3>("m2") == mock2 );

        // Change database, altering tables. Check that objects will not exists        
        sad::db::Table* tbl1 = new sad::db::Table();
        db->addTable("table1", tbl1);
        ASSERT_TRUE( db->table("table1") != nullptr );

        // Restore database and check, that everything is fine
        db->restoreSnapshot();

        ASSERT_TRUE( db->table("table") == tbl );
        ASSERT_TRUE( db->table("table1") == nullptr );
        ASSERT_TRUE( db->table("table2") == nullptr );        
        ASSERT_TRUE( db->objectByName<Mock3>("m1") == mock1 );
        ASSERT_TRUE( db->objectByName<Mock3>("m2") == mock2 );
    }
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void test_snapshot_change_objects()
    {
        sad::db::ObjectFactory* f = new sad::db::ObjectFactory();
        f->add<Mock3>("Mock3",  new sad::db::schema::Schema());

        sad::db::Database * db = new sad::db::Database();
        db->setFactory(f);

        sad::Renderer r;
        r.addDatabase("", db);

        // Fill initial data for snapshots
        sad::db::Table* tbl = new sad::db::Table();
        db->addTable("table", tbl);

        Mock3* mock1 = new Mock3();
        mock1->setIdC(22);
        mock1->setObjectName("m1");

        Mock3* mock2 = new Mock3();
        mock2->setIdC(23);
        mock2->setObjectName("m2");
        tbl->add(mock1);
        tbl->add(mock2);
        db->saveSnapshot();

        ASSERT_TRUE( db->objectByName<Mock3>("m1") == mock1 );
        ASSERT_TRUE( db->objectByName<Mock3>("m2") == mock2 );

        // Change database, altering tables. Check that objects will not exists  
        mock2->setId(25);
        tbl->remove(mock1);
        Mock3* mock3 = new Mock3();
        mock3->setIdC(24);
        mock3->setObjectName("m3");
        tbl->add(mock3);
        ASSERT_TRUE( db->objectByName<Mock3>("m1") == nullptr );
        ASSERT_TRUE( db->objectByName<Mock3>("m2") == mock2 );
        ASSERT_TRUE( mock2->id_c() == 25 );
        ASSERT_TRUE( db->objectByName<Mock3>("m3") != nullptr );
        
        // Restore database and check, that everything is fine
        db->restoreSnapshot();

        ASSERT_TRUE( db->table("table") == tbl );
        ASSERT_TRUE( db->objectByName<Mock3>("m1") != nullptr );
        ASSERT_TRUE( db->objectByName<Mock3>("m2") == mock2 );
        ASSERT_TRUE( mock2->id_c() == 23 );
        ASSERT_TRUE( db->objectByName<Mock3>("m3") == nullptr );
    }
    
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void test_snapshot_reset_objects()
    {
        sad::db::ObjectFactory* f = new sad::db::ObjectFactory();
        f->add<Mock3>("Mock3",  new sad::db::schema::Schema());

        sad::db::Database * db = new sad::db::Database();
        db->setFactory(f);

        sad::Renderer r;
        r.addDatabase("", db);

        // Fill initial data for snapshots
        sad::db::Table* tbl = new sad::db::Table();
        db->addTable("table", tbl);

        Mock3* mock1 = new Mock3();
        mock1->setIdC(22);
        mock1->setObjectName("m1");

        Mock3* mock2 = new Mock3();
        mock2->setIdC(23);
        mock2->setObjectName("m2");
        tbl->add(mock1);
        tbl->add(mock2);
        db->saveSnapshot();

        ASSERT_TRUE( db->objectByName<Mock3>("m1") == mock1 );
        ASSERT_TRUE( db->objectByName<Mock3>("m2") == mock2 );

        // Change database, altering tables. Check that objects will not exists  
        mock1->setId(32);
        mock2->setId(34);

        
        // Restore database and check, that everything is fine
        db->restoreSnapshot();

        ASSERT_TRUE( db->table("table") == tbl );
        ASSERT_TRUE( db->objectByName<Mock3>("m1") == mock1 );
        ASSERT_TRUE( mock1->id_c() == 22 );
        ASSERT_TRUE( db->objectByName<Mock3>("m2") == mock2 );
        ASSERT_TRUE( mock2->id_c() == 23 );
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void test_animation_easing_functions()
    {
        sad::animations::Color* clr1 = new sad::animations::Color();
        clr1->easing()->setFunctionType(sad::animations::easing::Types::ATTT_OutSine);
        clr1->easing()->setPeriod(2.0);
        clr1->easing()->setOvershootAmplitude(1.0);

        sad::animations::Color* clr2 = new sad::animations::Color();
        clr2->easing()->setFunctionType(sad::animations::easing::Types::ATTT_InSine);
        clr2->easing()->setPeriod(3.0);
        clr2->easing()->setOvershootAmplitude(2.0);


        sad::db::Database* db = new sad::db::Database();
        sad::db::Database* outDb = new sad::db::Database();
        sad::db::Table* table = new sad::db::Table();
        db->addTable("animations", table);

        table->add(clr1);
        table->add(clr2);

        sad::Renderer r;
        r.addDatabase("", db);
        r.addDatabase("out", outDb);

        sad::String output;
        
        db->save(output);
        ASSERT_TRUE( outDb->load(output)  );

        sad::animations::Color* oclr1 = static_cast<sad::animations::Color*>(outDb->queryByMajorId(clr1->MajorId));
        sad::animations::Color* oclr2 = static_cast<sad::animations::Color*>(outDb->queryByMajorId(clr2->MajorId));
    
        ASSERT_TRUE( oclr1 );
        ASSERT_TRUE( oclr1->easing()->functionType() == sad::animations::easing::Types::ATTT_OutSine );
        ASSERT_TRUE( sad::is_fuzzy_equal(oclr1->easing()->period(), 2.0) );
        ASSERT_TRUE( sad::is_fuzzy_equal(oclr1->easing()->overshootAmplitude(), 1.0) );

        ASSERT_TRUE( oclr2 );
        ASSERT_TRUE( oclr2->easing()->functionType() == sad::animations::easing::Types::ATTT_InSine );
        ASSERT_TRUE( sad::is_fuzzy_equal(oclr2->easing()->period(), 3.0) );
        ASSERT_TRUE( sad::is_fuzzy_equal(oclr2->easing()->overshootAmplitude(), 2.0) );
    }
                
} _sad_db_database_test;