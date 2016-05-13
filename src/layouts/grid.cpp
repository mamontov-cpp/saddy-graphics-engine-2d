#include "layouts/grid.h"

#include "db/dbtable.h"
#include "db/dbmethodpair.h"
#include "db/schema/schema.h"

#include "util/free.h"

#include "renderer.h"
#include "primitiverenderer.h"
#include "sadmutex.h"
#include "fuzzyequal.h"

#include <stdexcept>
#include <numeric>

// =================================== PUBLIC METHODS ===================================

DECLARE_SOBJ_INHERITANCE(sad::layouts::Grid,sad::SceneNode)


sad::layouts::Grid::Grid()
: m_rows(0),
m_cols(0),
m_padding_top(0),
m_padding_bottom(0),
m_padding_left(0),
m_padding_right(0),
m_fixed_width(false),
m_fixed_height(false),
m_render_color(255,0 ,0),
m_renderer(NULL),
m_loading(false)
{
    
}

sad::layouts::Grid::~Grid()
{
    
}

sad::layouts::Cell* sad::layouts::Grid::cell(unsigned int row, unsigned int col)
{
    sad::layouts::Cell* result = NULL;
    size_t pos = row * m_cols + col;
    if (pos < m_cell_views.size())
    {
        result = m_cell_views[pos];
    }
    return result;
}

void sad::layouts::Grid::setTreeName(sad::Renderer* r, const sad::String & tree_name)
{
    m_renderer = r;
}

sad::Renderer* sad::layouts::Grid::renderer() const
{
    sad::Renderer* result = this->sad::SceneNode::renderer();
    if (!result)
    {
        result = m_renderer;        
    }
    return result;
}

void sad::layouts::Grid::render()
{
    this->renderWithColor(m_render_color);
}

void sad::layouts::Grid::renderWithColor(const sad::AColor& clr)
{
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        m_cells[i]->Rendered = false;
    }
    sad::Renderer* r = this->renderer();
    if (r)
    {
        for(size_t i = 0; i < m_rows; i++)
        {
            for(size_t j = 0; j < m_cols; j++)
            {
                sad::layouts::Cell* cl = this->cell(i, j);
                if (cl)
                {
                    if (cl->Rendered == false)
                    {
                        cl->Rendered = true;
                        // No reasons to have inner grids being rendered, they must either be added to scene or be rendered by holder node
                        r->render()->rectangle(cl->AssignedArea, clr);
                    }
                }
            }
        }
    }
}


void sad::layouts::Grid::regions(sad::Vector<sad::Rect2D> & r)
{
    r << m_area;
}
 
bool sad::layouts::Grid::load(const picojson::value& v)
{
    m_loading = true;
    bool result = this->sad::SceneNode::load(v);
    m_loading = false;
    if (result)
    {        
        result = this->validate();
        if (result)
        {
            this->makeCellViews();
        }
    }
    return result;
}

static sad::db::schema::Schema* LayoutsGridSchema = NULL;
static sad::Mutex LayoutsGridSchemaInit;

