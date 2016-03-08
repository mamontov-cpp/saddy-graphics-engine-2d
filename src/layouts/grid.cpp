#include "layouts/grid.h"

#include "db/dbtable.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/schema/schema.h"

#include "renderer.h"
#include "primitiverenderer.h"
#include "sadmutex.h"

#include <stdexcept>

// =================================== PUBLIC METHODS ===================================

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
        this->fixCellViews();
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
        if (oldrows != m_rows)
        {
            this->updateCells();
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
        if (m_cols != oldcols)
        {
            this->updateCells();
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
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        delete m_cells[i];
    }
    m_cells.clear();
    sad::db::Database* db = this->table()->database();
    for(size_t i = 0; i < cells.size(); i++)
    {
        sad::layouts::Cell* cell = new sad::layouts::Cell();
        cell->setDatabase(db);
        cell->fromSerializable(cells[i], db);
        m_cells << cell;
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

void sad::layouts::Grid::update()
{
    
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

void sad::layouts::Grid::fixCellViews()
{
    unsigned int size = m_rows * m_cols;
    sad::db::Database* db = this->table()->database();
    sad::Hash<size_t, size_t> uncovered_cells;
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        uncovered_cells.insert(i, i);
    }

    sad::Hash<size_t, sad::Hash<size_t, sad::Vector<size_t> > > coverage;
    buildCoverage(coverage);
    bool changed = true;
    while(changed)
    {
        changed = false;
        for(sad::Hash<size_t, sad::Hash<size_t, sad::Vector<size_t> > >::iterator iit = coverage.begin();
            iit != coverage.end();
            ++iit)
        {
            sad::Hash<size_t, sad::Vector<size_t> >& jitsource = iit.value();
            for(sad::Hash<size_t, sad::Vector<size_t> >::iterator jit = jitsource.begin();
                jit != jitsource.end();
                ++jit)
            {
                if (jit.value().size() > 1)
                {
                    changed = true;
                }
            }
        }
    }

    // Add missing cells
    while(m_cells.size() < size)
    {
        sad::layouts::Cell* cell = new sad::layouts::Cell();
        cell->setDatabase(db);
        cell->setPaddingBottom(m_padding_bottom, false);
        cell->setPaddingTop(m_padding_top, false);
        cell->setPaddingLeft(m_padding_left, false);
        cell->setPaddingRight(m_padding_right, false);
        m_cells << cell;            
    }
    for(size_t i = 0 ; i < m_rows; i++)
    {
        for(size_t j = 0; j < m_cols; j++)
        {
            
        }
    }
}

void sad::layouts::Grid::buildCoverage(sad::Hash<size_t, sad::Hash<size_t, sad::Vector<size_t> > >& coverage)
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
                sad::Hash<size_t, sad::Vector<size_t> >& colToPos = coverage[currow];
                if (colToPos.contains(curcol) == false)
                {
                    colToPos.insert(curcol, sad::Vector<size_t>());
                }
                sad::Vector<size_t>& vec = colToPos[curcol];
                vec << i;
            }
        }
    }
}