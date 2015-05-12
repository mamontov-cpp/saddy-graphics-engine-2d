#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "duktape/context.h"
#include "sadpoint.h"
#include "db/save.h"
#include "fuzzyequal.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


struct ContextTest : tpunit::TestFixture
{
public:
	ContextTest() : tpunit::TestFixture(
	   TEST(ContextTest::testInitGet),
	   TEST(ContextTest::testPushGet),
	   TEST(ContextTest::testEvalNormal),
	   TEST(ContextTest::testEvalFail),
	   TEST(ContextTest::testEvalTimeout),
	   TEST(ContextTest::testEvalFromFileNormal),
	   TEST(ContextTest::testEvalFromFileFail)
	) {}

	/*! Tests getting and setting reference data
	 */
	void testInitGet()
	{
		sad::duktape::Context ctx;
		ASSERT_TRUE( sad::duktape::Context::getContext(ctx.context()) == &ctx );
		ASSERT_TRUE( sad::duktape::Context::getContext(ctx.context()) == &ctx );
	}

	/*! Tests pushing and getting values
	 */
	void testPushGet()
	{
		sad::duktape::Context ctx;

		// Common case
		int test_number = 0;
		{
			sad::Point2D pts2d(3, 4);
			sad::duktape::PushValue<sad::Point2D>::perform(&ctx, pts2d, false);
			sad::Maybe<sad::Point2D> mbpts2d =
				sad::duktape::GetValue<sad::Point2D>::perform(&ctx, test_number++);
			ASSERT_TRUE( mbpts2d.exists() );
			ASSERT_TRUE( sad::is_fuzzy_equal(mbpts2d.value().x(), 3));
			ASSERT_TRUE( sad::is_fuzzy_equal(mbpts2d.value().y(), 4));
		}

		// char
		{
			char c = 121;
			sad::duktape::PushValue<char>::perform(&ctx, c, false);
			sad::Maybe<char> maybev = sad::duktape::GetValue<char>::perform(&ctx, test_number++);
			ASSERT_TRUE( maybev.exists());			
			ASSERT_TRUE( maybev.value() == 121);
		}

		// unsigned char
		{
			unsigned char c = 121;
			sad::duktape::PushValue<unsigned char>::perform(&ctx, c, false);
			sad::Maybe<unsigned char> maybev = sad::duktape::GetValue<unsigned char>::perform(&ctx, test_number++);
			ASSERT_TRUE( maybev.exists());			
			ASSERT_TRUE( maybev.value() == 121);
		}

		// int
		{
			int c = 121;
			sad::duktape::PushValue<int>::perform(&ctx, c, false);
			sad::Maybe<int> maybev = sad::duktape::GetValue<int>::perform(&ctx, test_number++);
			ASSERT_TRUE( maybev.exists());			
			ASSERT_TRUE( maybev.value() == 121);
		}

		// unsigned int
		{
			unsigned int c = 121;
			sad::duktape::PushValue<unsigned int>::perform(&ctx, c, false);
			sad::Maybe<unsigned int> maybev = sad::duktape::GetValue<unsigned int>::perform(&ctx, test_number++);
			ASSERT_TRUE( maybev.exists());			
			ASSERT_TRUE( maybev.value() == 121);
		}

		// long
		{
			long c = 121;
			sad::duktape::PushValue<long>::perform(&ctx, c, false);
			sad::Maybe<long> maybev = sad::duktape::GetValue<long>::perform(&ctx, test_number++);
			ASSERT_TRUE( maybev.exists());			
			ASSERT_TRUE( maybev.value() == 121);
		}

		// unsigned long
		{
			unsigned long c = 121;
			sad::duktape::PushValue<unsigned long>::perform(&ctx, c, false);
			sad::Maybe<unsigned long> maybev = sad::duktape::GetValue<unsigned long>::perform(&ctx, test_number++);
			ASSERT_TRUE( maybev.exists());			
			ASSERT_TRUE( maybev.value() == 121);
		}

		// long long
		{
			long long c = 121;
			sad::duktape::PushValue<long long>::perform(&ctx, c, false);
			sad::Maybe<long long> maybev = sad::duktape::GetValue<long long>::perform(&ctx, test_number++);
			ASSERT_TRUE( maybev.exists());			
			ASSERT_TRUE( maybev.value() == 121);
		}

		// unsigned long
		{
			unsigned long long c = 121;
			sad::duktape::PushValue<unsigned long long>::perform(&ctx, c, false);
			sad::Maybe<unsigned long long> maybev = sad::duktape::GetValue<unsigned long long>::perform(&ctx, test_number++);
			ASSERT_TRUE( maybev.exists());			
			ASSERT_TRUE( maybev.value() == 121);
		}

		// bool
		{
			bool c = false;
			sad::duktape::PushValue<bool>::perform(&ctx, c, false);
			sad::Maybe<bool> maybev = sad::duktape::GetValue<bool>::perform(&ctx, test_number++);
			ASSERT_TRUE( maybev.exists());			
			ASSERT_TRUE( maybev.value() == false);
		}

		// float
		{
			float c = 1.5;
			sad::duktape::PushValue<float>::perform(&ctx, c, false);
			sad::Maybe<float> maybev = sad::duktape::GetValue<float>::perform(&ctx, test_number++);
			ASSERT_TRUE( maybev.exists());			
			ASSERT_TRUE( sad::is_fuzzy_equal(maybev.value(), c));
		}

		// double
		{
			double c = 1.5;
			sad::duktape::PushValue<double>::perform(&ctx, c, false);
			sad::Maybe<double> maybev = sad::duktape::GetValue<double>::perform(&ctx, test_number++);
			ASSERT_TRUE( maybev.exists());			
			ASSERT_TRUE( sad::is_fuzzy_equal(maybev.value(), c));
		}

		// long double
		{
			long double c = 1.5;
			sad::duktape::PushValue<long double>::perform(&ctx, c, false);
			sad::Maybe<long double> maybev = sad::duktape::GetValue<long double>::perform(&ctx, test_number++);
			ASSERT_TRUE( maybev.exists());			
			ASSERT_TRUE( sad::is_fuzzy_equal(maybev.value(), c));
		}

		// const char*
		{
			const char* c = "22";
			sad::duktape::PushValue<const char*>::perform(&ctx, c, false);
			sad::Maybe<const char*> maybev = sad::duktape::GetValue<const char*>::perform(&ctx, test_number++);
			ASSERT_TRUE( maybev.exists());			
			ASSERT_TRUE( sad::String(maybev.value()) == "22");
		}

		// sad::String
		{
			const char* c = "23";
			sad::duktape::PushValue<sad::String>::perform(&ctx, c, false);
			sad::Maybe<sad::String> maybev = sad::duktape::GetValue<sad::String>::perform(&ctx, test_number++);
			ASSERT_TRUE( maybev.exists());			
			ASSERT_TRUE( maybev.value() == "23");
		}
	}
	/*! Test for normal evaluation process
	 */
	void testEvalNormal()
	{
		sad::String error;
		sad::duktape::Context ctx;
		bool eval_result = ctx.eval("1 + 1", false, &error);
		ASSERT_TRUE( eval_result );
		ASSERT_TRUE( error.size() == 0 );
		sad::Maybe<int> result = sad::duktape::GetValue<int>::perform(&ctx, -1);
		ASSERT_TRUE( result.exists() );
		ASSERT_TRUE( result.value() == 2);		
	}
	/*! Test for non-compilable code
	 */
	void testEvalFail()
	{
		sad::String error;
		sad::duktape::Context ctx;
		bool eval_result = ctx.eval("1 + ;", true, &error);
		ASSERT_TRUE( !eval_result );
		ASSERT_TRUE( error.size() != 0 );
	}
	/*! Test for timeout
	 */
	void testEvalTimeout()
	{
		sad::String error;
		sad::duktape::Context ctx;
		ctx.setMaximumExecutionTime(1000);
		bool eval_result = ctx.eval("while(true) {}", true, &error);
		ASSERT_TRUE( !eval_result );
		ASSERT_TRUE( error.size() != 0 );
	}
	/*! Tests evaluation from file which just sums two numbers
	 */
	void testEvalFromFileNormal()
	{
		sad::String error;
		sad::duktape::Context ctx;
		bool eval_result = ctx.evalFromFile("tests/duktape/common.js", false, &error);
		ASSERT_TRUE( eval_result );
		ASSERT_TRUE( error.size() == 0 );
		sad::Maybe<int> result = sad::duktape::GetValue<int>::perform(&ctx, -1);
		ASSERT_TRUE( result.exists() );
		ASSERT_TRUE( result.value() == 2);
	}
	/*! Tests evaluation from file on non-existing file
	 */
	void testEvalFromFileFail()
	{
		sad::String error;
		sad::duktape::Context ctx;
		bool eval_result = ctx.evalFromFile("tests/duktape/notexists.js", false, &error);
		ASSERT_TRUE( !eval_result );
		ASSERT_TRUE( error.size() != 0 );
	}
} _context_test;