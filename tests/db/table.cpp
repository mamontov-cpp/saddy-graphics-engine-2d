#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "db/dbobject.h"
#include "db/dbtable.h"
#include "db/schema/schema.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbobjectfactory.h"

#include "mock3.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

/*!
 * Tests sad::db::Table
 */
struct SadDbTableTest : tpunit::TestFixture
{
public:
    SadDbTableTest() : tpunit::TestFixture(
        TEST(SadDbTableTest::test_add),
        TEST(SadDbTableTest::test_remove),
        TEST(SadDbTableTest::test_query_by_major_id),
        TEST(SadDbTableTest::test_query_by_minor_id),
        TEST(SadDbTableTest::test_query_by_name),
        TEST(SadDbTableTest::test_query_by_id),
        TEST(SadDbTableTest::test_save),
        TEST(SadDbTableTest::test_load_invalid),
        TEST(SadDbTableTest::test_load_valid)
    ) {}
   
    void test_add()
    {
        Mock3 * mock = new Mock3();
        mock->setObjectName("test");
        mock->MajorId = 2;
        
        sad::db::Table * t = new sad::db::Table();
        t->add(mock);
        
        ASSERT_TRUE(t->queryByMinorId(mock->MinorId)->objectName() == "test");
        ASSERT_TRUE(t->queryByMinorId(222) == nullptr);
        
        // Replace old mock to test object replacement in adding items
        Mock3 * mock2 = new Mock3();
        mock2->setObjectName("test2");
        mock2->MinorId = mock->MinorId;
        t->add(mock2);
        
        ASSERT_TRUE(t->queryByMinorId(mock2->MinorId)->objectName() == "test2");

        delete t;
    }
    
    void test_remove()
    {
        Mock3 * mock = new Mock3();
        mock->setObjectName("test");
        mock->MajorId = 2;
        
        sad::db::Table * t = new sad::db::Table();
        t->add(mock);
        
        ASSERT_TRUE(t->queryByMinorId(mock->MinorId)->objectName() == "test");


        unsigned long long oldminorid = mock->MinorId;
                
        // Remove old mock
        t->remove(mock);
        
        ASSERT_TRUE(t->queryByMinorId(oldminorid) == nullptr);

        delete t;
    }
    
    void test_query_by_major_id()
    {
        Mock3 * mock = new Mock3();
        mock->setObjectName("test");
        mock->MinorId = 1;
        mock->MajorId = 2;
        
        sad::db::Table * t = new sad::db::Table();
        t->add(mock);
        
        ASSERT_TRUE(t->queryByMajorId(mock->MajorId)->objectName() == "test");
        ASSERT_TRUE(t->queryByMajorId(155) == nullptr);
        
        delete t;
    }
    
    void test_query_by_minor_id()
    {
        Mock3 * mock = new Mock3();
        mock->setObjectName("test");
        mock->MinorId = 1;
        mock->MajorId = 2;
        
        sad::db::Table * t = new sad::db::Table();
        t->add(mock);
        
        ASSERT_TRUE(t->queryByMinorId(mock->MinorId)->objectName() == "test");
        ASSERT_TRUE(t->queryByMinorId(155) == nullptr);
        
        delete t;
    }
    
    void test_query_by_name()
    {
        Mock3 * mock = new Mock3();
        mock->setObjectName("test");
        mock->MinorId = 1;
        mock->MajorId = 2;
        
        sad::db::Table * t = new sad::db::Table();
        t->add(mock);
        
        ASSERT_TRUE(t->queryByName(mock->objectName())[0]->MajorId == mock->MajorId);
        ASSERT_TRUE(t->queryByName("a").size() == 0);
        
        delete t;
    }
    
    void test_query_by_id()
    {
        
        Mock3 * mock = new Mock3();
        mock->setObjectName("test");
        mock->MinorId = 1;
        mock->MajorId = 2;
        
        sad::db::Table * t = new sad::db::Table();
        t->add(mock);
        
        ASSERT_TRUE(t->queryById(mock->MajorId, mock->MinorId)->objectName() == mock->objectName());
        ASSERT_TRUE(t->queryById(mock->MajorId, 22) == nullptr);
        ASSERT_TRUE(t->queryById(22, mock->MinorId) == nullptr);
        ASSERT_TRUE(t->queryById(22, 155) == nullptr);
        
        delete t;
    }
    
    void test_save()
    {
        Mock3 * mock = new Mock3();
        mock->setObjectName("test");
        mock->MinorId = 1;
        mock->MajorId = 2;
        
        Mock3 * mock2 = new Mock3();
        mock2->setObjectName("test2");
        mock2->MinorId = 2;
        mock2->MajorId = 3;
        
        sad::db::Table * t = new sad::db::Table();
        t->add(mock);
        t->add(mock2);
        
        picojson::value v;
        t->save(v);
        delete t;
        
        sad::db::ObjectFactory f;
        f.add<Mock3>("Mock3",  new sad::db::schema::Schema());
        sad::db::Table * t2 = new sad::db::Table();
        bool result = t2->load(v, &f);
        ASSERT_TRUE(result);
        delete t2;
    }

    void test_load_invalid()
    {
        sad::db::Table * t = new sad::db::Table();

        sad::db::ObjectFactory f;
        f.add<Mock3>("Mock3",  new sad::db::schema::Schema());

        {
        picojson::value test;
        ASSERT_FALSE(t->load(test, &f));
        }

        {
        picojson::value test(22, false);
        ASSERT_FALSE(t->load(test, &f));
        }

        {
        picojson::value test(std::string("222"));
        ASSERT_FALSE(t->load(test, &f));
        }

        {
        picojson::value test(std::string("222"));
        ASSERT_FALSE(t->load(test, &f));
        }
        
        {
        picojson::value test(picojson::array_type, false);
        test.push_back(picojson::value(picojson::object_type, false));
        ASSERT_FALSE(t->load(test, &f));
        }

        delete t;
    }

    void test_load_valid()
    {
        sad::db::Table * t = new sad::db::Table();

        sad::db::ObjectFactory f;
        f.add<Mock3>("Mock3",  new sad::db::schema::Schema());

        picojson::value test(picojson::array_type, false);
        picojson::value object(picojson::object_type, false);
        object.insert("type", picojson::value("Mock3"));
        object.insert("prop", picojson::value(3.0));
        object.insert("prop2", picojson::value(3.0));
        object.insert("name", picojson::value("test"));
        object.insert("active", picojson::value(true));
        object.insert("majorid", picojson::value(1.0));
        object.insert("minorid", picojson::value(1.0));
        
        test.push_back(object);
        ASSERT_TRUE(t->load(test, &f));
        ASSERT_TRUE(t->queryByMinorId(1)->objectName() == "test");

        delete t;
    }
    
    
} _sad_db_table_test;