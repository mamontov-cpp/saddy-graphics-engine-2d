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

struct SimpleMovementAnimationTest : tpunit::TestFixture
{
public:
	SimpleMovementAnimationTest() : tpunit::TestFixture(
		TEST(SimpleMovementAnimationTest::testSetStartingPoint),
		TEST(SimpleMovementAnimationTest::testSetEndingPoint),
		TEST(SimpleMovementAnimationTest::testSetLooped),
		TEST(SimpleMovementAnimationTest::testSetTime)/*,
		TEST(SimpleMovementAnimationTest::testSetObjectName),
		TEST(SimpleMovementAnimationTest::testObjectName),
		TEST(SimpleMovementAnimationTest::testMajorId),
		TEST(SimpleMovementAnimationTest::testMinorId)*/
	) {}

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
		ASSERT_TRUE(sad::equal(result.value(), sad::Point2D(1,5)));
	}

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
		ASSERT_TRUE(sad::equal(result.value(), sad::Point2D(2,8)));
	}

	void testSetLooped() {
		std::string error;
		sad::dukpp03::Context ctx;
		bool eval_result = ctx.eval("var b = new sad.animations.SimpleMovement(); b.setLooped(false); b.looped()", false, &error);
		if (!eval_result)
		{
			printf("%s\n", error.c_str());
		}
		ASSERT_TRUE(eval_result);
		ASSERT_TRUE(error.size() == 0);
		::dukpp03::Maybe<bool> result = ::dukpp03::GetValue<bool, sad::dukpp03::BasicContext>::perform(&ctx, -1);
		ASSERT_TRUE(result.exists());
		ASSERT_TRUE(result.value()==false);
	}

	void testSetTime() {
		std::string error;
		sad::dukpp03::Context ctx;
		bool eval_result = ctx.eval("var b = new sad.animations.SimpleMovement(); b.setTime(5.0); b.time()", false, &error);
		if (!eval_result)
		{
			printf("%s\n", error.c_str());
		}
		ASSERT_TRUE(eval_result);
		ASSERT_TRUE(error.size() == 0);
		::dukpp03::Maybe<double> result = ::dukpp03::GetValue<double, sad::dukpp03::BasicContext>::perform(&ctx, -1);
		ASSERT_TRUE(result.exists());
		ASSERT_TRUE(result.value() == 5.0);
	}

	/*void testSetObjectName() {
		std::string error;
		sad::dukpp03::Context ctx;
		bool eval_result = ctx.eval("var b = new sad.animations.SimpleMovement(); b.setTime(5.0); b.time()", false, &error);
		if (!eval_result)
		{
			printf("%s\n", error.c_str());
		}
		ASSERT_TRUE(eval_result);
		ASSERT_TRUE(error.size() == 0);
		::dukpp03::Maybe<double> result = ::dukpp03::GetValue<double, sad::dukpp03::BasicContext>::perform(&ctx, -1);
		ASSERT_TRUE(result.exists());
		ASSERT_TRUE(result.value() == 5.0);
	}*/

} _simplemovementanimation_test;