sad::db::schema::Schema* sad::layouts::Grid::basicSchema()
{
    if (LayoutsGridSchema == NULL)
    {
        LayoutsGridSchemaInit.lock();
        if (LayoutsGridSchema == NULL)
        {
            LayoutsGridSchema = new sad::db::schema::Schema();
            LayoutsGridSchema->addParent(sad::SceneNode::basicSchema());
            LayoutsGridSchema->add(
                "area", 
                new sad::db::MethodPair<sad::layouts::Grid, sad::Rect2D>(
                    &sad::layouts::Grid::area,
                    &sad::layouts::Grid::setArea
                )
            );  
            LayoutsGridSchema->add(
                "rows", 
                new sad::db::MethodPair<sad::layouts::Grid, unsigned int>(
                    &sad::layouts::Grid::rows,
                    &sad::layouts::Grid::setRows
                )
            );  
            LayoutsGridSchema->add(
                "columns", 
                new sad::db::MethodPair<sad::layouts::Grid, unsigned int>(
                    &sad::layouts::Grid::columns,
                    &sad::layouts::Grid::setColumns
                )
            );  
            LayoutsGridSchema->add(
                "padding_top", 
                new sad::db::MethodPair<sad::layouts::Grid, double>(
                    &sad::layouts::Grid::paddingTop,
                    &sad::layouts::Grid::setDefaultPaddingTop
                )
            );  
            LayoutsGridSchema->add(
                "padding_bottom", 
                new sad::db::MethodPair<sad::layouts::Grid, double>(
                    &sad::layouts::Grid::paddingBottom,
                    &sad::layouts::Grid::setDefaultPaddingBottom
                )
            );
            LayoutsGridSchema->add(
                "padding_left", 
                new sad::db::MethodPair<sad::layouts::Grid, double>(
                    &sad::layouts::Grid::paddingLeft,
                    &sad::layouts::Grid::setDefaultPaddingLeft
                )
            );
            LayoutsGridSchema->add(
                "padding_right", 
                new sad::db::MethodPair<sad::layouts::Grid, double>(
                    &sad::layouts::Grid::paddingRight,
                    &sad::layouts::Grid::setDefaultPaddingRight
                )
            );
            LayoutsGridSchema->add(
                "fixed_width", 
                new sad::db::MethodPair<sad::layouts::Grid, bool>(
                    &sad::layouts::Grid::fixedWidth,
                    &sad::layouts::Grid::setFixedWidth
                )
            );
            LayoutsGridSchema->add(
                "fixed_height", 
                new sad::db::MethodPair<sad::layouts::Grid, bool>(
                    &sad::layouts::Grid::fixedHeight,
                    &sad::layouts::Grid::setFixedHeight
                )
            );
            LayoutsGridSchema->add(
                "render_color", 
                new sad::db::MethodPair<sad::layouts::Grid, sad::AColor>(
                    &sad::layouts::Grid::renderColor,
                    &sad::layouts::Grid::setRenderColor
                )
            );
            LayoutsGridSchema->add(
                "cells", 
                new sad::db::MethodPair<sad::layouts::Grid, sad::Vector<sad::layouts::SerializableCell> >(
                    &sad::layouts::Grid::cells,
                    &sad::layouts::Grid::setCells
                )
            );
            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(LayoutsGridSchema);
        }
        LayoutsGridSchemaInit.unlock();
    }
    return LayoutsGridSchema;
}

sad::db::schema::Schema* sad::layouts::Grid::schema() const
{
    return sad::layouts::Grid::basicSchema();
}


void sad::layouts::Grid::setArea(const sad::Rect2D & r)
{
    m_area = r;
    if (!m_loading)
    {
        this->update();
    }
}

sad::Rect2D sad::layouts::Grid::area() const
{
    return m_area;
}

void sad::layouts::Grid::setRows(unsigned int rows)
{
    m_rows = rows;
    unsigned int oldrows = m_rows;
    if (!m_loading)
    {
        if (oldrows > m_rows)
        {
            this->shrinkRows(oldrows, m_rows);
            this->update();
        }
        if (oldrows < m_rows)
        {
            this->expandRows(oldrows, m_rows);
            this->update();
        }
    }
}

unsigned int sad::layouts::Grid::rows() const
{
    return m_rows;
}

void sad::layouts::Grid::setColumns(unsigned int cols)
{
    unsigned int oldcols = m_cols;
    m_cols = cols;
    if (!m_loading)
    {
        if (oldcols > m_cols)
        {
            this->shrinkColumns(oldcols, m_cols);
            this->update();
        }
        if (oldcols < m_cols)
        {
            this->expandColumns(oldcols, m_cols);
            this->update();
        }
    }
}

unsigned int sad::layouts::Grid::columns() const
{
    return m_cols;
}

void sad::layouts::Grid::setPaddingTop(double value, bool propagate)
{
    m_padding_top = value;
    if (propagate)
    {
        for(size_t i = 0; i < m_cells.size(); i++)
        {
            m_cells[i]->setPaddingTop(value, false);
        }
        this->update();
    }
}

void sad::layouts::Grid::setDefaultPaddingTop(double value)
{
    setPaddingTop(value, false);
}

double sad::layouts::Grid::paddingTop() const
{
    return m_padding_top;
}

void sad::layouts::Grid::setPaddingBottom(double value, bool propagate)
{
    m_padding_bottom = value;
    if (propagate)
    {
        for(size_t i = 0; i < m_cells.size(); i++)
        {
            m_cells[i]->setPaddingBottom(value, false);
        }
        this->update();
    }
}

