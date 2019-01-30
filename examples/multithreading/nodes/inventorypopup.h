/*! \file inventorypopup.h
 *
 *  An inventory popup item, that renders a popup, drawn when hovering over
 *  item
 */
#pragma once
#include <sprite2d.h>
#include <label.h>
#include <sadpoint.h>

#include "../game/inventory.h"


namespace nodes
{

/*! \class nodes::InventoryNode
 *
 *  A basic inventory node, that renders inventory background and it's items
 */
class InventoryPopup : public sad::SceneNode
{
SAD_OBJECT
public:
    /*! An inventory item
     */
    InventoryPopup();
    /*! Frees all inner data
     */
    ~InventoryPopup();

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
    /*! Sets a popup's data about item
        \param[in] name a name about popup
        \param[in] description a description for popup
     */
    void setDescription(const sad::String& name, const sad::String& description);
    /*! Shows popup at some point, where popup should always touch that place
     */
    void showAt(const sad::Point2D& p);
private:
    /*! Recomputes popup position, according current pivot point
     */
    void recomputePopupPosition();
    /*! Sets area for popup
        \param[in] px pivot x point (top-left)
        \param[in] py pivot y point (top-left)
        \param[in] width a width for popup
        \param[in] height a height for popup
     */
    void setAreaForPopup(double px,  double py, double width, double height) const;
    /*! Background is non-copyable, to make implementation simpler
        \param[in] b other background
    */
    InventoryPopup(const nodes::InventoryPopup& b);
    /*! Background is non-copyable, to make implementation simpler
        \param[in] b other background
        \return background
    */
    nodes::InventoryPopup& operator=(const nodes::InventoryPopup& b);

    /*! A background item, must be white
     */
    sad::Sprite2D* m_background_underlay;
    /*! A background overlay, must be brown
     */
    sad::Sprite2D* m_background_overlay;
    /*! A label item
     */
    sad::Label* m_label;
    /*! A pivot point for popup
     */
    sad::Point2D m_pivot_point;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(nodes::InventoryPopup)
