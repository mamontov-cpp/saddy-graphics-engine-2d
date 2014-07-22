#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "db/dbobject.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/schema/schema.h"

#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

class Mock3: public sad::Object
{
SAD_OBJECT
public:
	Mock3()
	{
		m_schema.add("prop", new sad::db::Field<Mock3, int>(&Mock3::m_id));
		m_schema.add("prop2", new sad::db::MethodPair<Mock3, int>(&Mock3::id_c, &Mock3::setId));
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
		return const_cast<Mock3&>(*this).m_id;
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
		const_cast<Mock3 *>(this)->m_id = i;
	}

	void setIdR(int & i)
	{
		m_id = i;
	}

	void setIdRC(int & i) const
	{
		const_cast<Mock3 *>(this)->m_id = i;
	}

	void setIdCR(const int & i)
	{
		m_id = i;
	}

	void setIdCRC(const int & i) const
	{
		const_cast<Mock3 *>(this)->m_id = i;
	}

	int m_id;

	virtual sad::db::schema::Schema* schema() const
	{
		return &(const_cast<Mock3*>(this)->m_schema);
	}

	sad::db::schema::Schema m_schema;
};

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(Mock3);
DECLARE_SOBJ_INHERITANCE(Mock3, sad::Object)

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
