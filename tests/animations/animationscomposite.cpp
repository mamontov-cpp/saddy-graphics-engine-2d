#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#include <label.h>
#include <animations/animationscomposite.h>
#include <animations/animationssequential.h>
#include <animations/animationsparallel.h>
#include <db/dbtable.h>
#include <db/dbdatabase.h>
#pragma warning(pop)

/*! Tests for composite animation
*/

struct CompositeAnimationsTest : tpunit::TestFixture
{
public:
	CompositeAnimationsTest() : tpunit::TestFixture(
		TEST(CompositeAnimationsTest::testAddSequental),
		TEST(CompositeAnimationsTest::testInsertSequental),
		TEST(CompositeAnimationsTest::testAddParallel),
		TEST(CompositeAnimationsTest::testInsertParallel),
		TEST(CompositeAnimationsTest::testAddWithoutException),
		TEST(CompositeAnimationsTest::testInsertWithoutException)
	) {}


	// ReSharper disable once CppMemberFunctionMayBeStatic
	// ReSharper disable once CppMemberFunctionMayBeConst
	void testAddSequental()
	{
		sad::animations::Sequential* a = new sad::animations::Sequential();
		sad::animations::Sequential* b = new sad::animations::Sequential();
		std::logic_error ExpectedException("Objects looped, parent of itself");
		try
		{
			a->add(b);
			b->add(a);
			ASSERT_FALSE(1);
		}
		catch (std::logic_error ExpectedException)
		{
			ASSERT_TRUE(1);
		}	
	}

	void testInsertSequental()
	{
		sad::animations::Sequential* a = new sad::animations::Sequential();
		sad::animations::Sequential* b = new sad::animations::Sequential();
		std::logic_error ExpectedException("Objects looped, parent of itself");
		try
		{
			a->insert(b,1);
			b->insert(a,0);
			ASSERT_FALSE(1);
		}
		catch (std::logic_error ExpectedException)
		{
			ASSERT_TRUE(1);
		}
	}

	void testAddParallel()
	{
		sad::animations::Parallel* a = new sad::animations::Parallel();
		sad::animations::Parallel* b = new sad::animations::Parallel();
		std::logic_error ExpectedException("Objects looped, parent of itself");
		try
		{
			a->add(b);
			b->add(a);
			ASSERT_FALSE(1);
		}
		catch (std::logic_error ExpectedException)
		{
			ASSERT_TRUE(1);
		}
	}

	void testInsertParallel()
	{
		sad::animations::Parallel* a = new sad::animations::Parallel();
		sad::animations::Parallel* b = new sad::animations::Parallel();
		std::logic_error ExpectedException("Objects looped, parent of itself");
		try
		{
			a->insert(b, 1);
			b->insert(a, 0);
			ASSERT_FALSE(1);
		}
		catch (std::logic_error ExpectedException)
		{
			ASSERT_TRUE(1);
		}
	}

	void testAddWithoutException()
	{
		sad::animations::Parallel* a = new sad::animations::Parallel();
		sad::animations::Parallel* b = new sad::animations::Parallel();
		sad::animations::Parallel* c = new sad::animations::Parallel();
		std::logic_error ExpectedException("Objects looped, parent of itself");
		try
		{
			a->add(b);
			c->add(a);
			ASSERT_FALSE(0);
		}
		catch (std::logic_error ExpectedException)
		{
			ASSERT_TRUE(0);
		}

		
	}

	void testInsertWithoutException()
	{
		sad::animations::Parallel* a = new sad::animations::Parallel();
		sad::animations::Parallel* b = new sad::animations::Parallel();
		sad::animations::Parallel* c = new sad::animations::Parallel();
		std::logic_error ExpectedException("Objects looped, parent of itself");
		try
		{
			a->insert(b, 1);
			c->insert(a, 0);
			ASSERT_FALSE(0);
		}
		catch (std::logic_error ExpectedException)
		{
			ASSERT_TRUE(0);
		}
	}


} _composite_animations_test;
