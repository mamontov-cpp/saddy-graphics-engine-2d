#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)



/*!
 * Tests sad::Controls callbacks working and main workflow
 */
struct SadDbVariantTest : tpunit::TestFixture
{
 public:
   SadDbVariantTest() : tpunit::TestFixture(
	   TEST(SadDbVariantTest::test)
   ) {}

	void test()
	{
		sad::db::Variant v((int)22);

		ASSERT_TRUE(v.get<int>().value() == 22);
		ASSERT_TRUE(v.get<unsigned int>().value() == 22);

		v.set<int>(25);

		ASSERT_TRUE(v.get<int>().value() == 25);
	}

} _sad_db_variant;
