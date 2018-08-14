/*! \file inventorynode.h
 *
 *  An inventory node item, that draws a background and items
 */
#pragma once
#include <sprite2d.h> 
#include <label.h>

#include "../game/inventory.h"

namespace nodes
{

/*! \class nodes::InventoryNode
 *
 *  A basic inventory node, that renders inventory background and it's items
 */
class InventoryNode : public sad::SceneNode
{
SAD_OBJECT
public:
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
private:
    /*! Background is non-copyable, to make implementation simpler
        \param[in] b other background
    */
    InventoryNode(const nodes::InventoryNode& b);
    /*! Background is non-copyable, to make implementation simpler
        \param[in] b other background
        \return background
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
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(nodes::InventoryNode)