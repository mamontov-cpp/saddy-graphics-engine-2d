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
	   TEST(SadDbObjectTest::test_set)
   ) {}

    // By default, object has no schema
	void test_schema()
	{
		sad::db::Object o;
		ASSERT_TRUE(o.schema() == NULL);
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

} _sad_db_object;
