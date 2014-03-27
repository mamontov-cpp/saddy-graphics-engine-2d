#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

class Mock: public sad::Object
{
SAD_OBJECT
public:
	Mock()
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
		return const_cast<Mock&>(*this).m_id;
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
		const_cast<Mock *>(this)->m_id = i;
	}

	void setIdR(int & i)
	{
		m_id = i;
	}

	void setIdRC(int & i) const
	{
		const_cast<Mock *>(this)->m_id = i;
	}

	void setIdCR(const int & i)
	{
		m_id = i;
	}

	void setIdCRC(const int & i) const
	{
		const_cast<Mock *>(this)->m_id = i;
	}

	int m_id;
};

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(Mock);
DECLARE_SOBJ_INHERITANCE(Mock, sad::Object)

/*!
 * Tests sad::Controls callbacks working and main workflow
 */
struct SadDbPropertyTest : tpunit::TestFixture
{
 public:
   SadDbPropertyTest() : tpunit::TestFixture(
	   TEST(SadDbPropertyTest::testField),
	   TEST(SadDbPropertyTest::testMethodPair)
   ) {}

   	int test;

	void testField()
	{
		Mock m;
		
		sad::db::Property * test1 = new sad::db::Field<Mock, int>(&Mock::m_id);
		test1->setObject(&m);

		test1->set(3);
		ASSERT_TRUE(test1->get<int>().value() == 3);

		delete test1;
	}

	void processProperty(Mock &m, sad::db::Property * prop)
	{
		prop->setObject(&m);
		
		test++;

		prop->set(test);
		ASSERT_TRUE(prop->get<int>().value() == test);

		delete prop;
	}

	void testMethodPair()
	{
		Mock m;

		test = 3;

		sad::db::Property * p = new sad::db::MethodPair<Mock, int>(&Mock::id, &Mock::setId);
		processProperty(m, p);

		p = new sad::db::MethodPair<Mock, int>(&Mock::id_c, &Mock::setId);
		processProperty(m, p);

		p = new sad::db::MethodPair<Mock, int>(&Mock::id_r, &Mock::setId);
		processProperty(m, p);

		p = new sad::db::MethodPair<Mock, int>(&Mock::id_rc, &Mock::setId);
		processProperty(m, p);

		p = new sad::db::MethodPair<Mock, int>(&Mock::id_cr, &Mock::setId);
		processProperty(m, p);

		p = new sad::db::MethodPair<Mock, int>(&Mock::id_crc, &Mock::setId);
		processProperty(m, p);

		p = new sad::db::MethodPair<Mock, int>(&Mock::id, &Mock::setIdC);
		processProperty(m, p);

		p = new sad::db::MethodPair<Mock, int>(&Mock::id, &Mock::setIdR);
		processProperty(m, p);

		p = new sad::db::MethodPair<Mock, int>(&Mock::id, &Mock::setIdRC);
		processProperty(m, p);

		p = new sad::db::MethodPair<Mock, int>(&Mock::id, &Mock::setIdCR);
		processProperty(m, p);

		p = new sad::db::MethodPair<Mock, int>(&Mock::id, &Mock::setIdCRC);
		processProperty(m, p);
	}

} _sad_property;