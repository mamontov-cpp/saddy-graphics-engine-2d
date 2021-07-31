#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "db/dbobject.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/schema/schema.h"
#include "db/save.h"
#include "db/load.h"

#include "mock3.h"
#include "mock4.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


/*!
 * Tests sad::Controls callbacks working and main workflow
 */
struct SadDbObjectTest : tpunit::TestFixture
{
 public:
   SadDbObjectTest() : tpunit::TestFixture(
       TEST(SadDbObjectTest::test_schema),
       TEST(SadDbObjectTest::test_get),
       TEST(SadDbObjectTest::test_set),
       TEST(SadDbObjectTest::test_save),
       TEST(SadDbObjectTest::test_load_valid),
       TEST(SadDbObjectTest::test_load_absent_field),
       TEST(SadDbObjectTest::test_load_absent_field2),
       TEST(SadDbObjectTest::test_load_invalid_type),
       TEST(SadDbObjectTest::test_load_invalid_entry),
       TEST(SadDbObjectTest::test_load_nonrequired_field_specified),
       TEST(SadDbObjectTest::test_load_nonrequired_field_notspecified)	   
   ) {}

    // By default, object has no schema
    void test_schema()
    {
        sad::db::Object o;
        // Actually object has own schema, which must be inherited
        ASSERT_TRUE(o.schema() != nullptr);
    }

    void test_get()
    {
        Mock3 m;
        sad::db::schema::Schema s;
        m.m_id = 3;
        
        ASSERT_FALSE( m.getProperty<int>("not_exists").exists() );
        
        ASSERT_TRUE( m.getProperty<int>("prop").value() == 3 );		
        ASSERT_FALSE( m.getProperty<bool>("prop").exists() );

        ASSERT_TRUE( m.getProperty<int>("prop2").value() == 3 );		
        ASSERT_FALSE( m.getProperty<bool>("prop2").exists() );
    }

   void test_set()
   {
       Mock3 m;
       sad::db::schema::Schema s;
       m.m_id = 3;
        
       ASSERT_FALSE( m.setProperty<int>("not_exists", 2));
        
       ASSERT_TRUE( m.setProperty<int>("prop", 4) );		
       ASSERT_FALSE( m.setProperty<bool>("prop", false) );
       ASSERT_TRUE( m.m_id == 4 );

       ASSERT_TRUE( m.setProperty<int>("prop2", 5) );		
       ASSERT_FALSE( m.setProperty<bool>("prop2", false) );
       ASSERT_TRUE( m.m_id == 5 );
   }

   void test_save()
   {
       Mock3 m;
       m.m_id = 3;
       m.MajorId = 12;
       m.MinorId = 22;
       m.setObjectName("Mock3");
       picojson::value result;
       m.save(result);
       picojson::object r2 = result.get<picojson::object>();
       ASSERT_TRUE( r2["type"].get<std::string>() == "Mock3" );
       ASSERT_TRUE( r2.size() == 7 );
   }

   void test_load_valid()
   {
       Mock3 m;
       picojson::value r(picojson::object_type, false);
       r.insert("majorid", picojson::value(22.0));
       r.insert("minorid", picojson::value(12.0));
       r.insert("name", picojson::value("mock3"));
       r.insert("prop", picojson::value(3.0));
       r.insert("prop2", picojson::value(3.0));
       r.insert("active", picojson::value(true));
       bool result = m.load(r);
       ASSERT_TRUE(result);
       ASSERT_TRUE(m.MajorId == 22);
       ASSERT_TRUE(m.MinorId == 12);
       ASSERT_TRUE(m.objectName() == "mock3");
       ASSERT_TRUE(m.m_id == 3);
   }

   void test_load_absent_field()
   {
       Mock3 m;
       picojson::value r(picojson::object_type, false);
       //r.insert("majorid", picojson::value(22.0));
       r.insert("minorid", picojson::value(12.0));
       r.insert("name", picojson::value("mock3"));
       r.insert("prop", picojson::value(3.0));
       r.insert("prop2", picojson::value(3.0));
       bool result = m.load(r);
       ASSERT_FALSE(result);
   }

   void test_load_absent_field2()
   {
       Mock3 m;
       picojson::value r(picojson::object_type, false);
       r.insert("majorid", picojson::value(22.0));
       r.insert("minorid", picojson::value(12.0));
       r.insert("name", picojson::value("mock3"));
       r.insert("prop", picojson::value(3.0));
       //r.insert("prop2", picojson::value(3.0));
       bool result = m.load(r);
       ASSERT_FALSE(result);
   }

   void test_load_invalid_type()
   {
       Mock3 m;
       picojson::value r(picojson::object_type, false);
       r.insert("majorid", picojson::value("nullptr2"));
       r.insert("minorid", picojson::value(12.0));
       r.insert("name", picojson::value("mock3"));
       r.insert("prop", picojson::value(3.0));
       r.insert("prop2", picojson::value(3.0));
       bool result = m.load(r);
       ASSERT_FALSE(result);
   }

   void test_load_invalid_entry()
   {
       Mock3 m;
       picojson::value r(22.0);
       bool result = m.load(r);
       ASSERT_FALSE(result);
   }
   
   void test_load_nonrequired_field_specified()
   {
       Mock4 m;
       picojson::value r(picojson::object_type, false);
       r.insert("majorid", picojson::value(22.0));
       r.insert("minorid", picojson::value(12.0));
       r.insert("name", picojson::value("mock4"));
       r.insert("prop", picojson::value(500.0));
       r.insert("active", picojson::value(true));
       bool result = m.load(r);
       ASSERT_TRUE(result);
       ASSERT_TRUE(m.MajorId == 22);
       ASSERT_TRUE(m.MinorId == 12);
       ASSERT_TRUE(m.objectName() == "mock4");
       ASSERT_TRUE(m.m_id == 500);
   }
   
   void test_load_nonrequired_field_notspecified()
   {
       Mock4 m;
       picojson::value r(picojson::object_type, false);
       r.insert("majorid", picojson::value(22.0));
       r.insert("minorid", picojson::value(12.0));
       r.insert("name", picojson::value("mock4"));
       r.insert("active", picojson::value(true));
       bool result = m.load(r);
       ASSERT_TRUE(result);
       ASSERT_TRUE(m.MajorId == 22);
       ASSERT_TRUE(m.MinorId == 12);
       ASSERT_TRUE(m.objectName() == "mock4");
       ASSERT_TRUE(m.m_id == 22);
   }

} _sad_db_object;
