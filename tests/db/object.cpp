#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "db/dbobject.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"

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
	   TEST(SadDbObjectTest::test_schema)
   ) {}

    // By default, object has no schema
	void test_schema()
	{
		sad::db::Object o;
		ASSERT_TRUE(o.schema() == NULL);
	}

} _sad_db_object;
