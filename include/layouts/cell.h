/*! \file cell.h
    
    Describes a grid cell
 */
#pragma once
#include "serializablecell.h"

#include "../db/dbtypedlink.h"
#include "../sadptrvector.h"
#include "../sadrect.h"
#include "../scenenode.h"

namespace sad
{

class Renderer;

namespace db
{

class Database;

}

namespace layouts
{

class Grid;
    
class Cell
{
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
    void fromSerializable(const sad::layouts::SerializableCell& cell, sad::db::Database* db);
    /*! Updates inner children, according to assigned area
     */
    void update();
    /*! Sets top padding for a cell
        \param[in] value a new value for padding
        \param[in] upgrade_grid whether we should update grid after it.
     */
    void setPaddingTop(double value, bool update_grid);
    /*! Sets bottom padding for a cell
        \param[in] value a new value for padding
        \param[in] upgrade_grid whether we should update grid after it.
     */
    void setPaddingBottom(double value, bool update_grid);
    /*! Sets left padding for a cell
        \param[in] value a new value for padding
        \param[in] upgrade_grid whether we should update grid after it.
     */
    void setPaddingLeft(double value, bool update_grid);
    /*! Sets right padding for a cell
        \param[in] value a new value for padding
        \param[in] upgrade_grid whether we should update grid after it.
     */
    void setPaddingRight(double value, bool update_grid);
    /*! Returns a database, which is cell linked to
        \return database
     */
    sad::db::Database* database() const;
    /*! Sets a databasw, which is cell linked to
        \param[in] db database
     */
    void setDatabase(sad::db::Database* db);
    /*! Moves a grid by specified vector
        \param[in] point
     */
    void moveBy(const sad::Point2D& p);
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
    /*! A database, which is cell is linked to 
     */
    sad::db::Database* m_db;
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