void sad::layouts::Grid::setDefaultPaddingBottom(double value)
{
    setPaddingBottom(value, false);
}

double sad::layouts::Grid::paddingBottom() const
{
    return m_padding_bottom;
}

void sad::layouts::Grid::setPaddingLeft(double value, bool propagate)
{
    m_padding_left = value;
    if (propagate)
    {
        for(size_t i = 0; i < m_cells.size(); i++)
        {
            m_cells[i]->setPaddingLeft(value, false);
        }
        this->update();
    }
}

void sad::layouts::Grid::setDefaultPaddingLeft(double value)
{
    setPaddingLeft(value, false);
}

double sad::layouts::Grid::paddingLeft() const
{
    return m_padding_left;
}

void sad::layouts::Grid::setPaddingRight(double value, bool propagate)
{
    m_padding_right = value;
    if (propagate)
    {
        for(size_t i = 0; i < m_cells.size(); i++)
        {
            m_cells[i]->setPaddingRight(value, false);
        }
        this->update();
    }
}

void sad::layouts::Grid::setDefaultPaddingRight(double value)
{
    setPaddingRight(value, false);
}

double sad::layouts::Grid::paddingRight() const
{
    return m_padding_right;
}

void sad::layouts::Grid::setFixedWidth(bool flag)
{
    m_fixed_width = flag;
    if (!m_loading)
    {
        this->update();
    }
}

bool sad::layouts::Grid::fixedWidth() const
{
    return m_fixed_width;
}

void sad::layouts::Grid::setFixedHeight(bool flag)
{
    m_fixed_height = flag;
    if (!m_loading)
    {
        this->update();
    }
}

bool sad::layouts::Grid::fixedHeight() const
{
    return m_fixed_height;
}

void sad::layouts::Grid::setRenderColor(const sad::AColor& clr)
{
    m_render_color = clr;
}

const sad::AColor& sad::layouts::Grid::renderColor() const
{
    return m_render_color;
}

sad::Vector<sad::layouts::SerializableCell> sad::layouts::Grid::cells() const
{
    sad::Vector<sad::layouts::SerializableCell> result;
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        sad::layouts::SerializableCell cell;
        m_cells[i]->toSerializable(cell);
        result << cell;
    }
    return result;
}

void sad::layouts::Grid::setCells(const sad::Vector<sad::layouts::SerializableCell>& cells)
{
    sad::Vector<sad::layouts::Cell*> oldcells = static_cast<sad::Vector<sad::layouts::Cell*>&>(m_cells);    
    m_cells.clear();
    sad::db::Database* db = NULL;
    sad::db::Table* tbl = this->table();
    if (tbl) {
        db = tbl->database();
    }
    for(size_t i = 0; i < cells.size(); i++)
    {
        sad::layouts::Cell* cell = new sad::layouts::Cell();
        cell->setDatabase(db);
        cell->fromSerializable(cells[i], db);
        cell->setGrid(this);
        m_cells << cell;
    }
    if (!m_loading)
    {
        if (this->validate())
        {
            // Remove old cells
            sad::util::free(oldcells);
            this->makeCellViews();
            this->update();
        }
        else
        {
            sad::util::free(m_cells);
            static_cast<sad::Vector<sad::layouts::Cell*>&>(m_cells) = oldcells;
        }
    }
    else
    {
        sad::util::free(oldcells);
    }
}

sad::layouts::Cell* sad::layouts::Grid::cell(size_t pos) const
{
    sad::layouts::Cell* result = NULL;
    if (pos < m_cells.size())
    {
        result = m_cells[pos];
    }
    return result;
}

sad::Maybe<sad::layouts::Grid::SearchResult> sad::layouts::Grid::find(sad::SceneNode* node) const
{
    sad::Maybe<sad::layouts::Grid::SearchResult> result;
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        sad::Maybe<size_t> maybe_pos = m_cells[i]->find(node);
        if (maybe_pos.exists())
        {
            result.setValue(sad::layouts::Grid::SearchResult(i, maybe_pos.value()));
        }
    }
    return result;
}

