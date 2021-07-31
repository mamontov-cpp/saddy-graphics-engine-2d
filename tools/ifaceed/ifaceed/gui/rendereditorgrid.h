/*! \file rendereditorgrid.h
    
    Describes a simple process, that render editor's grid, described in settings
 */
#pragma once
#include <pipeline/pipelineprocess.h>
#include <sadcolor.h>
#include <scene.h>

namespace gui
{

/*! A simple process, that forces rendering all grids in database
 */ 
class RenderEditorGrid: public sad::pipeline::AbstractProcess
{
public:
    /*! Construct default editor's grid, as disabled grid
     */ 
    RenderEditorGrid();
    /*! Could be inherited
     */
    virtual ~RenderEditorGrid();
    /*! Whether editor is enabled
        \return whether editor is enabled
     */ 
    bool isEnabled() const;
    /*! A color for grid
     */
    sad::AColor Color;
    /*! A space between two parts of grid on horizontal axis
     */
    size_t GridSpaceX;
    /*! A space between two parts of grid on vertical axis
     */
    size_t GridSpaceY;    
protected:
    /*! Renders editor grid if enabled
     */ 
    virtual void _process() override;
    /*! An inner scene for grid
     */
    sad::Scene m_scene;
};

}
