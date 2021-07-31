/*! \file selectionborder.h
    

    Defines an selection border, which is used for highlighting a selected object
 */
#pragma once
#include "border.h"
#include "deletehotspot.h"
#include "resizehotspot.h"

namespace core
{
    
namespace borders
{

/*! Defines an selection border, which is used, when item is selected
 */
class SelectionBorder: public core::borders::Border
{
public:
    /*! Creates new border, with shared data
        \param[in] data specified data
     */
    SelectionBorder(core::Shared* data);
    /*! Can be inherited
     */
    virtual ~SelectionBorder() override;
    /*! Whether we should show hotspots
        \param[in] flag a flag for hotspots
     */
    void toggleShowHotspot(bool flag);
    /*! Returns a hot spot for removal of object
        \return hotspot
     */
    core::borders::DeleteHotspot* deleteHotspot();
    /*! Returns hotspot for resizing, whether point within one
        \return a hotspot (nullptr if not found)
     */
    core::borders::ResizeHotspot* selectedResizeHotspot(const sad::Point2D& p);
protected:
     /*! Renders a selected object border as render border
      */
     void _process() override;
     /*! A delete hotspot for deletion
      */
     core::borders::DeleteHotspot* m_delete_hotspot;
     /*! A hotspots for resizing elements
      */
     core::borders::ResizeHotspot* m_resize_hotspots[4];
     /*! Whether border should show hotspots
      */
     bool m_show_hotspots;
};

}

}
