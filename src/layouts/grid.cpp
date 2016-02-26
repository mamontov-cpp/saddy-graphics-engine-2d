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
m_renderer(NULL),
m_render_color(255,0 ,0),
m_loading(false)
{
	
}

sad::layouts::Grid::~Grid()
{
	
}

sad::layouts::Cell* sad::layouts::Grid::cell(unsigned int row, unsigned int col)
{
	sad::layouts::Cell* result = NULL;
	if (m_map.contains(row))
	{
		if (m_map[row].contains(col))
		{
			unsigned int pos = m_map[row][col];
			if (pos < m_cells.size())
			{
				result = m_cells[pos];
			}
		}
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
	for(size_t i = 0; i < m_cells.size(); i++)
	{
		m_cells[i]->Rendered = false;
	}
	sad::AColor clr(255, 0, 0);
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
						r->render()->rectangle(cl->AssignedArea, m_render_color);
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
m_renderer(o.m_renderer),
m_render_color(o.m_render_color),
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
