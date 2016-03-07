#include "layouts/cell.h"
#include "layouts/grid.h"

#include <stdexcept>

// ============================ PUBLIC METHODS ============================

sad::layouts::Cell::Cell() 
: Rendered(false),
m_row_span(1),
m_col_span(1), 
m_valign(sad::layouts::LVA_Middle), 
m_halign(sad::layouts::LHA_Middle),
m_stacking_type(sad::layouts::LST_Horizontal),
m_padding_top(0),
m_padding_bottom(0),
m_padding_left(0),
m_padding_right(0),
m_grid(NULL),
m_db(NULL)
{
    
}

sad::layouts::Cell::~Cell()
{
    
}

void sad::layouts::Cell::toSerializable(sad::layouts::SerializableCell& cell) const
{
    cell.Width = m_width;
    cell.Height = m_height;
    cell.RowSpan = m_row_span;
    cell.ColSpan = m_col_span;
    cell.VAlign = m_valign;
    cell.HAlign = m_halign;
    cell.Children.clear();
    for(size_t i = 0; i < m_children.size(); i++)
    {
        cell.Children << m_children[i]->majorId();
    }
    cell.StackingType = m_stacking_type;
    cell.PaddingTop = m_padding_top;
    cell.PaddingRight = m_padding_right;
    cell.PaddingLeft = m_padding_left;
    cell.PaddingBottom = m_padding_bottom;
    cell.AssignedArea = AssignedArea;
}

void sad::layouts::Cell::fromSerializable(
    const sad::layouts::SerializableCell& cell,
    sad::db::Database* db
)
{
    this->m_width = cell.Width;
    this->m_height = cell.Height;
    this->m_row_span = cell.RowSpan;
    this->m_col_span = cell.ColSpan;
    this->m_valign = cell.VAlign;
    this->m_halign = cell.HAlign;
    for(size_t i = 0; i < m_children.size(); i++)
    {
        delete m_children[i];
    }
    this->m_children.clear();
    this->m_db = db;
    for(size_t i = 0; i < cell.Children.size(); i++)
    {
        sad::db::TypedLink<sad::SceneNode>* child = new sad::db::TypedLink<sad::SceneNode>();
        child->setDatabase(db);
        child->setMajorId(cell.Children[i]);
        this->m_children << child;
    }
    this->m_stacking_type = cell.StackingType;
    this->m_padding_top = cell.PaddingTop;
    this->m_padding_right = cell.PaddingRight;
    this->m_padding_left = cell.PaddingLeft;
    this->m_padding_bottom = cell.PaddingBottom;
    this->AssignedArea = cell.AssignedArea;

    // Children locations are stored separately and assigned area is located here. No reason to update grid at all
}

void sad::layouts::Cell::update()
{
    // TODO: Update children location here
}

void sad::layouts::Cell::setWidth(const sad::layouts::LengthValue& width, bool upgrade_grid)
{
    m_width = width;
    tryNotify(upgrade_grid);
}

const sad::layouts::LengthValue& sad::layouts::Cell::width() const
{
    return m_width;
}

void sad::layouts::Cell::setHeight(const sad::layouts::LengthValue height, bool upgrade_grid)
{
    m_height = height;
    tryNotify(upgrade_grid);
}

const sad::layouts::LengthValue& sad::layouts::Cell::height() const
{
    return m_height;
}

void sad::layouts::Cell::setRowSpan(unsigned int rows, bool upgrade_grid)
{
    m_row_span = rows;
    if (m_row_span < 1)
    {
        m_row_span = 1;
    }
    tryNotify(upgrade_grid);
}

unsigned int sad::layouts::Cell::rowSpan() const
{
    return m_row_span;
}

void sad::layouts::Cell::setColSpan(unsigned int rows, bool upgrade_grid)
{
    m_col_span = rows;
    if (m_col_span < 1)
    {
        m_col_span = 1;
    }
    tryNotify(upgrade_grid);
}

unsigned int sad::layouts::Cell::colSpan() const
{
    return m_col_span;
}

void sad::layouts::Cell::setVerticalAlignment(sad::layouts::VerticalAlignment align, bool upgrade_grid)
{
    m_valign = align;
    tryNotify(upgrade_grid);
}

sad::layouts::VerticalAlignment sad::layouts::Cell::verticalAlignment() const
{
    return m_valign;
}

void sad::layouts::Cell::setHorizontalAlignment(sad::layouts::HorizontalAlignment align, bool upgrade_grid)
{
    m_halign = align; 
    tryNotify(upgrade_grid);
}

sad::layouts::HorizontalAlignment sad::layouts::Cell::horizontalAlignment() const
{
    return m_halign;
}

