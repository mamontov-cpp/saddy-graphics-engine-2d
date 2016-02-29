#include "layouts/grid.h"

#include "renderer.h"
#include "primitiverenderer.h"

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
	if (pos < m_cells.size())
	{
		result = m_cells[pos];
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
    return result;
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
	if (!m_rows)
	{
		m_rows = 1;
	}
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
	if (!m_cols)
	{
		m_cols = 1;
	}
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
