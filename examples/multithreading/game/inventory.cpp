#include "inventory.h"

const int game::Inventory::Width = 9;

const int game::Inventory::Height = 7;


// ============================================ PUBLIC METHODS  ============================================

game::Inventory::Inventory() : m_items_count(0)
{
    for(size_t i = 0; i < game::Inventory::Height; i++)
    {
        m_items.insert(i, sad::Hash<int, game::Item *>());
        for(size_t j = 0; j < game::Inventory::Width; j++)
        {
            m_items[i].insert(j, NULL);
        }
    }
}

void game::Inventory::clear()
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

game::Inventory::~Inventory() 
{
    this->clear();
}

game::Item* game::Inventory::getItemByIndex(int i, int j) 
{
    if (!m_items.contains(i))
    {
        return NULL;
    }
    if (!m_items[i].contains(j))
    {
        return NULL;
    }
    return m_items[i][j];
}

bool game::Inventory::addItem(game::Item* item) 
{
    if (m_items_count == Height * Width)
        return false;
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            if (m_items[i][j] == NULL) {
                m_items[i][j] = item;
                m_items_count++;
                return true;
            }
        }
    }
    return false;
}

bool game::Inventory::replaceItem(int i1, int j1, int i2, int j2) 
{
    if (!m_items.contains(i1) || !m_items.contains(i2))
    {
        return false;
    }
    if (!m_items[i1].contains(j1) || !m_items[i2].contains(j2))
    {
        return false;
    }
    std::swap(m_items[i1][j1], m_items[i2][j2]);
    return true;
}

bool game::Inventory::storeItem(int i, int j, game::Item* item)
{
    if (!m_items.contains(i))
    {
        return false;
    }
    if (!m_items[i].contains(j))
    {
        return false;
    }
    if (m_items[i][j] != NULL)
    {
        return false;
    }
    m_items[i][j] = item;
    return true;
}

game::Item* game::Inventory::takeItem(int i, int j)
{
    if (!m_items.contains(i))
    {
        return NULL;
    }
    if (!m_items[i].contains(j))
    {
        return NULL;
    }
    game::Item* item = m_items[i][j];
    m_items[i][j] = NULL;
    return item;;
}


// ============================================ PRIVATE METHODS  ============================================

game::Inventory::Inventory(const game::Inventory&) : m_items_count(0)
{
    
}

game::Inventory& game::Inventory::operator=(const game::Inventory&)
{
    return *this;
}