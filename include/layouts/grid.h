/*! \file grid.h
    
    Describes a table, which contains cells with child nodes, arranging them in table.
    Note, that cells can contains grids inside them, allowing nesting inside of grid
 */
#pragma once
#include "../scenenode.h"
#include "../sadptrvector.h"
#include "../sadcolor.h"
#include "../sadhash.h"
#include "cell.h"

namespace sad
{
    
class Renderer;

namespace layouts
{
/*! Describes a table, which contains cells with child nodes, arranging them in table.
    Note, that cells can contains grids inside them, allowing nesting inside of grid.
    
    Note, that grid is expanded automatically, as new cells are added, so area property
    may change, due to large inner nodes.
    
    Also, you could see fixed width or fixed height and table will try to adjust to it.
    It may fail if one of nodes is too large to be fit.
 */ 
class Grid: public sad::SceneNode
{
SAD_OBJECT  
public:
    /*! A result of search for an object in grid.
        The first element is position of cell in grid, which could be returned by cell() method.
        The second one is a position of children in search
     */
    typedef sad::Pair<size_t, size_t> SearchResult;
    /*! Constructs a default grid with no cells
     */
    Grid();
    /*! Grid could be inherited
     */
    virtual ~Grid() override;
    /*! Returns cell by it's row and it's cell
        \param[in] row a row for cell
        \param[in] col a column for cell
     */
    sad::layouts::Cell* cell(unsigned int row, unsigned int col);
    /*! Sets a tree name for object with specified renderer
        \param[in] r renderer, which tree should be fetched from
        \param[in] tree_name a name for an item for object
     */
    virtual void setTreeName(sad::Renderer* r, const sad::String & tree_name) override;
    /*! Returns renderer for a grid
        \return renderer
     */
    // ReSharper disable once CppHidingFunction
    sad::Renderer* renderer() const override;
    /*! Allows rendering of a grid
     */ 
    virtual void render() override;
    /*! Allows rendering of a grid with specified color
        \param[in] clr color
     */ 
    virtual void renderWithColor(const sad::AColor& clr);
    /*! Returns list of regions
        \return list of regions
     */
	// ReSharper disable once CppInconsistentNaming
	// ReSharper disable once CppHidingFunction
    sad::Vector<sad::Rect2D> getRegions();
    /*! Fills vector of regions with exactly one region, describing an area of grid
        \param[out] r a vector of regions
     */
    virtual void regions(sad::Vector<sad::Rect2D> & r) override;
    /*! Loads grid from picojson object
        \param[in] v a picojson object
        \return  whether it as successfull
     */
    virtual bool load(const picojson::value& v) override;
    /*! A basic schema for object
        \return a schema 
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const  override;
    /*! Sets an area for table
        \param[in] r area for table
     */
    void setArea(const sad::Rect2D & r);
    /*! Returns area for label
        \return area for label
     */
    sad::Rect2D area() const;
    /*! Sets rows for grid
        \param[in] rows for grid
     */
    void setRows(unsigned int rows);
    /*! Returns rows for grid
        \return rows for grid
     */ 
    unsigned int rows() const;
    /*! Sets cols for grid
        \param[in] cols for grid
     */
    void setColumns(unsigned int cols);
    /*! Returns cols for grid
        \return cols for grid
     */ 
    unsigned int columns() const;
    /*! Sets top padding for cells. May set all cells padding's to a current value 
        if propagate is true, otherwise sets this settings for newly created cells
        \param[in] value a value for top padding
        \param[in] propagate whether we should upgrade all paddings
     */
    void setPaddingTop(double value, bool propagate);
    /*! Sets default value for top padding of grid cells. Equivalent of call of setPaddingTop(value, false).
        \param[in] value a value
     */
    void setDefaultPaddingTop(double value);
    /*! Returns default value for top padding of grid cells
        \return value
     */
    double paddingTop() const;
    /*! Sets bottom padding for cells. May set all cells padding's to a current value 
        if propagate is true, otherwise sets this settings for newly created cells
        \param[in] value a value for top padding
        \param[in] propagate whether we should upgrade all paddings
     */
    void setPaddingBottom(double value, bool propagate);
    /*! Sets default value for bottom padding of grid cells. Equivalent of call of setPaddingTop(value, false).
        \param[in] value a value
     */
    void setDefaultPaddingBottom(double value);
    /*! Returns default value for bottom padding of grid cells
        \return value
     */
    double paddingBottom() const;
    /*! Sets left padding for cells. May set all cells padding's to a current value 
        if propagate is true, otherwise sets this settings for newly created cells
        \param[in] value a value for top padding
        \param[in] propagate whether we should upgrade all paddings
     */
    void setPaddingLeft(double value, bool propagate);
    /*! Sets default value for left padding of grid cells. Equivalent of call of setPaddingTop(value, false).
        \param[in] value a value
     */
    void setDefaultPaddingLeft(double value);
    /*! Returns default value for left padding of grid cells
        \return value
     */
    double paddingLeft() const;
    /*! Sets right padding for cells. May set all cells padding's to a current value 
        if propagate is true, otherwise sets this settings for newly created cells
        \param[in] value a value for top padding
        \param[in] propagate whether we should upgrade all paddings
     */
    void setPaddingRight(double value, bool propagate);
    /*! Sets default value for right padding of grid cells. Equivalent of call of setPaddingTop(value, false).
        \param[in] value a value
     */
    void setDefaultPaddingRight(double value);
    /*! Returns default value for right padding of grid cells
        \return value
     */
    double paddingRight() const;
    /*! Sets fixed width for a grid
        \param[in] flag whether it has fixed width
     */
    void setFixedWidth(bool flag);
    /*! Returns true if grid has fixed width
        \return true if grid has fixed width
     */
    bool fixedWidth() const;
    /*! Sets fixed height for a table
        \param[in] flag whether it has fixed height
     */
    void setFixedHeight(bool flag);
    /*! Returns true if grid has fixed height
        \return true if grid has fixed height
     */
    bool fixedHeight() const;
    /*! Sets rendering color for a grid
        \param[in] clr color of a grid
     */
    void setRenderColor(const sad::AColor& clr);
    /*! Returns rendering color for a grid
        \return rendering color
     */
    const sad::AColor& renderColor() const;
    /*! Returns a list of a serializable cells
        \return list of cells
     */ 
    sad::Vector<sad::layouts::SerializableCell> cells() const;
    /*! This is very unsafe functions, since this will replace all cells by new cells without actually checking their size
        \param[in] cells list of cells
     */
    void setCells(const sad::Vector<sad::layouts::SerializableCell>& cells);
    /*! Moves a grid by specified vector
        \param[in] p point, which specifies a vector
     */
    virtual void moveBy(const sad::Point2D& p) override;
    /*! Returns a cell by inner position
        \param[in] pos a position
        \return cell
     */
    sad::layouts::Cell* cell(size_t pos) const;
     /*! Finds a scene node by it's address
        \param[in] node a node
        \return result, which is set if found
     */
    sad::Maybe<sad::layouts::Grid::SearchResult> find(sad::SceneNode* node) const;
     /*! Finds a scene node by it's major id
        \param[in] major_id a major id for a node
        \return result, which is set if found
     */
    sad::Maybe<sad::layouts::Grid::SearchResult> find(unsigned long long major_id) const;
    /*! Merges several cells into one. Returns true if successfull. Could return false in
        cases if row_span and col_span cause system to go out of bounds of grid, or some 
        cells go into the set only partially. In that case, nothing is performed.
        \param[in] row a starting row
        \param[in] col a starting column
        \param[in] row_span amount of row spanning
        \param[in] col_span amount of columns spanning
     */
    bool merge(size_t row, size_t col, size_t row_span, size_t col_span);
    /*! Splits one big cell into small ones. Note that children will remain in first cell.
        Range must define only one big cell, otherwise false is returned and nothing is performed.
        \param[in] row a starting row
        \param[in] col a starting column
        \param[in] row_span amount of row spanning
        \param[in] col_span amount of columns spanning      
     */
    bool split(size_t row, size_t col, size_t row_span, size_t col_span);
    /*! Forces grid to recompute all items for cells
     */ 
    void update();
    /*! Returns a a major ids list for all children of all cells
        \return list of major ids
     */
    sad::Vector<unsigned long long> childrenMajorIds() const;
    /*! Returns a list of nodes for all children of all cells
        \return list of child nodes
     */
    sad::Vector<sad::SceneNode*> children() const;
    /*! Returns actual amount of allocated cells in grid (not the product of rows and columns)
        \return actual amount of allocated cells
     */
    size_t allocatedCellCount() const;
    /*! Sets a table, propagating database to a cells
        \param[in] t a table
     */
    virtual void setTable(sad::db::Table* t) override;
private:
    /*! The grid is not copyable
        \param[in] o a grid
     */
    Grid(const sad::layouts::Grid& o);
    /*! The grid is not copyable
        \param[in] o a grid
        \return self-reference
     */
    sad::layouts::Grid& operator=(const sad::layouts::Grid& o);
    /*! Expands rows to a specified size
        \param[in] old_rows an old rows count
        \param[in] new_rows a new rows count
     */
    void expandRows(size_t old_rows, size_t new_rows);
    /*! Shrinks rows to a specified size
        \param[in] old_rows an old rows count
        \param[in] new_rows a new rows count
     */
    void shrinkRows(size_t old_rows, size_t new_rows);
    /*! Expands columns to a specified size
        \param[in] old_cols an old columns count
        \param[in] new_cols a new rows count
     */
    void expandColumns(size_t old_cols, size_t new_cols);
    /*! Shrinks columns to a specified size
        \param[in] old_cols an old columns count
        \param[in] new_cols a new rows count
     */
    void shrinkColumns(size_t old_cols, size_t new_cols);
    /*! Regenerates cell views array, according to current settings
        \param[in] p_rows a pointer, which should point to current rows count (nullptr for current rows)
        \param[in] p_cols a pointer, which should point to current columns count (nullptr for current columns)
     */
    void makeCellViews(size_t* p_rows = nullptr, size_t* p_cols = nullptr);
    /*! Validates list of cells, returning whether it's valid
        \return whether list of cells is valid
     */
    bool validate() const;
    /*! Builds coverage for a list of cells
        \param[out] coverage a coverage for cells
     */
    void buildCoverage(sad::Hash<size_t, sad::Hash<size_t, sad::Vector<size_t> > >& coverage) const;  
    /*! Makes new cell with specified row and column
        \param[in] row a row 
        \param[in] col a column
        \param[in] row_span amount of spanning rows
        \param[in] col_span amount of spanning columns
     */
    sad::layouts::Cell* makeCell(size_t row, size_t col, size_t row_span, size_t col_span);
    /*! A Computes a list of cells, that are affected by changes of region
        \param[in] row a row of affected region
        \param[in] col a column of affected region
        \param[in] row_span amount of spanning rows for affected region
        \param[in] col_span amount of spanning columns for affected region
        \param[out] affected_cells a list of affected cells
     */
    void cellsAffectedByRegion(
        size_t row, 
        size_t col, 
        size_t row_span, 
        size_t col_span,
        sad::Hash<size_t, sad::layouts::Cell*>& affected_cells
    );
    /*! Recalculates spans of affected cells
        \param[in] row a row of affected region
        \param[in] col a column of affected region
        \param[in] row_span amount of spanning rows for affected region
        \param[in] col_span amount of spanning columns for affected region
        \param[in] merge true if we merging cells, false otherwise
        \param[in] affected_cells a list of affected cells
        \param[out] to_be_erased a list of cells to be erased
     */
    void recalculateSpansOfAffectedCells(
        size_t row, 
        size_t col, 
        size_t row_span, 
        size_t col_span,
        bool merge,
        const sad::Hash<size_t, sad::layouts::Cell*>& affected_cells,
        sad::Vector<sad::layouts::Cell*>& to_be_erased
    );
    /*! An area for a grid
     */
    sad::Rect2D m_area;
    /*! An amount of rows
     */
    unsigned int m_rows;
    /*! An amount of columns
     */
    unsigned int m_cols;
    /*! A list of cells
     */
    sad::PtrVector<sad::layouts::Cell> m_cells;
    /*! A viewing vector for cells
     */
    sad::Vector<sad::layouts::Cell*> m_cell_views;
    /*! A default top padding for all cells
     */
    double m_padding_top;
    /*! A default bottom padding for all cells
     */
    double m_padding_bottom;
    /*! A default left padding for all cells
     */
    double m_padding_left;
    /*! A default right padding for all cells
     */
    double m_padding_right; 
    /*! Whether it's fixed width table.
     */
    bool m_fixed_width;
    /*! Whether it's fixed height table
     */
    bool m_fixed_height;
    /*! A rendering color for a grid
     */
    sad::AColor m_render_color;
    /*! A parent renderer for a grid
     */
    sad::Renderer * m_renderer;
    /*! Toggles loading mode on grid. If grid is loading, no update should be called
     */
    bool m_loading; 
};

}   
    
}


DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::layouts::Grid);