sad::Maybe<sad::layouts::Grid::SearchResult> sad::layouts::Grid::find(unsigned long long major_id) const
{
    sad::Maybe<sad::layouts::Grid::SearchResult> result;
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        sad::Maybe<size_t> maybe_pos = m_cells[i]->find(major_id);
        if (maybe_pos.exists())
        {
            result.setValue(sad::layouts::Grid::SearchResult(i, maybe_pos.value()));
        }
    }
    return result;
}

struct CellComparator {
    bool operator()(sad::layouts::Cell* a, sad::layouts::Cell* b) const
    {   
        if (a->Row == b->Row)
        {
            return a->Col < b->Col;
        }
        return a->Row < b->Row;
    }   
};

bool sad::layouts::Grid::merge(size_t row, size_t col, size_t row_span, size_t col_span)
{
    if ((row + row_span > m_rows) || (col + col_span) > m_cols)
    {
        return false;
    }
    sad::Vector<sad::layouts::Cell*> merging_cells;
    for(size_t i = 0; i < row_span; i++)
    {
        for(size_t j = 0; j < col_span; j++)
        {
            merging_cells << cell(row + i, col + j);
        }
    }
    std::unique(merging_cells.begin(), merging_cells.end()); //-V530
    bool result = true;
    for(size_t i = 0; i < merging_cells.size(); i++)
    {
        sad::layouts::Cell* cell = merging_cells[i];
        if ((cell->Row < row) 
            || (cell->Col < col)
            || ((cell->Row + cell->rowSpan()) > (row + row_span)) 
            || ((cell->Col + cell->colSpan()) > (col + col_span)) )
        {
            result = false;
        }
    }
    if (result)
    {
        CellComparator less;
        std::sort(merging_cells.begin(), merging_cells.end(), less);
        sad::layouts::Cell* target_cell = merging_cells[0];
        for(size_t i = 1; i < merging_cells.size(); i++)
        {
            for(size_t j = 0; j < merging_cells[i]->m_children.size(); j++)
            {
                target_cell->m_children << merging_cells[i]->m_children[j];
            }
            merging_cells[i]->m_children.clear();
            delete merging_cells[i];
            m_cells.removeAll(merging_cells[i]);
        }       
        target_cell->setRowSpan(row_span);
        target_cell->setColSpan(col_span);      
        makeCellViews();
        this->update();
    }
    return result;
}

bool sad::layouts::Grid::split(size_t row, size_t col, size_t row_span, size_t col_span)
{
    sad::layouts::Cell* cell = NULL;
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        if (m_cells[i]->Row == row 
            && m_cells[i]->Col == col
            && m_cells[i]->rowSpan() == row_span
            && m_cells[i]->colSpan() == col_span)
        {
            cell = m_cells[i];
        }           
    }
    bool result = false;
    sad::db::Database* db = this->table()->database();
    if (cell != NULL)
    {
        result = true;
        for(size_t i = 0; i < row_span; i++)
        {
            for(size_t j = 0; j < col_span; j++)
            {
                if ((i != 0) || (j != 0))
                {
                    sad::layouts::Cell* newcell = new sad::layouts::Cell();
                    newcell->setGrid(this);
                    newcell->setDatabase(db);
                    newcell->setPaddingBottom(m_padding_bottom, false);
                    newcell->setPaddingTop(m_padding_top, false);
                    newcell->setPaddingLeft(m_padding_left, false);
                    newcell->setPaddingRight(m_padding_right, false);
                    newcell->Row = row + i;
                    newcell->Col = col + j;
                    m_cells << newcell;
                }
            }
        }
        cell->setRowSpan(1);
        cell->setColSpan(1);
        CellComparator less;
        std::sort(m_cells.begin(), m_cells.end(), less);
        makeCellViews();
        this->update();
    }
    return result;
}

