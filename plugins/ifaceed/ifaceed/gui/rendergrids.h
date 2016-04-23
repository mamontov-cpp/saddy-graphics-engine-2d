/*! \file rendergrids.h
    
    Describes a simple process, that forces rendering all grids in database
 */
#pragma once
#include <pipeline/pipelineprocess.h>
#include <layouts/grid.h>
#include <sadvector.h>
#include <sadcolor.h>

namespace core
{
class Editor;
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
    void cleanup();
    /*! Forces grid to synchronize it's cache with database, cleaning up the cache
        and filling it with new grids
     */
    void synchronizeWithDatabase();
    /*! Adds new grid to cache
        \param[in] g grid
     */
    void add(sad::layouts::Grid* g);
    /*! Inserts a grid to cache
        \param[in] pos a position for grid
        \param[in] g grid
     */
    void insert(int pos, sad::layouts::Grid* g);
    /*! Finds a grid in cache, returning it 
        \param[in] g grid
        \return position (-1 if not found)
     */
    int find(sad::layouts::Grid* g);
    /*! Removes a grid from cache
        \param[in] g grid
        \return a position of grid in cache (-1 if not found)
     */
    int remove(sad::layouts::Grid* g);
    /*! Swaps two grids in cache
        \param[in] pos1 first position
        \param[in] pos2 second position
     */
    void swap(int pos1, int pos2);
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
    /*! Whether sprites are init
     */
    bool m_init;
    /*! A default color for grids
     */
    sad::AColor m_default_color;
    /*! A color, which will be used to render a color for selected grid
     */
    sad::AColor m_selected_color;
    /*! An inner list of grids
     */
    sad::Vector<sad::layouts::Grid*> m_grid;
    /*! Invokes a delegate inside of process
     */ 
    virtual void _process();
};

}