void sad::layouts::Cell::setStackingType(sad::layouts::StackingType type, bool upgrade_grid)
{
    m_stacking_type = type;
    tryNotify(upgrade_grid);
}

sad::layouts::StackingType sad::layouts::Cell::stackingType() const
{
    return m_stacking_type;
}

void sad::layouts::Cell::setPaddingTop(double value, bool update_grid)
{
    m_padding_top = value;
    tryNotify(update_grid);    
}

double sad::layouts::Cell::paddingTop() const
{
    return m_padding_top;
}

void sad::layouts::Cell::setPaddingBottom(double value, bool update_grid)
{
    m_padding_bottom = value;
    tryNotify(update_grid);  	
}

double sad::layouts::Cell::paddingBottom() const
{
    return m_padding_bottom;
}

void sad::layouts::Cell::setPaddingLeft(double value, bool update_grid)
{
    m_padding_left = value;
    tryNotify(update_grid);  	
}

double sad::layouts::Cell::paddingLeft() const
{
    return m_padding_left;
}

void sad::layouts::Cell::setPaddingRight(double value, bool update_grid)
{
    m_padding_right = value;
    tryNotify(update_grid);  			
}

double sad::layouts::Cell::paddingRight() const
{
    return m_padding_right;
}

void sad::layouts::Cell::setNodes(const sad::Vector<sad::SceneNode*>& nodes, bool update_grid)
{
    for(size_t i = 0; i < m_children.size(); i++)
    {
        delete m_children[i];
    }
    m_children.clear();
    for(size_t i = 0; i < nodes.size(); i++)
    {
        sad::db::TypedLink<sad::SceneNode>* node = new sad::db::TypedLink<sad::SceneNode>();
        node->setDatabase(m_db);
        node->setObject(nodes[i]);
        m_children << node;
    }
    tryNotify(update_grid);
}

sad::Vector<sad::SceneNode*> sad::layouts::Cell::nodes() const
{
    sad::Vector<sad::SceneNode*> result;
    for(size_t i = 0; i < m_children.size(); i++)
    {
        result << m_children[i]->value();
    }
    return result;
}

void sad::layouts::Cell::setMajorIds(const sad::Vector<unsigned long long>& nodes, bool update_grid)
{
    for(size_t i = 0; i < m_children.size(); i++)
    {
        delete m_children[i];
    }
    m_children.clear();
    for(size_t i = 0; i < nodes.size(); i++)
    {
        sad::db::TypedLink<sad::SceneNode>* node = new sad::db::TypedLink<sad::SceneNode>();
        node->setDatabase(m_db);
        node->setMajorId(nodes[i]);
        m_children << node;
    }
    tryNotify(update_grid);
}

sad::Vector<unsigned long long> sad::layouts::Cell::majorIds() const
{
    sad::Vector<unsigned long long> result;
    for(size_t i = 0; i < m_children.size(); i++)
    {
        result << m_children[i]->majorId();
    }
    return result;
}

sad::db::Database* sad::layouts::Cell::database() const
{
    return m_db;
}

void sad::layouts::Cell::setDatabase(sad::db::Database* db)
{
    m_db = db;
    for(size_t i = 0; i < m_children.size(); i++)
    {
        m_children[i]->setDatabase(db);
    }
}

void sad::layouts::Cell::moveBy(const sad::Point2D& p)
{
    for(size_t i = 0; i < 4; i++)
    {
        this->AssignedArea[4] += p;
    }
    for(size_t i = 0; i < m_children.size(); i++)
    {
        sad::SceneNode* node = m_children[i]->value();
        if (node)
        {
            node->moveBy(p);
        }
    }
}

void sad::layouts::Cell::tryNotify(bool update_grid)
{
    if (update_grid)
    {
        if (m_grid)
        {
            m_grid->update();
        }
    }
    else
    {
        update();
    }
}

// ============================ PRIVATE METHODS ============================

sad::layouts::Cell::Cell(const sad::layouts::Cell& o)
: Rendered(o.Rendered), 
m_row_span(o.m_row_span), 
m_col_span(o.m_col_span),
m_valign(o.m_valign),
m_halign(o.m_halign),
m_stacking_type(o.m_stacking_type),
m_padding_top(o.m_padding_top),
m_padding_bottom(o.m_padding_bottom),
m_padding_left(o.m_padding_left),
m_padding_right(o.m_padding_right),
m_grid(o.m_grid),
m_db(o.m_db)
{
    throw std::runtime_error("Not implemented");
}

sad::layouts::Cell& sad::layouts::Cell::operator=(const sad::layouts::Cell& o)
{
    throw std::runtime_error("Not implemented");
    // ReSharper disable once CppUnreachableCode
    return *this;
}