void sad::layouts::Grid::update()
{
    // 1. Create empty arrays of widths and heights, filled with zeros
    sad::Vector<sad::Vector<double> > widths;
    sad::Vector<sad::Vector<double> > heights;
    for(size_t i = 0; i < m_rows; i++)
    {
        widths << sad::Vector<double>();
        heights << sad::Vector<double>();
        for(size_t j = 0; j < m_cols; j++)
        {
            widths[i] << 0.0;
            heights[i] << 0.0;
        }
    }
    
    // 2. Fill this array with actual widths and heights, distributed from cells
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        sad::layouts::Cell* cell = m_cells[i];
        sad::Size2D computedSize = cell->computedSize();
        sad::Size2D size = cell->preferredSize();
        if (cell->width().Unit != sad::layouts::LU_Auto)
        {
            size.Width = computedSize.Width;            
        }
        if (cell->height().Unit != sad::layouts::LU_Auto)
        {
            size.Height = computedSize.Height;
        }
        
        if (sad::is_fuzzy_zero(size.Width) && this->fixedWidth())
        {
            size.Width = m_area.width() / cell->colSpan();
        }
        
        if (sad::is_fuzzy_zero(size.Height) && this->fixedHeight())
        {
            size.Height = m_area.height() / cell->rowSpan();
        }
        
        for(size_t row = 0; row < cell->rowSpan(); row++)
        {
            for(size_t col = 0; col < cell->colSpan(); col++)
            {
                widths[cell->Row + row][cell->Col + col] = size.Width / cell->colSpan();
                heights[cell->Row + row][cell->Col + col] = size.Height / cell->rowSpan();              
            }
        }
    }
    
    // 3. Linearize widths and heights by picking maximal width and height on row or column
    sad::Vector<double> rowtoheight;    
    sad::Vector<double> coltowidth;
    rowtoheight.resize(m_rows, 0.0);
    coltowidth.resize(m_cols, 0.0);
    for(size_t i = 0; i < m_rows; i++)
    {
        for(size_t j = 0; j < m_cols; j++)
        {
            rowtoheight[i] = std::max(rowtoheight[i], heights[i][j]);
            coltowidth[j] = std::max(coltowidth[j], widths[i][j]);
        }
    }
    double totalwidth = std::accumulate(coltowidth.begin(), coltowidth.end(), 0.0);
    double totalheight = std::accumulate(rowtoheight.begin(), rowtoheight.end(), 0.0);
    
    // 4. Recompute new widths and heights of cell, according to fixation parameters
    
    // 4.1 Adjust widths
    {
        // 4.1.1 Compute factors for widths or adjust area
        sad::Point2D startingpoint = m_area.p3();
        double factor = 1.0;
        if (this->fixedWidth())
        {
            factor = 1.0;
            if (sad::is_fuzzy_zero(totalwidth) == false)
            {
                factor = m_area.width() / totalwidth;
            }       
        }
        else
        {
            m_area = sad::Rect2D(startingpoint.x(), startingpoint.y() - m_area.height(), startingpoint.x() + totalwidth, startingpoint.y());
        }
        // 4.1.2 Recompute widths
        for(size_t i = 0; i < m_cols; i++)
        {
            coltowidth[i] *= factor;
        }
    }
    // 4.2 Adjust heights
    {
        // 4.2.1 Compute factors for height or adjust area
        sad::Point2D startingpoint = m_area.p3();
        double factor = 1.0;
        if (this->fixedHeight())
        {
            factor = 1.0;
            if (sad::is_fuzzy_zero(totalheight) == false)
            {
                factor = m_area.height() / totalheight;
            }       
        }
        else
        {
            m_area = sad::Rect2D(startingpoint.x(), startingpoint.y() - totalheight, startingpoint.x() + m_area.width(), startingpoint.y());
        }
        // 4.2.2 Recompute heights
        for(size_t i = 0; i < m_rows; i++)
        {
            rowtoheight[i] *= factor;
        }
    }
    
    // 5. Update assigned areas for all of cells
    sad::Point2D startingpoint = m_area.p3();
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        sad::layouts::Cell* cell = m_cells[i];
        double xstart = startingpoint.x() + std::accumulate(coltowidth.begin(), coltowidth.begin() + cell->Col, 0.0);
        double ystart = startingpoint.y() - std::accumulate(rowtoheight.begin(), rowtoheight.begin() + cell->Row, 0.0);
        double width = std::accumulate(coltowidth.begin() + cell->Col, coltowidth.begin() + cell->Col + cell->colSpan(), 0.0);
        double height = std::accumulate(rowtoheight.begin() + cell->Row, rowtoheight.begin() + cell->Row + cell->rowSpan(), 0.0);
        cell->AssignedArea = sad::Rect2D(xstart, ystart - height, xstart + width, ystart);
    }
    
    // 6. Update all cells.
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        m_cells[i]->update();
    }
}

