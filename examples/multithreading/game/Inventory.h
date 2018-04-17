#pragma once
#include "../../examples/game/game.h"
#include "item.h"
#include "sadhash.h"

namespace game 
{

class Inventory 
{
public:
	Inventory();
	virtual ~Inventory();
	game::Item* getItemByIndex(int i, int j);
	bool addItem(Item* item);
	void replaceItem(int i1, int j1, int i2, int j2);
protected:
	sad::Hash<int, sad::Hash<int, game::Item *> > m_items;
	int m_height;
	int m_width;
	int m_items_count;
};

}