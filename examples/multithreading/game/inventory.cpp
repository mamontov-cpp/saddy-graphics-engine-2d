#include "inventory.h"

#include "../nodes/inventorynode.h"

#include <geometry2d.h>

const int game::Inventory::Width = 9;

const int game::Inventory::Height = 5; // 6,7-th row is reserved for recycle bin, which should be placed on 7-th row, 9-th place


// ============================================ PUBLIC METHODS  ============================================

game::Inventory::Inventory() : m_node(NULL), m_started_dragging_item(false)
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

void game::Inventory::reset()
{
    this->clear();
    this->setNode(NULL);

    m_started_dragging_item = false;
    m_item_being_dragged.set3(NULL);
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
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            if (m_items[i][j] == NULL) {
                m_items[i][j] = item;
                item->notifyAdded();
                if (m_node)
                {
                    m_node->tryMakeSpriteAndStore(i, j, item);
                }
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
    if (m_node)
    {
        m_node->tryMakeSpriteAndStore(i1, j1, m_items[i1][j1]);
        m_node->tryMakeSpriteAndStore(i2, j2, m_items[i2][j2]);
    }
    

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
    if (item)
    {
        item->notifyRemoved();
    }
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

void game::Inventory::eachExisting(const std::function<void(int, int, game::Item*&)>& f)
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

sad::Maybe<sad::Triplet<int, int, game::Item*> > game::Inventory::getItemWhichIsUnderCursor(const sad::Point2D& p)
{
    sad::Maybe<sad::Triplet<int, int, game::Item*> > result;
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
                        result.setValue(sad::Triplet<int, int, game::Item*>(row, column, item));
                        return result;
                    }
                }
            }
        }
    }
    return result;
}

void game::Inventory::setNode(nodes::InventoryNode* node)
{
    m_node = node;
}

nodes::InventoryNode* game::Inventory::node() const
{
    return m_node;
}

void game::Inventory::setBasketArea(const sad::Rect2D& rect)
{
    m_basket_rect = rect;
}

bool game::Inventory::isInBasketArea(const sad::Point2D& p) const
{
    return sad::isWithin(p, m_basket_rect);
}

bool game::Inventory::isStartedDraggingItem() const
{
    return m_started_dragging_item;
}

void game::Inventory::tryStartDraggingItem(const sad::Point2D& p)
{
   if (!m_started_dragging_item)
   {
       sad::Maybe<sad::Triplet<int, int, game::Item*> > result_data = this->getItemWhichIsUnderCursor(p);
       if (result_data.exists())
       {
           game::Item* item = result_data.value().p3();
           if (item->sprite())
           { 
               m_item_being_dragged = result_data.value();
               m_started_dragging_item = true;
               m_old_item_area = item->sprite()->area();
               m_clicked_point_for_dragging = p;
               sad::Scene* scene = item->sprite()->scene();
               if (scene)
               {
                   scene->setLayer(item->sprite(), scene->objectCount() - 1);
               }
           }
       }
   }
}

void game::Inventory::tryMoveDraggedItem(const sad::Point2D& p) const
{
    if (m_started_dragging_item)
    {
        sad::Point2D diff = p - m_clicked_point_for_dragging;
        sad::Rect2D new_area  = m_old_item_area;
        sad::moveBy(diff, new_area);
        m_item_being_dragged.p3()->sprite()->setArea(new_area);
    }
}

void game::Inventory::tryReleaseDraggedItem(const sad::Point2D& p)
{
    tryMoveDraggedItem(p);
    if (!this->node())
    {
        return;
    }
    if (m_started_dragging_item)
    {
        m_started_dragging_item = false;
        for (int new_row = 0; new_row < game::Inventory::Height; ++new_row)
        {
            for(int new_column = 0; new_column < game::Inventory::Width; ++new_column)
            {
                sad::Rect2D rect = this->node()->getInventorySlotPosition(new_row, new_column);
                if (sad::isWithin(p, rect))
                {
                    this->replaceItem(m_item_being_dragged.p1(), m_item_being_dragged.p2(), new_row, new_column);
                    return;
                }
            }
        }
        if (this->isInBasketArea(p))
        {
            delete this->takeItem(m_item_being_dragged.p1(), m_item_being_dragged.p2());
        }
        else
        {
            m_item_being_dragged.p3()->sprite()->setArea(m_old_item_area);
        }
    }
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

game::Inventory::Inventory(const game::Inventory&) : m_node(NULL), m_started_dragging_item(false)
{

}

game::Inventory& game::Inventory::operator=(const game::Inventory&)
{
    return *this;
}
