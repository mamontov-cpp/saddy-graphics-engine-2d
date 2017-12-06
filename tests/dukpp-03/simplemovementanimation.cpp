#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdio>
#include "dukpp-03/context.h"
#include "sprite2d.h"
#include "fuzzyequal.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

/*! Tests for SimpleMovement class
*/
struct SimpleMovementAnimationTest : tpunit::TestFixture
{
public:
	SimpleMovementAnimationTest() : tpunit::TestFixture(
		TEST(SimpleMovementAnimationTest::testSetStartingPoint),
		TEST(SimpleMovementAnimationTest::testSetEndingPoint)
	) {}

	/*! Test for getting and setting startingPoint property
	*/
	// ReSharper disable once CppMemberFunctionMayBeStatic
	// ReSharper disable once CppMemberFunctionMayBeConst
	void testSetStartingPoint() {
		std::string error;
		sad::dukpp03::Context ctx;
		bool eval_result = ctx.eval("var b = new sad.animations.SimpleMovement(); b.setStartingPoint(new sad.Point2D(1,5)); b.startingPoint()", false, &error);
		if (!eval_result)
		{
			printf("%s\n", error.c_str());
		}
		ASSERT_TRUE(eval_result);
		ASSERT_TRUE(error.size() == 0);
		::dukpp03::Maybe<sad::Point2D> result = ::dukpp03::GetValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(&ctx, -1);
		ASSERT_TRUE(result.exists());
		ASSERT_TRUE(sad::equal(result.value(), sad::Point2D(1, 5)));
	}

	/*! Test for getting and setting endingPoint property
	*/
	// ReSharper disable once CppMemberFunctionMayBeStatic
	// ReSharper disable once CppMemberFunctionMayBeConst
	void testSetEndingPoint() {
		std::string error;
		sad::dukpp03::Context ctx;
		bool eval_result = ctx.eval("var b = new sad.animations.SimpleMovement(); b.setEndingPoint(new sad.Point2D(2,8)); b.endingPoint()", false, &error);
		if (!eval_result)
		{
			printf("%s\n", error.c_str());
		}
		ASSERT_TRUE(eval_result);
		ASSERT_TRUE(error.size() == 0);
		::dukpp03::Maybe<sad::Point2D> result = ::dukpp03::GetValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(&ctx, -1);
		ASSERT_TRUE(result.exists());
		ASSERT_TRUE(sad::equal(result.value(), sad::Point2D(2, 8)));
	}

} _simplemovementanimation_test;