sad::Vector<unsigned long long> sad::layouts::Grid::childrenMajorIds() const
{
    sad::Vector<unsigned long long>  result;
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        result << m_cells[i]->childrenMajorIds();
    }
    return result;
}

void sad::layouts::Grid::setTable(sad::db::Table* t)
{
    this->sad::SceneNode::setTable(t);
    sad::db::Database* db = NULL;
    if (t)
    {
        db = t->database();
    }
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        m_cells[i]->setDatabase(db);
    }   
}

void sad::layouts::Grid::moveBy(const sad::Point2D& p)
{
    for(size_t i = 0; i < 4; i++)
    {
        m_area[i] += p;
    }
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        m_cells[i]->moveBy(p);
    }
}

// =================================== PRIVATE METHODS ===================================

sad::layouts::Grid::Grid(const sad::layouts::Grid& o)
: m_rows(o.m_rows), 
m_cols(o.m_cols),
m_padding_top(o.m_padding_top),
m_padding_bottom(o.m_padding_bottom),
m_padding_left(o.m_padding_left),
m_padding_right(o.m_padding_right),
m_fixed_width(o.m_fixed_width),
m_fixed_height(o.m_fixed_height),
m_render_color(o.m_render_color),
m_renderer(o.m_renderer),
m_loading(false)
{
    throw std::runtime_error("Not implemented");
}

sad::layouts::Grid& sad::layouts::Grid::operator=(const sad::layouts::Grid& o)
{
    throw std::runtime_error("Not implemented");
    // ReSharper disable once CppUnreachableCode
    return *this;
}

void sad::layouts::Grid::expandRows(size_t oldrows, size_t newrows)
{
    sad::db::Database* db = NULL;
    sad::db::Table* table = this->table();
    if (table) 
    {
        db = table->database();
    }
    for(size_t row = oldrows; row < newrows; row++)
    {
        for(size_t col = 0; col < m_cols; col++)
        {
            sad::layouts::Cell* newcell = new sad::layouts::Cell();
            newcell->setGrid(this);
            newcell->setDatabase(db);
            newcell->setPaddingBottom(m_padding_bottom, false);
            newcell->setPaddingTop(m_padding_top, false);
            newcell->setPaddingLeft(m_padding_left, false);
            newcell->setPaddingRight(m_padding_right, false);
            newcell->Row = row;
            newcell->Col = col;
            m_cells << newcell;
        }
    }
    CellComparator less;
    std::sort(m_cells.begin(), m_cells.end(), less);
    makeCellViews();
}

void sad::layouts::Grid::shrinkRows(size_t oldrows, size_t newrows)
{
    sad::Vector<sad::layouts::Cell*> toberemoved;
    for(size_t row = oldrows - 1; row >= newrows; row--)
    {
        for(size_t col = 0; col < m_cols; col++)
        {
            sad::layouts::Cell* oldcell = this->cell(row, col);
            if ((oldcell->Row + oldcell->rowSpan()) > row)
            {
                if (oldcell->rowSpan() > 1)
                {
                    oldcell->setRowSpan(row - oldcell->Row);
                }
                else
                {
                    toberemoved << oldcell;
                }
            }
        }
    }

    std::unique(toberemoved.begin(), toberemoved.end()); //-V530
    for(size_t i = 0; i < toberemoved.size(); i++)
    {
        delete toberemoved[i];
        m_cells.removeAll(toberemoved[i]);
    }
    CellComparator less;
    std::sort(m_cells.begin(), m_cells.end(), less);
    makeCellViews();
}

void sad::layouts::Grid::expandColumns(size_t oldcols, size_t newcols)
{
    sad::db::Database* db = NULL;
    sad::db::Table* table = this->table();
    if (table) 
    {
        db = table->database();
    }
    for(size_t col = oldcols; col < newcols; col++)
    {
        for(size_t row = 0; row < m_rows; row++)
        {
            sad::layouts::Cell* newcell = new sad::layouts::Cell();
            newcell->setGrid(this);
            newcell->setDatabase(db);
            newcell->setPaddingBottom(m_padding_bottom, false);
            newcell->setPaddingTop(m_padding_top, false);
            newcell->setPaddingLeft(m_padding_left, false);
            newcell->setPaddingRight(m_padding_right, false);
            newcell->Row = row;
            newcell->Col = col;
            m_cells << newcell;
        }
    }
    CellComparator less;
    std::sort(m_cells.begin(), m_cells.end(), less);
    makeCellViews();
}

