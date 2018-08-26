/*! \file inventorynode.h
 *
 *  An inventory node item, that draws a background and items
 */
#pragma once
#include <sprite2d.h>
#include <label.h>

#include "../game/inventory.h"

#include <sadvector.h>

namespace nodes
{

class InventoryPopup;

/*! \class nodes::InventoryNode
 *
 *  A basic inventory node, that renders inventory background and it's items
 */
class InventoryNode : public sad::SceneNode
{
SAD_OBJECT
public:
    /*! An icon size for node
     */
    const static double IconSize;
    /*! An inventory item
        \param[in] inventory an inventory item
     */
    InventoryNode(game::Inventory* inventory);
    /*! Frees all inner data
     */
    ~InventoryNode();

    /*! Renders a sprites, setting a blend functions
     */
    virtual void render();
    /*! Called, when renderer for scene is changed
     */
    virtual void rendererChanged();
    /*! Updates scene for all of background
        \param[in] scene a scene, which will render a node
     */
    virtual void setScene(sad::Scene* scene);

    /*! Called, when inventory is being cleared
     */
    void clearInventorySprites() const;
    /*! Tries to make sprite if not exists for item and store them
     *  \param[in] i a row for an item
     *  \param[in] j a column for an item
     *  \param[in] item an item for store
     */
    void tryMakeSpriteAndStore(int i, int j, game::Item* item);
    /*! Erases sprite for an item with position
     *  \param[in] i a row for an item
     *  \param[in] j a column fir an item
     */
    void eraseSprite(int i, int j);
    /*! Returns an inventory slot icon position
     *  \param[in] row a row for slot icon
     *  \param[in] column a column  for slot icon
     */
    sad::Rect2D getInventoryIconPosition(int row, int column) const;
    /*! Returns an inventory slot position
    *  \param[in] row a row for slot icon
    *  \param[in] column a column  for slot icon
    */
    sad::Rect2D getInventorySlotPosition(int row, int column) const;
    /*! Returns inventory popup
     */
    nodes::InventoryPopup* popup() const;
private:
    /*! Gets position of rectangle in slot
     *  \param[in] row a row
     *  \param[in] column a column
     *  \param[in] input_width a width
     *  \param[in] input_height a height data
     */
    sad::Rect2D getPositionInSlot(int row, int column, double input_width, double input_height) const;
    /*! Background is non-copyable, to make implementation simpler
     *  \param[in] b other background
     */
    InventoryNode(const nodes::InventoryNode& b);
    /*! Background is non-copyable, to make implementation simpler
     *  \param[in] b other background
     *  \return background
     */
    nodes::InventoryNode& operator=(const nodes::InventoryNode& b);

    /*! An inventory data
     */
    game::Inventory* m_inventory;
    /*! A background item
     */
    sad::Sprite2D* m_background;
    /*! A label item
     */
    sad::Label* m_label;
    /*! A slot item, used to draw inventory slots
     */
    sad::Sprite2D* m_slot;
    /*! A basket item sprite
     */
    sad::Sprite2D* m_basket_item;
    /*! An inventory popup
     */
    nodes::InventoryPopup* m_popup;
    /*! Whether we already computed inventory slots places
     */
    bool m_computed_inventory_slots_places;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(nodes::InventoryNode)
