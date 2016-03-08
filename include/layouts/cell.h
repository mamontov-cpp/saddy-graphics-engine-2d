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
    /*! A normalized rectangle, which is used in some computation
     */ 
    typedef sad::Pair<sad::Point2D, sad::Point2D> NormalizedRectangle;
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
    /*! Sets width for a cell
        \param[in] width a new width for a cell
        \param[in] update_grid whether we should notify grid
     */
    void setWidth(const sad::layouts::LengthValue& width, bool upgrade_grid = true);
    /*! Returns width of a cell
        \return width of a cell
     */
    const sad::layouts::LengthValue& width() const;
    /*! Sets height for a cell
        \param[in] height a new width for a cell
        \param[in] update_grid whether we should notify grid
     */
    void setHeight(const sad::layouts::LengthValue height, bool upgrade_grid = true);
    /*! Returns height of a cell
        \return height of a cell
     */
    const sad::layouts::LengthValue& height() const;
    /*! Sets count of spanning rows for a cell
        \param[in] rows a count of spanning rows for cell
        \param[in] update_grid whether we should notify grid
     */
    void setRowSpan(unsigned int rows, bool upgrade_grid = true);
    /*! Returns count of spanning rows for a cell
        \return count of spanning rows for a cell
     */
    unsigned int rowSpan() const;
    /*! Sets count of spanning columns for a cell
        \param[in] rows a count of spanning columns for cell
        \param[in] update_grid whether we should notify grid
     */
    void setColSpan(unsigned int rows, bool upgrade_grid = true);
    /*! Returns count of spanning columns for a cell
        \return count of spanning columns for a cell
     */
    unsigned int colSpan() const;
    /*! Sets vertical alignment for a cell
        \param[in] align a vertical alignment
        \param[in] update_grid whether we should notify grid
     */
    void setVerticalAlignment(sad::layouts::VerticalAlignment align, bool upgrade_grid = true);
    /*! Returns vertical alignment for a cell
        \return vertical alignment
     */
    sad::layouts::VerticalAlignment verticalAlignment() const;
    /*! Sets horizontal alignment for a cell
        \param[in] align a horizontal alignment
        \param[in] update_grid whether we should notify grid
     */
    void setHorizontalAlignment(sad::layouts::HorizontalAlignment align, bool upgrade_grid = true);
    /*! Returns horizontal alignment for a cell
        \return horizontal alignment
     */
    sad::layouts::HorizontalAlignment horizontalAlignment() const;
    /*! Sets type of stacking for a cell
        \param[in] type a type of 
     */
    void setStackingType(sad::layouts::StackingType type, bool upgrade_grid = true);
    /*! Returns stacking type for a cells
        \return stacking type
     */
    sad::layouts::StackingType stackingType() const;
    /*! Sets top padding for a cell
        \param[in] value a new value for padding
        \param[in] update_grid whether we should update grid after it.
     */
    void setPaddingTop(double value, bool update_grid = true);
    /*! Returns top padding for a cell
        \return top padding
     */
    double paddingTop() const;
    /*! Sets bottom padding for a cell
        \param[in] value a new value for padding
        \param[in] update_grid whether we should update grid after it.
     */
    void setPaddingBottom(double value, bool update_grid = true);
    /*! Returns bottom padding for a cell
        \return bottom padding
     */
    double paddingBottom() const;
    /*! Sets left padding for a cell
        \param[in] value a new value for padding
        \param[in] update_grid whether we should update grid after it.
     */
    void setPaddingLeft(double value, bool update_grid = true);
    /*! Returns left padding for a cell
        \return left padding
     */
    double paddingLeft() const;
    /*! Sets right padding for a cell
        \param[in] value a new value for padding
        \param[in] update_grid whether we should update grid after it.
     */
    void setPaddingRight(double value, bool update_grid = true);
    /*! Returns right padding for a cell
        \return right padding
     */
    double paddingRight() const;
    /*! Set nodes for a cell
        \param[in] nodes a nodes
        \param[in] update_grid whether we should update grid after it.
     */
    void setChildren(const sad::Vector<sad::SceneNode*>& nodes, bool update_grid = true);
    /*! Returns list of nodes for a cells. Note, that could return NULL
        if nodes are not found
        \return list of children as nodes
     */
    sad::Vector<sad::SceneNode*> children() const;
    /*! Set nodes links as major ids for a cell
        \param[in] nodes a nodes
        \param[in] update_grid whether we should update grid after it.
     */
    void setChildren(const sad::Vector<unsigned long long>& nodes, bool update_grid = true);
    /*! Returns a a major ids list for a children of cell
        \return list of major ids
     */
    sad::Vector<unsigned long long> childrenMajorIds() const;
    /*! Adds child to cell
        \param[in] node a node
        \param[in] update_grid whether we should update grid after it.
     */
    void addChild(sad::SceneNode* node, bool update_grid = true);
    /*! Adds child to cell
        \param[in] major_id a major id for added child
        \param[in] update_grid whether we should update grid after it.
     */
    void addChild(unsigned long long major_id, bool update_grid = true);
    /*! Inserts child to cell
        \param[in] pos a position for inserting. If it's out of container, child will be added to end
        \param[in] node a node
        \param[in] update_grid whether we should update grid after it.
     */
    void insertChild(unsigned int pos, sad::SceneNode* node, bool update_grid = true);
    /*! Inserts child to cell
        \param[in] pos a position for inserting. If it's out of container, child will be added to end
        \param[in] major_id a major id for a cell
        \param[in] update_grid whether we should update grid after it.
     */
    void insertChild(unsigned int pos, unsigned long long major_id, bool update_grid = true);
    /*! Removes a child at specified position
        \param[in] pos a position
     */
    void removeChild(unsigned int pos, bool update_grid = true);
    /*! Swaps to children of cell by their positions
        \param[in] pos1 first position
        \param[in] pos2 second position
        \param[in] update_grid whether we should update grid after it.
     */
    void swapChildren(unsigned int pos1, unsigned int pos2, bool update_grid = true);
    /*! Returns count of children in cell
        \return count of children in cell
     */
    size_t childrenCount() const;
    /*! Returns a child of cell by position
        \param[in] pos a position
        \return a node
     */
    sad::SceneNode* child(unsigned int pos) const;
    /*! Returns  a child's major id by it's position
        \param[in] pos a position
        \return a node
     */
    unsigned long long childMajorId(unsigned int pos) const;
    /*! Finds a scene node by it's name
        \param[in] node a node
        \return result, which is set if found
     */
    sad::Maybe<size_t> find(sad::SceneNode* node) const;
    /*! Finds a scene node by it's name
        \param[in] major_id a major id for an object
        \return result, which is set if found
     */
    sad::Maybe<size_t> find(unsigned long long major_id) const;
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
    /*! Returns a preferred size for a cell
        \return a preferred size
     */
    sad::Size2D preferredSize() const;
    /*! An assigned area for cell
     */
    sad::Rect2D AssignedArea;
    /*! A mark flag for each cell to ensure it was rendered only once. Used by sad::layouts::Grid
     */
    bool Rendered;
protected:
    /*! Computes normalized sizes for children 
     */
    void computeNormalizedChildrenSizes();
    /*! Normalizes rectangle to a pair
        \param[in] r rectangle
        \return normalized rectangle as first element has minimal x and y coords, and second one - maximal
     */
    static sad::layouts::Cell::NormalizedRectangle normalize(const sad::Rect2D& r);
    /*! Normalizes a rectangle pair, changing coordinates, according rules from previous method
        \param[out] result a result
        \param[in] p point
     */
    static void normalize(sad::layouts::Cell::NormalizedRectangle& result, const sad::Point2D& p);
    /*! Tries notify a grid, if update_grid is specified
        \param[in] update_grid whether we should notify a grid
     */
    void tryNotify(bool update_grid);
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
    /*! A rectangles for sized for normalized childrens
     */
    sad::Vector<sad::layouts::Cell::NormalizedRectangle> m_normalized_children;

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