void sad::layouts::Grid::shrinkColumns(size_t oldcols, size_t newcols)
{
    sad::Vector<sad::layouts::Cell*> toberemoved;
    for(size_t col = oldcols - 1; col >= newcols; col--)
    {
        for(size_t row = 0; row < m_rows; row++)
        {
            sad::layouts::Cell* oldcell = this->cell(row, col);
            if ((oldcell->Col + oldcell->colSpan()) > col)
            {
                if (oldcell->colSpan() > 1)
                {
                    oldcell->setColSpan(col - oldcell->Col);
                }
                else
                {
                    toberemoved << oldcell;
                }
            }
        }
    }

    std::unique(toberemoved.begin(), toberemoved.end()); //-V530
    for(size_t i = 0; i < toberemoved.size(); i++)
    {
        delete toberemoved[i];
        m_cells.removeAll(toberemoved[i]);
    }
    CellComparator less;
    std::sort(m_cells.begin(), m_cells.end(), less);
    makeCellViews();
}

void sad::layouts::Grid::makeCellViews()
{
    m_cell_views.clear();
    sad::Hash<size_t, sad::Hash<size_t, sad::Vector<size_t> > > coverage;
    buildCoverage(coverage);
    for(size_t i = 0; i < m_rows; i++)
    {
        for(size_t j = 0; j < m_cols; j++)
        {
            if (coverage.contains(i))
            {
                const sad::Hash<size_t, sad::Vector<size_t> >& coltopos = coverage[i];
                if (coltopos.contains(j))
                {
                    const sad::Vector<size_t>& positions =  coltopos[j];
                    // Exactly one cell should correspond to each cell slot
                    if (positions.size() == 1) 
                    {
                        m_cell_views << m_cells[positions[0]];
                    }
                }       
            }
        }
    }
}

bool sad::layouts::Grid::validate() const
{
    bool result = true;
    sad::Hash<size_t, sad::Hash<size_t, sad::Vector<size_t> > > coverage;
    buildCoverage(coverage);
    for(size_t i = 0; i < m_rows; i++)
    {
        for(size_t j = 0; j < m_cols; j++)
        {
            bool current_result = false;
            if (coverage.contains(i))
            {
                const sad::Hash<size_t, sad::Vector<size_t> >& coltopos = coverage[i];
                if (coltopos.contains(j))
                {
                    const sad::Vector<size_t>& positions =  coltopos[j];
                    // Exactly one cell should correspond to each cell slot
                    if (positions.size() == 1) 
                    {
                        sad::layouts::Cell* cell = m_cells[positions[0]];
                        // Cell should be in bounds of grid
                        if ((cell->Row + cell->rowSpan()) <= m_rows
                            && (cell->Col + cell->colSpan()) <= m_cols)
                        {
                            current_result = true;
                        }
                    }
                }
            }
            
            result = result && current_result;
        }
    }
    return result;
}

void sad::layouts::Grid::buildCoverage(sad::Hash<size_t, sad::Hash<size_t, sad::Vector<size_t> > >& coverage) const
{
    coverage.clear();
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        for(size_t rowpos = 0; rowpos < m_cells[i]->rowSpan(); rowpos++)
        {
            for(size_t colpos = 0; colpos < m_cells[i]->colSpan(); colpos++)
            {
                size_t currow = m_cells[i]->Row + rowpos;
                size_t curcol = m_cells[i]->Col + colpos;

                if (coverage.contains(currow) == false)
                {
                    coverage.insert(currow, sad::Hash<size_t, sad::Vector<size_t> >());
                }
                sad::Hash<size_t, sad::Vector<size_t> >& coltopos = coverage[currow];
                if (coltopos.contains(curcol) == false)
                {
                    coltopos.insert(curcol, sad::Vector<size_t>());
                }
                sad::Vector<size_t>& vec = coltopos[curcol];
                vec << i;
            }
        }
    }
}
