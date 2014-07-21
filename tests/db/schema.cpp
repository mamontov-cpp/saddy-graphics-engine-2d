#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/schema/schema.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

class Mock2: public sad::Object
{
SAD_OBJECT
public:
	Mock2()
	{
		
	}
	
	int id()
	{
		return m_id;
	}
	int id_c() const
	{
		return m_id;
	}

	int& id_r()
	{
		return m_id;
	}

	int& id_rc() const
	{
		return const_cast<Mock2&>(*this).m_id;
	}

	const int& id_cr()
	{
		return m_id;
	}

	const int& id_crc() const
	{
		return m_id;
	}

	void setId(int i)
	{
		m_id = i;
	}

	void setIdC(int i) const
	{
		const_cast<Mock2 *>(this)->m_id = i;
	}

	void setIdR(int & i)
	{
		m_id = i;
	}

	void setIdRC(int & i) const
	{
		const_cast<Mock2 *>(this)->m_id = i;
	}

	void setIdCR(const int & i)
	{
		m_id = i;
	}

	void setIdCRC(const int & i) const
	{
		const_cast<Mock2 *>(this)->m_id = i;
	}

	int m_id;
};

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(Mock2);
DECLARE_SOBJ_INHERITANCE(Mock2, sad::Object)

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

			ASSERT_TRUE( s.getProperty("key") != NULL );
			ASSERT_TRUE( s.getProperty("key2")== NULL );
		}
		// Check getting through parent
		{
			sad::db::schema::Schema current, parent;
			current.setParent(&parent);
			parent.add("key", new sad::db::Field<Mock2, int>(&Mock2::m_id));

			ASSERT_TRUE( current.getProperty("key") != NULL );
			ASSERT_TRUE( current.getProperty("key2")== NULL );
		}
   }
} _sad_db_schema_schema_test;