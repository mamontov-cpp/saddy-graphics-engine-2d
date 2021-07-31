/*! \file rendergrids.h
    
    Describes a simple process, that forces rendering all grids in database
 */
#pragma once
#include <pipeline/pipelineprocess.h>
#include <layouts/grid.h>
#include <sadvector.h>
#include <sadcolor.h>
#include <db/dbtable.h>

namespace core
{
class Editor;

namespace borders
{
class ResizeHotspot;    
}

}

namespace gui
{

/*! A simple process, that forces rendering all grids in database
 */ 
class RenderGrids: public sad::pipeline::AbstractProcess
{
public:
    /*! Construct new delegate for rendering
        \param[in] editor editor
     */
    RenderGrids(core::Editor* editor);
    /*! Sets, whether grid rendering is enabled or disabled
        \param[in] enabled enabled
     */
    void setEnabled(bool enabled);
    /*! Cleans up inner cache for rendering grids
     */
    void cleanup() const;
    /*! Returns list of grids from table
        \param[in] t table
        \param[out] grids list of grids 
     */
    static void gridsFromTable(sad::db::Table* t, sad::Vector<sad::layouts::Grid*>& grids);
    /*! Forces grid to synchronize it's cache with database, cleaning up the cache
        and filling it with new grids
     */
    void synchronizeWithDatabase() const;
    /*! Adds new grid to cache
        \param[in] g grid
     */
    void add(sad::layouts::Grid* g) const;
    /*! Inserts a grid to cache
        \param[in] pos a position for grid
        \param[in] g grid
     */
    void insert(int pos, sad::layouts::Grid* g) const;
    /*! Finds a grid in cache, returning it 
        \param[in] g grid
        \return position (-1 if not found)
     */
    int find(sad::layouts::Grid* g) const;
    /*! Removes a grid from cache
        \param[in] g grid
        \return a position of grid in cache (-1 if not found)
     */
    int remove(sad::layouts::Grid* g) const;
    /*! Swaps two grids in cache
        \param[in] g1 first grid
        \param[in] g2 second grid
     */
    void swap(sad::layouts::Grid* g1, sad::layouts::Grid* g2) const;
    /*! Returns selected color for rendered grids
        \return selected color
     */
    static const sad::AColor& selectedColor();
    /*! Returns default color for rendered grids
        \return default color
     */
    static const sad::AColor& defaultColor();
    /*! Returns hotspot for resizing, whether point within one
        \return a hotspot (nullptr if not found)
     */
    core::borders::ResizeHotspot* selectedResizeHotspot(const sad::Point2D& p);
    /*! Enables rendering resizing hotspots
     */
    void enableResizeHotspots();
    /*! Disables rendering resizing hotspots
     */
    void disableResizeHotspots();
    /*! Could be inherited
     */
    virtual ~RenderGrids();
protected:
    /*! A scene
     */
    sad::Scene* m_scene;
    /*! A panel for rendering ways
     */
    core::Editor * m_editor;
    /*! A flag for disabling resize hotspot rendering
     */
    bool m_disable_resize_hotspots;
    /*! A default color for grids
     */
    static sad::AColor m_default_color;
    /*! A color, which will be used to render a color for selected grid
     */
    static sad::AColor m_selected_color;
    /*! Renders grids if enabled
     */ 
    virtual void _process() override;

    /*! A hotspots for resizing grids
     */
    core::borders::ResizeHotspot* m_resize_hotspots[4];
};

}
