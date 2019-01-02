/*! \file inventory.h
 *
 *  Describes an in-game inventory
 */
#pragma once
#include "item.h"
#include <sadhash.h>
#include <sadrect.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <utility>
#include <functional>

namespace nodes
{
class InventoryNode;
}


namespace game
{

 /*! \class game::Inventory
  *
  *  An inventory for storing each item
  */
class Inventory
{
public:
    /*! An inventory row map
     */
    typedef sad::Hash<int, game::Item *> RowHashMap;
    /*! A list of item as a hash map
     */
    typedef sad::Hash<int, game::Inventory::RowHashMap> HashMap;
    /*! Creates an empty directory
     */
    Inventory();
    /*! Frees all items, deleting them
     */
    virtual ~Inventory();
    /*! Clears all inventory items in inventory
     */
    void clear();
    /*! Resets inventory, clearing it and some local data
     */
    void reset();
    /*! Returns item by it's index in inventory
     *  \param[in] i row
     *  \param[in] j column
     *  \return item or NULL if not found
     */
    game::Item* getItemByIndex(int i, int j);
    /*! Adds item to inventory
     *  \param[in] item an item in inventory
     *  \return false if inventory is full, true otherwise
     */
    bool addItem(game::Item* item);
    /*! Replaces items by positions
     *  \param[in] i1 row for first item
     *  \param[in] j1 column for first item
     *  \param[in] i2 row for second item
     *  \param[in] j2 column for second item
     *  \return false if invalid indexes specified
     */
    bool replaceItem(int i1, int j1, int i2, int j2);
    /*! Tries to store item into inventory
     *  \param[in] i a row index
     *  \param[in] j a column index
     *  \param[in] item an item
     *  \return false if place already taken
     */
    bool storeItem(int i, int j, game::Item* item);
    /*! Takes item, returning it but removing also an item
     *  \param[in] i an index
     *  \param[in] j an index
     *  \return pointer to item or NULL if not found
     */
    game::Item* takeItem(int i, int j);
    /*! Returns an item from inventory
     *  \param[in] i an index for row
     *  \param[in] j an index for column
     *  \return pointer to item or NULL if not found
     */
    game::Item* item(int i, int j);
    /*! Returns an items from inventory
     *  \return an items list from inventory
     */
    const game::Inventory::HashMap& items() const;
    /*! Performs action with each existing item in inventory
     *  \param[in] f function, which is called with row, column and item
     */
    void eachExisting(const std::function<void(int, int, game::Item*&)>& f);
    /*! Returns an item, which is under current cursor
        \param[in] p point
        \return item or nothing, with row and column as first items
     */
    sad::Maybe<sad::Triplet<int, int, game::Item*> > getItemWhichIsUnderCursor(const sad::Point2D& p);
    /*! Sets node for inventory
     *  \param[in] node a node item data
     */
    void setNode(nodes::InventoryNode* node);
    /*! Returns node for inventory
     */
    nodes::InventoryNode* node() const;
    /*! Sets basket area for basket
     *  \param[in] rect area
     */
    void setBasketArea(const sad::Rect2D& rect);
    /*! Tests, whether point in basket's area
     *  \param[in] p point
     *  \return whether point in basket's area
     */
    bool isInBasketArea(const sad::Point2D& p) const;
    /*! Whether we are dragging item
     *  \return true, if we are dragging item
     */
    bool isStartedDraggingItem() const;
    /*! Tries to start dragging item, if can
     *  \param[in] p point, whhere user clicked
     */
    void tryStartDraggingItem(const sad::Point2D& p);
    /*! Tries to move dragged area to current point
     *  \param[in] p point
     */
    void tryMoveDraggedItem(const sad::Point2D& p) const;
    /*! Tries to release dragged item, placing it or removing
     *  \param[in] p point
     */
    void tryReleaseDraggedItem(const sad::Point2D& p);
    /*! Returns true if inventory is full
     *  \return true, if inventory is full
     */
    bool isFull() const;
    /*! A maximal horizontal cells of inventory
     */
    static const int Width;
    /*! A maximal vertical cells of inventory
     */
    static const int Height;
protected:
    /*! A hash-map, storing all of items
     */
    game::Inventory::HashMap m_items;
    /*! An inventory node
     */
    nodes::InventoryNode* m_node;
    /*! An area for basket rectangle
     */
    sad::Rect2D m_basket_rect;
    /*! Whether we started dragging item
     */
    bool m_started_dragging_item;
    /*! An old rectangle area for dragging item
     */
    sad::Rect2D m_old_item_area;
    /*! A clicked point for dragging
     */
    sad::Point2D m_clicked_point_for_dragging;
    /*! An item being dragged
     */
    sad::Triplet<int, int, game::Item*> m_item_being_dragged;
    /*! Whether inventory is full
     */
    int m_free_slots;
private:
    /*! Deletes all items from inventory
     */
    void deleteAllItems();
    /*! A copying is disabled for inventory
     *  \param[in] inventory other inventory
     */
    Inventory(const game::Inventory& inventory);
    /*! A copying is disabled for inventory
     *  \param[in] inventory other inventory
     *  \return result
     */
    game::Inventory& operator=(const game::Inventory& inventory);
};

}
