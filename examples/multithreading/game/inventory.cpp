#include "inventory.h"

#include "../nodes/inventorynode.h"

#include <geometry2d.h>

const int game::Inventory::Width = 9;

const int game::Inventory::Height = 5; // 6,7-th row is reserved for recycle bin, which should be placed on 7-th row, 9-th place


// ============================================ PUBLIC METHODS  ============================================

game::Inventory::Inventory() : m_items_count(0), m_node(NULL)
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
    if (m_node)
    {
        m_node->clearInventorySprites();
    }
    this->deleteAllItems();
}

game::Inventory::~Inventory()
{
    this->deleteAllItems();
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
                if (m_node)
                {
                    m_node->tryMakeSpriteAndStore(i, j, item);
                }
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
    if (m_node)
    {
        m_node->swapSpritePositions(i1, j1, i2, j2);
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
    if (m_node)
    {
        m_node->tryMakeSpriteAndStore(i, j, item);
    }
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
    if (m_node)
    {
        m_node->eraseSprite(i, j);
    }
    game::Item* item = m_items[i][j];
    m_items[i][j] = NULL;
    return item;;
}

game::Item* game::Inventory::item(int i, int j)
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


const game::Inventory::HashMap& game::Inventory::items() const
{
    return m_items;
}

void game::Inventory::eachExisting(std::function<void(int, int, game::Item*&)> f)
{
    for (game::Inventory::HashMap::iterator it = m_items.begin(); it != m_items.end(); ++it)
    {
        int row = it.key();
        game::Inventory::RowHashMap& rowmap = it.value();
        for (game::Inventory::RowHashMap::iterator jt = rowmap.begin(); jt != rowmap.end(); ++jt)
        {
            int column = jt.key();
            if (jt.value())
            {
                f (row, column, jt.value());
            }
        }
    }
}

game::Item* game::Inventory::getItemWhichIsUnderCursor(const sad::Point2D& p)
{
    for (game::Inventory::HashMap::iterator it = m_items.begin(); it != m_items.end(); ++it)
    {
        int row = it.key();
        game::Inventory::RowHashMap& rowmap = it.value();
        for (game::Inventory::RowHashMap::iterator jt = rowmap.begin(); jt != rowmap.end(); ++jt)
        {
            int column = jt.key();
            game::Item* item = jt.value();
            if (item)
            {
                sad::Sprite2D* sprite = item->sprite();
                if (sprite)
                {
                    if (sad::isWithin(p, sprite->area()))
                    {
                        return item;
                    }
                }
            }
        }
    }
    return NULL;
}

void game::Inventory::setNode(nodes::InventoryNode* node)
{
    m_node = node;
}

nodes::InventoryNode* game::Inventory::node() const
{
    return m_node;
}

// ============================================ PRIVATE METHODS  ============================================


void game::Inventory::deleteAllItems()
{
    for (sad::Hash<int, sad::Hash<int, game::Item *> >::iterator it = m_items.begin(); it != m_items.end(); ++it)
    {
        sad::Hash<int, game::Item *>& items = it.value();
        for (sad::Hash<int, game::Item *>::iterator jt = items.begin(); jt != items.end(); ++jt)
        {
            delete jt.value();
            jt.value() = NULL;
        }
    }
}

game::Inventory::Inventory(const game::Inventory&) : m_items_count(0), m_node(NULL)
{

}

game::Inventory& game::Inventory::operator=(const game::Inventory&)
{
    return *this;
}
