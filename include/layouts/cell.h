/*! \file cell.h
    
    Describes a grid cell
 */
#pragma once
#include "serializablecell.h"

#include "../object.h"
#include "../db/dbtypedlink.h"
#include "../sadptrvector.h"
#include "../sadrect.h"
#include "../scenenode.h"

namespace sad
{

class Renderer;
    
namespace layouts
{

class Grid;
    
class Cell: public sad::Object
{
SAD_OBJECT
public:
    /*! Inits a default cell
     */
    Cell();
    /*! Could be inherited
     */
    virtual ~Cell();
    /*! Converts cell to a serializable data
        \param[in] cell a cell
     */
    void toSerializable(sad::layouts::SerializableCell& cell) const;
    /*! Restores cell from a serializable representation
        \param[in] cell a cell
        \param[in] db a source database, where objects are located
     */
    void fromSerializable(sad::layouts::SerializableCell& cell, sad::db::Database* db);
    /*! An assigned area for cell
     */
    sad::Rect2D AssignedArea;
	/*! A mark flag for each cell to ensure it was rendered only once. Used by sad::layouts::Grid
	 */
	bool Rendered;
protected:
    /*! Width of a cell
     */
    sad::layouts::LengthValue m_width;
    /*! Height of a cell
     */
    sad::layouts::LengthValue m_height;
    /*! Amount of spanning rows for cell
     */
    unsigned int m_row_span;
    /*! Amount of spanning columns for cell
     */
    unsigned int m_col_span;
    /*! Vertical alignment for cell
     */
    sad::layouts::VerticalAlignment m_valign;
    /*! Horizontal alignment for cell
     */
    sad::layouts::HorizontalAlignment m_halign;
    /*! An array of children, which should be arranged in cell
     */
    sad::PtrVector<sad::db::TypedLink<sad::SceneNode> > m_children;
    /*! A stacking type for a cell
     */
    sad::layouts::StackingType m_stacking_type;
    /*! A top padding for cell
     */
    double m_padding_top;
    /*! A bottom padding for cell
     */
    double m_padding_bottom;
    /*! A left padding for cell
     */
    double m_padding_left;
    /*! A right padding for cell 
     */
    double m_padding_right;

    /*! A grid, which cell is being linked to
     */
    sad::layouts::Grid* m_grid;
private:
    /*! This object is non-copyable
        \param[in] o object
     */
    Cell(const sad::layouts::Cell& o);
    /*! This object is non-copyable
        \param[in] o object
        \return self-reference
     */
    sad::layouts::Cell& operator=(const sad::layouts::Cell& o);
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::layouts::Cell)