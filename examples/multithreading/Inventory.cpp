#include "inventory.h"

game::Inventory::Inventory() : m_height(10), m_width(20), m_items_count(0)
{

}

game::Inventory::~Inventory() 
{
	for (sad::Hash<int, sad::Hash<int, game::Item *> >::iterator it = m_items.begin(); it != m_items.end(); ++it)
	{
		sad::Hash<int, game::Item *>& items = it.value();
		for (sad::Hash<int, game::Item *>::iterator jt = items.begin(); jt != items.end(); ++jt)
		{
			delete jt.value();
		}
	}
}

game::Item* game::Inventory::getItemByIndex(int i, int j) 
{
	if (m_items[i][j] != NULL)
		return m_items[i][j];
	return NULL;
}

bool game::Inventory::addItem(game::Item* item) 
{
	if (m_items_count == 200)
		return false;
		for (int i = 0; i < m_height; i++) {
			for (int j = 0; j < m_width; j++) {
				if (m_items[i][j] == NULL) {
					m_items[i][j] = item;
					m_items_count++;
					return true;
				}
			}
		}
		return false;
}

void game::Inventory::replaceItem(int i1, int j1, int i2, int j2) 
{
	game::Item * temp = NULL;
	temp = m_items[i1][j1];
	m_items[i1][j1] = m_items[i2][j2];
	m_items[i2][j2] = temp;
}


