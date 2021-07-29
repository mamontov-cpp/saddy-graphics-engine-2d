#include "layouts/grid.h"

#include "db/dbtable.h"
#include "db/dbmethodpair.h"
#include "db/schema/schema.h"

#include "util/free.h"

#include "renderer.h"
#include "primitiverenderer.h"
#include "sadmutex.h"
#include "geometry2d.h"

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
m_renderer(nullptr),
m_loading(false)
{
    
}

sad::layouts::Grid::~Grid()
{
    
}

sad::layouts::Cell* sad::layouts::Grid::cell(unsigned int row, unsigned int col)
{
    sad::layouts::Cell* result = nullptr;
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
                        r->render()->rectangle(this->scene(), cl->AssignedArea, clr, this->m_shader_function);
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

sad::Vector<sad::Rect2D> sad::layouts::Grid::getRegions()
{
    sad::Vector<sad::Rect2D> r;
    r << m_area;
    return r;
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

static sad::db::schema::Schema* LayoutsGridSchema = nullptr;
static sad::Mutex LayoutsGridSchemaInit;

sad::db::schema::Schema* sad::layouts::Grid::basicSchema()
{
    if (LayoutsGridSchema == nullptr)
    {
        LayoutsGridSchemaInit.lock();
        if (LayoutsGridSchema == nullptr)
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
    unsigned int oldrows = m_rows;
    if (!m_loading)
    {
        if (oldrows > rows)
        {
            this->shrinkRows(oldrows, rows);
        }
        if (oldrows < rows)
        {
            this->expandRows(oldrows, rows);
        }
    }
    m_rows = rows;
    if (oldrows != rows)
    {
        if (!m_loading)
        {
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
    if (!m_loading)
    {
        if (oldcols > cols)
        {
            this->shrinkColumns(oldcols, cols);
        }
        if (oldcols < cols)
        {
            this->expandColumns(oldcols, cols);
        }
    }
    m_cols = cols;
    if (oldcols != m_cols)
    {
        if (!m_loading)
        {
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
        if (!m_loading)
        {
            this->update();
        }
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
        if (!m_loading)
        {
            this->update();
        }
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
        if (!m_loading)
        {
            this->update();
        }
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
        if (!m_loading)
        {
            this->update();
        }
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
    sad::db::Database* db = nullptr;
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
    sad::layouts::Cell* result = nullptr;
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
    if (((row + row_span > m_rows)) || ((col + col_span) > m_cols) || (row_span == 0) || (col_span == 0))
    {
        return false;
    }
    sad::Hash<size_t, sad::layouts::Cell*> cells_to_be_trimmed;
    this->cellsAffectedByRegion(row, col, row_span, col_span, cells_to_be_trimmed);
    size_t hash = row * m_cols + col;
    sad::layouts::Cell* cell;
    if (cells_to_be_trimmed.contains(hash))
    {
        cell = cells_to_be_trimmed[hash];
    }
    else
    {
        cell = this->makeCell(row, col, 1, 1);
        m_cells << cell;
    }

    sad::Vector<sad::layouts::Cell*> to_be_erased;
    this->recalculateSpansOfAffectedCells(row, col, row_span, col_span, true, cells_to_be_trimmed, to_be_erased);
    
    CellComparator less;

    std::unique(to_be_erased.begin(), to_be_erased.end()); //-V530
    std::sort(to_be_erased.begin(), to_be_erased.end(), less);
    for(size_t i = 0; i < to_be_erased.size(); i++)
    {
        for(size_t j = 0; j < to_be_erased[i]->m_children.size(); j++)
        {
            cell->m_children << to_be_erased[i]->m_children[j];
        }
        to_be_erased[i]->m_children.clear();
        delete to_be_erased[i];
        m_cells.removeAll(to_be_erased[i]);
    }       
    cell->setRowSpan(row_span);
    cell->setColSpan(col_span);  
    std::sort(m_cells.begin(), m_cells.end(), less);
    makeCellViews();
    this->update();
    return true;
}

bool sad::layouts::Grid::split(size_t row, size_t col, size_t row_span, size_t col_span)
{
    if ((row + row_span > m_rows) || ((col + col_span) > m_cols) || (row_span == 0) || (col_span == 0))
    {
        return false;
    }
    sad::Hash<size_t, sad::layouts::Cell*> cells_to_be_trimmed;
    this->cellsAffectedByRegion(row, col, row_span, col_span, cells_to_be_trimmed);
    // Make new cells to fill free place, after old cells will return to their old state
    for(size_t i = 0; i < row_span; i++)
    {
        for(size_t j = 0; j < col_span; j++)
        {
            size_t pos = (row + i) * m_cols + (col + j);
            if (cells_to_be_trimmed.contains(pos) == false)
            {
                m_cells << this->makeCell(row + i, col + j, 1, 1);
            }
        }
    }
    sad::Vector<sad::layouts::Cell*> tmp;
    this->recalculateSpansOfAffectedCells(row, col, row_span, col_span, false, cells_to_be_trimmed, tmp);
    CellComparator less;
    std::sort(m_cells.begin(), m_cells.end(), less);
    makeCellViews();
    this->update();
    return true;
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

    // 3.1. Fill zero-size rows with redistributed left-space from other cells
    if (this->fixedWidth())
    {
        double width_sum = 0;
        size_t amount = 0;
        // Compute total amount of space to be redistributed
        // and amount of empty cells
        for(size_t col = 0; col  < m_cols; col++)
        {
            if (sad::is_fuzzy_zero(coltowidth[col]))
            {
                ++amount;
            }
            else
            {
                width_sum += coltowidth[col];
            }
        }
        // Redistribute empty cells if needed
        if (width_sum < m_area.width() && amount != 0)
        {
            double w = (m_area.width() - width_sum) / amount;
            for(size_t col = 0; col  < m_cols; col++)
            {
                if (sad::is_fuzzy_zero(coltowidth[col]))
                {
                    coltowidth[col] = w;
                }                    
            }
        }
    }

    // 3.2. Fill zero-size cols with redistributed left-space from other cells
    if (this->fixedHeight())
    {
        double height_sum = 0;
        size_t amount = 0;
        // Compute total amount of space to be redistributed
        // and amount of empty cells
        for(size_t row = 0; row < m_rows; row++)
        {
            if (sad::is_fuzzy_zero(rowtoheight[row]))
            {
                ++amount;
            }
            else
            {
                height_sum += rowtoheight[row];
            }
        }
        // Redistribute empty cells if needed
        if (height_sum < m_area.height() && amount != 0)
        {
            double h = (m_area.height() - height_sum) / amount;
            for(size_t row = 0; row < m_rows; row++)
            {
                if (sad::is_fuzzy_zero(rowtoheight[row]))
                {
                    rowtoheight[row] = h;
                }                    
            }
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

sad::Vector<sad::SceneNode*> sad::layouts::Grid::children() const
{
    sad::Vector<sad::SceneNode*> result;
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        result << m_cells[i]->children();
    }
    return result;
}

size_t sad::layouts::Grid::allocatedCellCount() const
{
    return m_cells.size();
}

void sad::layouts::Grid::setTable(sad::db::Table* t)
{
    this->sad::SceneNode::setTable(t);
    sad::db::Database* db = nullptr;
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
    sad::db::Database* db = nullptr;
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
    makeCellViews(&newrows, nullptr);
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
    makeCellViews(&newrows, nullptr);
}

void sad::layouts::Grid::expandColumns(size_t oldcols, size_t newcols)
{
    sad::db::Database* db = nullptr;
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
    makeCellViews(nullptr, &newcols);
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
    makeCellViews(nullptr, &newcols);
}

void sad::layouts::Grid::makeCellViews(size_t* prows, size_t* pcols)
{
    size_t rows = m_rows;
    size_t cols = m_cols;
    if (prows)
    {
        rows = *prows;
    }
    if (pcols)
    {
        cols = *pcols;
    }
    m_cell_views.clear();
    sad::Hash<size_t, sad::Hash<size_t, sad::Vector<size_t> > > coverage;
    buildCoverage(coverage);
    for(size_t i = 0; i < rows; i++)
    {
        for(size_t j = 0; j < cols; j++)
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

sad::layouts::Cell* sad::layouts::Grid::makeCell(size_t row, size_t col, size_t rowspan, size_t colspan)
{
    sad::db::Database* db = nullptr;
    if (this->table())
    {
        db = this->table()->database();
    }
    sad::layouts::Cell* newcell = new sad::layouts::Cell();
    newcell->setGrid(this);
    newcell->setDatabase(db);
    newcell->setPaddingBottom(m_padding_bottom, false);
    newcell->setPaddingTop(m_padding_top, false);
    newcell->setPaddingLeft(m_padding_left, false);
    newcell->setPaddingRight(m_padding_right, false);
    newcell->Row = row;
    newcell->Col = col;
    newcell->setRowSpan(rowspan);
    newcell->setColSpan(colspan);

    return newcell;
}


void sad::layouts::Grid::cellsAffectedByRegion(
    size_t row, 
    size_t col, 
    size_t rowspan, 
    size_t colspan,
    sad::Hash<size_t, sad::layouts::Cell*>& affected_cells
)
{
    size_t rangerowmax = row + rowspan - 1;
    size_t rangecolmax = col + colspan - 1;
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        unsigned int rowmin = m_cells[i]->Row;
        unsigned int colmin = m_cells[i]->Col;
        unsigned int rowmax = m_cells[i]->Row + m_cells[i]->rowSpan() - 1;
        unsigned int colmax = m_cells[i]->Col + m_cells[i]->colSpan() - 1;
        
        if (sad::collides1D<size_t>(row, rangerowmax, rowmin, rowmax) && sad::collides1D<size_t>(col, rangecolmax, colmin, colmax))
        {
            affected_cells.insert(m_cells[i]->Row * m_cols + m_cells[i]->Col, m_cells[i]);
        }           
    }    
}

void sad::layouts::Grid::recalculateSpansOfAffectedCells(
    size_t row, 
    size_t col, 
    size_t rowspan, 
    size_t colspan,
    bool merge,
    const sad::Hash<size_t, sad::layouts::Cell*>& affected_cells,
    sad::Vector<sad::layouts::Cell*>& to_be_erased
)
{
    size_t rangerowmax = row + rowspan - 1;
    size_t rangecolmax = col + colspan - 1;
    for(sad::Hash<size_t, sad::layouts::Cell*>::const_iterator it = affected_cells.const_begin(); it != affected_cells.const_end(); ++it) {
        sad::layouts::Cell* cell = it.value();
        unsigned int rowmin = cell->Row;
        unsigned int colmin = cell->Col;
        unsigned int rowmax = cell->Row + cell->rowSpan() - 1;
        unsigned int colmax = cell->Col + cell->colSpan() - 1;
    //  Template for handled case descriptions
    //  Vertical bounds describe affected region, top-left and bottom right points of cell are marked with vertical lines
    //  _1,1_  _1,2_  _1,3_  |  _1,4_  _1,5_   |  _1,6_  _1,7_  _1,8_
    //  _2,1_  _2,2_  _2,3_  |  _2,4_  _2,5_   |  _2,6_  _2,7_  _2,8_
    //  _____________________________________________________________
    //  _3,1_  _3,2_  _3,3_  |  _3,4_  _3,5_   |  _3,6_  _3,7_  _3,8_
    //  _4,1_  _4,2_  _4,3_  |  _4,4_  _4,5_   |  _4,6_  _4,7_  _4,8_
    //  _____________________________________________________________
    //  _5,1_  _5,2_  _5,3_  |  _5,4_  _5,5_   |  _5,6_  _5,7_  _5,8_
    //  _6,1_  _6,2_  _6,3_  |  _6,4_  _6,5_   |  _6,6_  _6,7_  _6,8_                    

        if (rowmin < row)
        {
            if (colmin < col)
            {
                // Case when row and column are in left top corner from range
                if (colmax <= rangecolmax)
                {
                    //  Handle cases, like following: 
                    //  Vertical bounds describe affected region, top-left and bottom right points of cell are marked with vertical lines
                    //  *******************     ************
                    //  |1,1|  _1,2_  _1,3*  |  *1,4_  _1,5*   |  _1,6_  _1,7_  _1,8_
                    //  *2,1_  _2,2_  _2,3*  |  *2,4_  _2,5*   |  _2,6_  _2,7_  _2,8_
                    //  ________________________************_________________________
                    //  *3,1_  _3,2_  _3,3*  |  _3,4_  _3,5_   |  _3,6_  _3,7_  _3,8_
                    //  *4,1_  _4,2_  _4,3*  |  _4,4_  |4,5|   |  _4,6_  _4,7_  _4,8_
                    //  *******************__________________________________________
                    //  _5,1_  _5,2_  _5,3_  |  _5,4_  _5,5_   |  _5,6_  _5,7_  _5,8_
                    //  _6,1_  _6,2_  _6,3_  |  _6,4_  _6,5_   |  _6,6_  _6,7_  _6,8_                    
                    if (rowmax <= rangerowmax) 
                    {
                        // Split onto part before affected region and on top of it
                        cell->setColSpan(col - colmin);
                        m_cells << this->makeCell(rowmin, col, row - rowmin, colmax - col + 1);
                    }
                    //  Handle cases, like following: 
                    //  Vertical bounds describe affected region, top-left and bottom right points of cell are marked with vertical lines
                    //  *******************     ************
                    //  |1,1|  _1,2_  _1,3*  |  *1,4_  _1,5*   |  _1,6_  _1,7_  _1,8_
                    //  *2,1_  _2,2_  _2,3*  |  *2,4_  _2,5*   |  _2,6_  _2,7_  _2,8_
                    //  ________________________************_________________________
                    //  *3,1_  _3,2_  _3,3*  |  _3,4_  _3,5_   |  _3,6_  _3,7_  _3,8_
                    //  *4,1_  _4,2_  _4,3*  |  _4,4_  _4,5_   |  _4,6_  _4,7_  _4,8_
                    //  _____________________________________________________________
                    //                       |  *************  |
                    //  _5,1_  _5,2_  _5,3_  |  *5,4_  |5,5|*  |  _5,6_  _5,7_  _5,8_
                    //  *******************  |  *************  |
                    //  _6,1_  _6,2_  _6,3_  |  _6,4_  _6,5_   |  _6,6_  _6,7_  _6,8_ 
                    else
                    {
                        // Split onto part before affected region, on top of it and on bottom of it
                        cell->setColSpan(col - colmin);
                        m_cells << this->makeCell(rowmin, col, row - rowmin, colmax - col + 1);
                        m_cells << this->makeCell(rangerowmax + 1, col, rowmax - rangerowmax, colmax - col + 1);
                    }
                }
                else
                {
                    //  Handle cases, like following: 
                    //  Vertical bounds describe affected region, top-left and bottom right points of cell are marked with vertical lines
                    //  *********************     **************     ***************
                    //  *|1,1|  _1,2_  _1,3_*  |  *_1,4_  _1,5_*   | *_1,6_   _1,7_*  _1,8_
                    //  *_2,1_  _2,2_  _2,3_*  |  *_2,4_  _2,5_*   | *_2,6_   _2,7_*  _2,8_
                    //  __________________________**************_____*_____________*_______
                    //  *_3,1_  _3,2_  _3,3_*  |   _3,4_  _3,5_    | *_3,6_   |3,7|*  _3,8_
                    //  *********************                        **************
                    //   _4,1_  _4,2_  _4,3_   |   _4,4_  _4,5_    |  _4,6_   _4,7_   _4,8_
                    //  _____________________________________________________________
                    //   _5,1_  _5,2_  _5,3_   |   _5,4_  _5,5_    |  _5,6_   _5,7_   _5,8_
                    //   _6,1_  _6,2_  _6,3_   |   _6,4_  _6,5_    |  _6,6_   _6,7_   _6,8_ 
                    if (rowmax <= rangerowmax) 
                    {
                        // Split onto part before affected region, on top of it and behind it
                        cell->setColSpan(col - colmin);
                        m_cells << this->makeCell(rowmin, col, row - rowmin, rangecolmax - col + 1);
                        m_cells << this->makeCell(rowmin, rangecolmax + 1, rowmax - rowmin + 1, colmax - rangecolmax);
                    }
                    //  Handle cases, like following: 
                    //  Vertical bounds describe affected region, top-left and bottom right points of cell are marked with vertical lines
                    //  *********************     **************     ***************
                    //  *|1,1|  _1,2_  _1,3_*  |  *_1,4_  _1,5_*   | *_1,6_   _1,7_*  _1,8_
                    //  *_2,1_  _2,2_  _2,3_*  |  *_2,4_  _2,5_*   | *_2,6_   _2,7_*  _2,8_
                    //  __________________________**************_____*_____________*_______
                    //  *_3,1_  _3,2_  _3,3_*  |   _3,4_  _3,5_    | *_3,6_   _3,7_*  _3,8_
                    //  *_4,1_  _4,2_  _4,3_*  |   _4,4_  _4,5_    | *_4,6_   _4,7_*  _4,8_
                    //  __________________________**************_____*_____________*_______
                    //  *_5,1_  _5,2_  _5,3_*  |  *_5,4_  _5,5_*   | *_5,6_   |5,7|*  _5,8_
                    //  *********************     **************     ***************
                    //   _6,1_  _6,2_  _6,3_   |   _6,4_  _6,5_    |  _6,6_   _6,7_  _6,8_ 
                    else
                    {
                        // Split onto part before affected region, on top of it, on bottom of it and behind it
                        cell->setColSpan(col - colmin);
                        m_cells << this->makeCell(rowmin, col, row - rowmin, rangecolmax - col + 1);
                        m_cells << this->makeCell(rangerowmax + 1, col, rowmax - rangerowmax, rangecolmax - col + 1);
                        m_cells << this->makeCell(rowmin, rangecolmax + 1, rowmax - rowmin + 1, colmax - rangecolmax);
                    }
                }
            }
            else
            {
                // Case when row and column in top middle part of region
                if (colmax <= rangecolmax)
                {
                    //  Handle cases, like:
                    //                          **************
                    //  _1,1_  _1,2_  _1,3_  |  *|1,4|  _1,5_*   |  _1,6_  _1,7_  _1,8_
                    //  _2,1_  _2,2_  _2,3_  |  *_2,4_  _2,5_*   |  _2,6_  _2,7_  _2,8_
                    //  ________________________**************_________________________
                    //  _3,1_  _3,2_  _3,3_  |   _3,4_  _3,5_    |  _3,6_  _3,7_  _3,8_
                    //  _4,1_  _4,2_  _4,3_  |   _4,4_  |4,5|    |  _4,6_  _4,7_  _4,8_
                    //  _______________________________________________________________
                    //  _5,1_  _5,2_  _5,3_  |   _5,4_  _5,5_    |  _5,6_  _5,7_  _5,8_
                    //  _6,1_  _6,2_  _6,3_  |   _6,4_  _6,5_    |  _6,6_  _6,7_  _6,8_                   
                    if (rowmax <= rangerowmax)
                    {
                        cell->setRowSpan(row - rowmin);
                    }
                    //  Handle cases, like:
                    //                          **************
                    //  _1,1_  _1,2_  _1,3_  |  *|1,4|  _1,5_*   |  _1,6_  _1,7_  _1,8_
                    //  _2,1_  _2,2_  _2,3_  |  *_2,4_  _2,5_*   |  _2,6_  _2,7_  _2,8_
                    //  ________________________**************_________________________
                    //  _3,1_  _3,2_  _3,3_  |   _3,4_  _3,5_    |  _3,6_  _3,7_  _3,8_
                    //  _4,1_  _4,2_  _4,3_  |   _4,4_  _4,5_    |  _4,6_  _4,7_  _4,8_
                    //  ________________________**************_________________________
                    //  _5,1_  _5,2_  _5,3_  |  *_5,4_  _5,5_*   |  _5,6_  _5,7_  _5,8_
                    //  _6,1_  _6,2_  _6,3_  |  *_6,4_  |6,5|*   |  _6,6_  _6,7_  _6,8_                   
                    //                          **************
                    else
                    {
                        cell->setRowSpan(row - rowmin);   
                        m_cells << this->makeCell(rangerowmax + 1, colmin, rowmax - rangerowmax, colmax - colmin + 1);                       
                    }
                }
                else
                {
                    //  Handle cases, like:
                    //                          **************     **************
                    //  _1,1_  _1,2_  _1,3_  |  *|1,4|  _1,5_*   | *_1,6_  _1,7_*  _1,8_
                    //  _2,1_  _2,2_  _2,3_  |  *_2,4_  _2,5_*   | *_2,6_  _2,7_*  _2,8_
                    //  ________________________**************_____*____________*______
                    //  _3,1_  _3,2_  _3,3_  |   _3,4_  _3,5_    | *_3,6_  _3,7_*  _3,8_
                    //  _4,1_  _4,2_  _4,3_  |   _4,4_  _4,5_    | *_4,6_  |4,7|*  _4,8_
                    //  ___________________________________________**************______
                    //  _5,1_  _5,2_  _5,3_  |   _5,4_  _5,5_    |  _5,6_  _5,7_   _5,8_
                    //  _6,1_  _6,2_  _6,3_  |   _6,4_  _6,5_    |  _6,6_  _6,7_   _6,8_   
                    if (rowmax <= rangerowmax)
                    {
                        cell->setRowSpan(row - rowmin);  
                        cell->setColSpan(rangecolmax - colmin + 1);
                        m_cells << this->makeCell(rowmin, rangecolmax + 1, rowmax - rowmin + 1, colmax - rangecolmax); 
                    }
                    //  Handle cases, like:
                    //                          **************     **************
                    //  _1,1_  _1,2_  _1,3_  |  *|1,4|  _1,5_*   | *_1,6_  _1,7_*  _1,8_
                    //  _2,1_  _2,2_  _2,3_  |  *_2,4_  _2,5_*   | *_2,6_  _2,7_*  _2,8_
                    //  ________________________**************_____*____________*______
                    //  _3,1_  _3,2_  _3,3_  |   _3,4_  _3,5_    | *_3,6_  _3,7_*  _3,8_
                    //  _4,1_  _4,2_  _4,3_  |   _4,4_  _4,5_    | *_4,6_  |4,7|*  _4,8_
                    //  ________________________**************_____*____________*______
                    //  _5,1_  _5,2_  _5,3_  |  *_5,4_  _5,5_*   | *_5,6_  _5,7_*  _5,8_
                    //  _6,1_  _6,2_  _6,3_  |  *_6,4_  _6,5_*   | *_6,6_  |6,7|*  _6,8_
                    //                          **************     **************
                    else
                    {
                        cell->setRowSpan(row - rowmin);  
                        cell->setColSpan(rangecolmax - colmin + 1);
                        m_cells << this->makeCell(rangerowmax + 1, colmin, rowmax - rangerowmax, rangecolmax - colmin + 1);  
                        m_cells << this->makeCell(rowmin, rangecolmax + 1, rowmax - rowmin + 1, colmax - rangecolmax);                         
                    }
                }                
            }
        }
        else
        {
            if (colmin < col)
            {
                if (colmax <= rangecolmax)
                {
                    //  Handle cases, like following: 
                    //  Vertical bounds describe affected region, top-left and bottom right points of cell are marked with vertical lines
                    //   _1,1_  _1,2_  _1,3_   |  _1,4_  _1,5_   |  _1,6_  _1,7_  _1,8_
                    //   _2,1_  _2,2_  _2,3_   |  _2,4_  _2,5_   |  _2,6_  _2,7_  _2,8_
                    //  *********************_________________________________________
                    //  *|3,1|  _3,2_  _3,3_*  |  _3,4_  _3,5_   |  _3,6_  _3,7_  _3,8_
                    //  *_4,1_  _4,2_  _4,3_*  |  _4,4_  |4,5|   |  _4,6_  _4,7_  _4,8_
                    //  *********************_________________________________________
                    //   _5,1_  _5,2_  _5,3_   |  _5,4_  _5,5_   |  _5,6_  _5,7_  _5,8_
                    //   _6,1_  _6,2_  _6,3_   |  _6,4_  _6,5_   |  _6,6_  _6,7_  _6,8_                    
                    if (rowmax <= rangerowmax) 
                    {
                        cell->setColSpan(col - colmin);
                    }
                    //  Handle cases, like following: 
                    //  Vertical bounds describe affected region, top-left and bottom right points of cell are marked with vertical lines
                    //   _1,1_  _1,2_  _1,3_   |  _1,4_  _1,5_   |  _1,6_  _1,7_  _1,8_
                    //   _2,1_  _2,2_  _2,3_   |  _2,4_  _2,5_   |  _2,6_  _2,7_  _2,8_
                    //  *********************_________________________________________
                    //  *|3,1|  _3,2_  _3,3_*  |  _3,4_  _3,5_   |  _3,6_  _3,7_  _3,8_
                    //  *_4,1_  _4,2_  _4,3_*  |  _4,4_  _4,5_   |  _4,6_  _4,7_  _4,8_
                    //  *___________________*_____**************______________________
                    //  *_5,1_  _5,2_  _5,3_*   | *_5,4_  _5,5_*  |  _5,6_  _5,7_  _5,8_
                    //  *_6,1_  _6,2_  _6,3_*   | *_6,4_  |6,5|*  |  _6,6_  _6,7_  _6,8_ 
                    //  *********************   | **************  |
                    else
                    {
                        cell->setColSpan(col - colmin);
                        m_cells << this->makeCell(rangerowmax + 1, col, rowmax - rangerowmax, colmax - col + 1);
                    }
                }
                else
                {
                    //  Handle cases, like following: 
                    //  Vertical bounds describe affected region, top-left and bottom right points of cell are marked with vertical lines
                    //   _1,1_  _1,2_  _1,3_   |  _1,4_  _1,5_   |   _1,6_  _1,7_   _1,8_
                    //   _2,1_  _2,2_  _2,3_   |  _2,4_  _2,5_   |   _2,6_  _2,7_   _2,8_
                    //  *********************_______________________**************___
                    //  *|3,1|  _3,2_  _3,3_*  |  _3,4_  _3,5_   |  *_3,6_  _3,7_*  _3,8_
                    //  *_4,1_  _4,2_  _4,3_*  |  _4,4_  _4,5_   |  *_4,6_  |4,7|*  _4,8_
                    //  *********************_______________________*************___
                    //   _5,1_  _5,2_  _5,3_   |  _5,4_  _5,5_   |   _5,6_  _5,7_   _5,8_
                    //   _6,1_  _6,2_  _6,3_   |  _6,4_  _6,5_   |   _6,6_  _6,7_   _6,8_  
                    if (rowmax <= rangerowmax) 
                    {
                        cell->setColSpan(col - colmin);
                        m_cells << this->makeCell(rowmin, rangecolmax + 1, rowmax - rowmin + 1, colmax - rangecolmax);
                    }
                    //  Handle cases, like following: 
                    //  Vertical bounds describe affected region, top-left and bottom right points of cell are marked with vertical lines
                    //   _1,1_  _1,2_  _1,3_   |  _1,4_  _1,5_   |   _1,6_  _1,7_   _1,8_
                    //   _2,1_  _2,2_  _2,3_   |  _2,4_  _2,5_   |   _2,6_  _2,7_   _2,8_
                    //  *********************_______________________**************___
                    //  *|3,1|  _3,2_  _3,3_*  |  _3,4_  _3,5_   |  *_3,6_  _3,7_*  _3,8_
                    //  *_4,1_  _4,2_  _4,3_*  |  _4,4_  _4,5_   |  *_4,6_  |4,7|*  _4,8_
                    //  _________________________**************_____*____________*___
                    //  *_5,1_  _5,2_  _5,3_*  | *_5,4_  _5,5_*  |  *_5,6_  _5,7_*  _5,8_
                    //  *_6,1_  _6,2_  _6,3_*  | *_6,4_  _6,5_*  |  *_6,6_  |6,7|*  _6,8_ 
                    //  *********************    **************     **************
                    else
                    {
                        cell->setColSpan(col - colmin);
                        m_cells << this->makeCell(rangerowmax + 1, col, rowmax - rangerowmax, rangecolmax - col + 1);
                        m_cells << this->makeCell(rowmin, rangecolmax + 1, rowmax - rowmin + 1, colmax - rangecolmax);
                    }
                }
            }
            else
            {
                // Perform context-dependent action here (depends whether we merge or split)

                // For split, we just set cell to one size
                if (merge)
                {
                    if ((rowmin != row) || (colmin != col))
                    {
                        to_be_erased << cell;
                    }
                }
                else
                {
                    cell->setRowSpan(1);
                    cell->setColSpan(1);
                }
                if (colmax <= rangecolmax)
                {
                    //  Handle cases like:
                    //  Vertical bounds describe affected region, top-left and bottom right points of cell are marked with vertical lines
                    //  _1,1_  _1,2_  _1,3_  |  _1,4_  _1,5_   |  _1,6_  _1,7_  _1,8_
                    //  _2,1_  _2,2_  _2,3_  |  _2,4_  _2,5_   |  _2,6_  _2,7_  _2,8_
                    //  _____________________________________________________________
                    //  _3,1_  _3,2_  _3,3_  |  |3,4|  _3,5_   |  _3,6_  _3,7_  _3,8_
                    //  _4,1_  _4,2_  _4,3_  |  _4,4_  |4,5|   |  _4,6_  _4,7_  _4,8_
                    //  _____________________________________________________________
                    //  _5,1_  _5,2_  _5,3_  |  _5,4_  _5,5_   |  _5,6_  _5,7_  _5,8_
                    //  _6,1_  _6,2_  _6,3_  |  _6,4_  _6,5_   |  _6,6_  _6,7_  _6,8_ 
                    if (rowmax <= rangerowmax) 
                    {
                        // DO NOTHING: Already done stuff before it
                    }
                    //  Handle cases like:
                    //  Vertical bounds describe affected region, top-left and bottom right points of cell are marked with vertical lines
                    //  _1,1_  _1,2_  _1,3_  |  _1,4_  _1,5_   |  _1,6_  _1,7_  _1,8_
                    //  _2,1_  _2,2_  _2,3_  |  _2,4_  _2,5_   |  _2,6_  _2,7_  _2,8_
                    //  _____________________________________________________________
                    //  _3,1_  _3,2_  _3,3_  |  |3,4|  _3,5_   |  _3,6_  _3,7_  _3,8_
                    //  _4,1_  _4,2_  _4,3_  |  _4,4_  _4,5_   |  _4,6_  _4,7_  _4,8_
                    //  ________________________**************_______________________
                    //  _5,1_  _5,2_  _5,3_  |  *_5,4_  _5,5_* |  _5,6_  _5,7_  _5,8_
                    //  _6,1_  _6,2_  _6,3_  |  *_6,4_  |6,5|* |  _6,6_  _6,7_  _6,8_  
                    //                          **************
                    else
                    {
                         m_cells << this->makeCell(rangerowmax + 1, colmin, rowmax - rangerowmax, colmax - colmin + 1);
                    }
                }
                else
                {
                    //  Handle cases like:
                    //  Vertical bounds describe affected region, top-left and bottom right points of cell are marked with vertical lines
                    //  _1,1_  _1,2_  _1,3_  |  _1,4_  _1,5_   |  _1,6_  _1,7_  _1,8_
                    //  _2,1_  _2,2_  _2,3_  |  _2,4_  _2,5_   |  _2,6_  _2,7_  _2,8_
                    //  _________________________________________**************______
                    //  _3,1_  _3,2_  _3,3_  |  |3,4|  _3,5_   | *_3,6_  _3,7_*  _3,8_
                    //  _4,1_  _4,2_  _4,3_  |  _4,4_  _4,5_   | *_4,6_  |4,7|*  _4,8_
                    //  _________________________________________**************______
                    //  _5,1_  _5,2_  _5,3_  |  _5,4_  _5,5_   |  _5,6_  _5,7_  _5,8_
                    //  _6,1_  _6,2_  _6,3_  |  _6,4_  _6,5_   |  _6,6_  _6,7_  _6,8_ 
                    if (rowmax <= rangerowmax) 
                    {
                        m_cells << this->makeCell(rowmin, rangecolmax + 1, rowmax - rowmin + 1, colmax - rangecolmax);
                    }
                    //  Handle cases like:
                    //  Vertical bounds describe affected region, top-left and bottom right points of cell are marked with vertical lines
                    //  _1,1_  _1,2_  _1,3_  |  _1,4_  _1,5_   |  _1,6_  _1,7_  _1,8_
                    //  _2,1_  _2,2_  _2,3_  |  _2,4_  _2,5_   |  _2,6_  _2,7_  _2,8_
                    //  _________________________________________**************______
                    //  _3,1_  _3,2_  _3,3_  |  |3,4|  _3,5_   | *_3,6_  _3,7_*  _3,8_
                    //  _4,1_  _4,2_  _4,3_  |  _4,4_  _4,5_   | *_4,6_  |4,7|*  _4,8_
                    //  _______________________**************____*____________*______
                    //  _5,1_  _5,2_  _5,3_  | *_5,4_  _5,5_*  | *_5,6_  _5,7_*  _5,8_
                    //  _6,1_  _6,2_  _6,3_  | *_6,4_  _6,5_*  | *_6,6_  |6,7|*  _6,8_
                    //                         **************    **************
                    else
                    {
                        m_cells << this->makeCell(rangerowmax + 1, colmin, rowmax - rangerowmax, rangecolmax - colmin + 1);                        
                        m_cells << this->makeCell(rowmin, rangecolmax + 1, rowmax - rowmin + 1, colmax - rangecolmax);                        
                    }
                }
            }
        }
    }
}