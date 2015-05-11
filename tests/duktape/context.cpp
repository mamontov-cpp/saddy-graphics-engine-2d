#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "duktape/context.h"
#include "db/save.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


struct ContextTest : tpunit::TestFixture
{
public:
	ContextTest() : tpunit::TestFixture(
	   TEST(ContextTest::testInitGet)	
	) {}

	/*! Tests getting and setting reference data
	 */
	void testInitGet()
	{
		sad::duktape::Context ctx;
		ASSERT_TRUE( sad::duktape::Context::getContext(ctx.context()) == &ctx );
		ASSERT_TRUE( sad::duktape::Context::getContext(ctx.context()) == &ctx );
	}
} _context_test;