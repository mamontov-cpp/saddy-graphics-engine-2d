#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "db/dbfield.h"
#include "db/dbobject.h"
#include "db/dbmethodpair.h"
#include "db/dbobjectfactory.h"
#include "mock.h"
#include "mock2.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


sad::db::schema::Schema * _mock_schema;
sad::db::schema::Schema * _mock2_schema;

/*!
 * Tests sad::Controls callbacks working and main workflow
 */
struct SadDbObjectFactoryTest : tpunit::TestFixture
{
 public:
   SadDbObjectFactoryTest() : tpunit::TestFixture(
           TEST(SadDbObjectFactoryTest::test_schema),
           TEST(SadDbObjectFactoryTest::test_object)
   ) {}

	void test_schema()
	{
		sad::db::ObjectFactory f;
		sad::db::schema::Schema s;
		
		
		f.add<Mock>("Mock", &s, false);
		f.add<Mock2>("Mock2",  new sad::db::schema::Schema());
		
		ASSERT_TRUE( f.schema("Mock") != NULL );
		ASSERT_TRUE( f.schema("Mock2") != NULL );
		ASSERT_TRUE( f.schema("Mock3") == NULL );		
	}

	void test_object()
	{
		sad::db::ObjectFactory f;
		sad::db::schema::Schema s;
		
		
		f.add<Mock>("Mock", &s, false);
		f.add<Mock2>("Mock2",  new sad::db::schema::Schema());
		
		sad::db::Object * k;
		k = f.create("Mock");
		ASSERT_TRUE( k != NULL );
		delete k;
		k = f.create("Mock2");
		ASSERT_TRUE( k != NULL );
		delete k;
		ASSERT_TRUE( f.create("Mock3") == NULL );		
	}
	
       
} _sad_object_factory;