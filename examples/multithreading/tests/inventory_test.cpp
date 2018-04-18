#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#define _INC_STDIO
#include "tpunit++.hpp"
#include "../game/inventory.h"
#pragma warning(pop)

struct InventoryTest : tpunit::TestFixture
{
public:
	InventoryTest() : tpunit::TestFixture(
		TEST(InventoryTest::testReplaceItem),
		TEST(InventoryTest::testAddItem),
		TEST(InventoryTest::testGetByIndex)
	) {}

	/*! A common test for exposed API
	*/
	// ReSharper disable once CppMemberFunctionMayBeStatic
	// ReSharper disable once CppMemberFunctionMayBeConst
	void testReplaceItem()
	{
		std::string error;
		game::Inventory inv;
		game::Item stick("stick.jpg", "stick");
		game::Item bank("bank.jpg", "bank");
		inv.addItem(&stick);
		inv.addItem(&bank);
		inv.replaceItem(0,0,0,1);
		if (inv.getItemByIndex(0,1)!= &bank)
		{
			printf("%s\n", error.c_str());
		}
		ASSERT_TRUE(inv.getItemByIndex(0, 1) == &bank);
		/*ASSERT_TRUE(error.size() == 0);
		game::Maybe<bool> result = game::GetValue<bool, game::Inventory>::perform(&inv, -1);
		ASSERT_TRUE(result.exists());
		ASSERT_TRUE(result.value() == 16);*/
	}

	/*! A common test for cloning
	*/
	// ReSharper disable once CppMemberFunctionMayBeStatic
	// ReSharper disable once CppMemberFunctionMayBeConst
	void testAddItem()
	{
		std::string error;
		game::Inventory inv;
		game::Item stick("stick.jpg", "stick");
		bool eval_result = inv.addItem(&stick);
		if (inv.getItemByIndex(0,0)!= &stick)
		{
			printf("%s\n", error.c_str());
		}
		ASSERT_TRUE(inv.getItemByIndex(0, 0) == &stick);
		/*ASSERT_TRUE(error.size() == 0);
		::game::Maybe<bool> result = ::game::GetValue<bool, game::Inventory>::perform(&inv, -1);
		ASSERT_TRUE(result.exists());
		ASSERT_TRUE(result.value() == true);*/
	}

	/*! A common test for .to3D
	*/
	// ReSharper disable once CppMemberFunctionMayBeStatic
	// ReSharper disable once CppMemberFunctionMayBeConst
	void testGetByIndex()
	{
		std::string error;
		game::Inventory inv;
		bool eval_result = inv.getItemByIndex(0, 0);
		if (!eval_result)
		{
			printf("%s\n", error.c_str());
		}
		ASSERT_TRUE(eval_result == true);
		/*ASSERT_TRUE(error.size() == 0);
		::dukpp03::Maybe<int> result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(&ctx, -1);
		ASSERT_TRUE(result.exists());
		ASSERT_TRUE(result.value() == 16);*/
	}




} _inventory_test;