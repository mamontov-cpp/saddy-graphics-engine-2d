#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "db/dbfield.h"
#include "db/dbobject.h"
#include "db/dbmethodpair.h"
#include "db/schema/schema.h"
#include "db/save.h"
#include "db/load.h"
#include "mock2.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


/*!
 * Tests sad::Controls callbacks working and main workflow
 */
struct SadDbSchemaTest : tpunit::TestFixture
{
 public:
   SadDbSchemaTest() : tpunit::TestFixture(
       TEST(SadDbSchemaTest::testCheck),
       TEST(SadDbSchemaTest::testGet)
   ) {}

   void testCheck()
   {
       sad::db::Property * test1 = new sad::db::Field<Mock2, int>(&Mock2::m_id);
       sad::db::Property * test2 = new sad::db::MethodPair<Mock2, int>(&Mock2::id_c, &Mock2::setId);
       sad::db::schema::Schema schema;

       schema.add("id", test1);
       schema.add("id2", test2);
       
       {
           picojson::value v(picojson::object_type, false);
           v.insert("_", picojson::value(22.0));
           v.insert("_2", picojson::value(22.0));
           ASSERT_FALSE( schema.check(v) );
       }

       {
           picojson::value v(picojson::object_type, false);
           v.insert("id", picojson::value(22.0));
           v.insert("_2", picojson::value(22.0));
           ASSERT_FALSE( schema.check(v) );
       }

       {
           picojson::value v(picojson::object_type, false);
           v.insert("_", picojson::value(22.0));
           v.insert("id2", picojson::value(22.0));
           ASSERT_FALSE( schema.check(v) );
       }

       {
           picojson::value v(picojson::object_type, false);
           v.insert("id", picojson::value(22.0));
           v.insert("id2", picojson::value(22.0));
           ASSERT_TRUE( schema.check(v) );
       }

       {
           picojson::value v(picojson::object_type, false);
           v.insert("id", picojson::value("test1"));
           v.insert("id2", picojson::value(22.0));
           ASSERT_FALSE( schema.check(v) );
       }

       {
           picojson::value v(picojson::object_type, false);
           v.insert("id", picojson::value(22.0));
           v.insert("id2", picojson::value("test2"));
           ASSERT_FALSE( schema.check(v) );
       }

       {
           picojson::value v(picojson::object_type, false);
           v.insert("id", picojson::value("test1"));
           v.insert("id2", picojson::value("test2"));
           ASSERT_FALSE( schema.check(v) );
       }
   }

   void testGet()
   {
        {
            sad::db::schema::Schema s;
            s.add("key", new sad::db::Field<Mock2, int>(&Mock2::m_id));

            ASSERT_TRUE( s.getProperty("key") != nullptr );
            ASSERT_TRUE( s.getProperty("key2")== nullptr );
        }
        // Check getting through parent
        {
            sad::db::schema::Schema current, parent;
            current.addParent(&parent);
            parent.add("key", new sad::db::Field<Mock2, int>(&Mock2::m_id));

            ASSERT_TRUE( current.getProperty("key") != nullptr );
            ASSERT_TRUE( current.getProperty("key2")== nullptr );
        }
   }
} _sad_db_schema_schema_test;