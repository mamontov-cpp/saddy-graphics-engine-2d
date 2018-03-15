#pragma once
#include "game.h"
#include "item.h"

Class game::Inventory{
public:
	Item::Item items[int][int];
	Inventory();
	~Inventory();
	
	Item::item getItemByIndex(int i, int j);
	void addItem(Item::Item);
	bool checkCell();
	void replaceItem(Item);
}