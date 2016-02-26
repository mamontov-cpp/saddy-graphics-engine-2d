#include "layouts/grid.h"
#include <stdexcept>

// =================================== PUBLIC METHODS ===================================

sad::layouts::Grid::Grid()
: m_rows(0),
m_cols(0),
m_padding_top(0),
m_padding_bottom(0),
m_padding_left(0),
m_padding_right(0)
{
	
}

sad::layouts::Grid::~Grid()
{
	
}


// =================================== PRIVATE METHODS ===================================

sad::layouts::Grid::Grid(const sad::layouts::Grid& o)
: m_rows(o.m_rows), 
m_cols(o.m_cols),
m_padding_top(o.m_padding_top),
m_padding_bottom(o.m_padding_bottom),
m_padding_left(o.m_padding_left),
m_padding_right(o.m_padding_right)
{
    throw std::runtime_error("Not implemented");
}

sad::layouts::Grid& sad::layouts::Grid::operator=(const sad::layouts::Grid& o)
{
    throw std::runtime_error("Not implemented");
    // ReSharper disable once CppUnreachableCode
    return *this;
}
