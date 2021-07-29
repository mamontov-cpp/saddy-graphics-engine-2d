#include "layouts/cell.h"
#include "layouts/grid.h"

#include <db/dbvariant.h>
#include <db/save.h>
#include <db/load.h>


#include <stdexcept>


// ============================ PUBLIC METHODS ============================

sad::layouts::Cell::Cell()
: Rendered(false), Row(0), Col(0),
m_row_span(1),
m_col_span(1),
m_valign(sad::layouts::VerticalAlignment::LVA_Middle),
m_halign(sad::layouts::HorizontalAlignment::LHA_Middle),
m_stacking_type(sad::layouts::StackingType::LST_Horizontal),
m_padding_top(0),
m_padding_bottom(0),
m_padding_left(0),
m_padding_right(0),
m_grid(nullptr),
m_db(nullptr)
{
    m_width.Unit = sad::layouts::Unit::LU_Auto;
    m_height.Unit = sad::layouts::Unit::LU_Auto;
}

sad::layouts::Cell::~Cell()
{

}

sad::db::Variant sad::layouts::Cell::getProperty(const sad::String& prop) const
{
    sad::db::Variant v;
    if (prop == "width")
    {
        v = sad::db::Variant(sad::layouts::LengthValue(this->width()));
    }
    if (prop == "height")
    {
        v = sad::db::Variant(sad::layouts::LengthValue(this->height()));
    }
    if (prop == "valign")
    {
        v = sad::db::Variant(this->verticalAlignment());
    }
    if (prop == "halign")
    {
        v = sad::db::Variant(this->horizontalAlignment());
    }
    if (prop == "stacking_type")
    {
        v = sad::db::Variant(this->stackingType());
    }
    if (prop == "padding_top")
    {
        v = sad::db::Variant(this->paddingTop());
    }
    if (prop == "padding_left")
    {
        v = sad::db::Variant(this->paddingLeft());
    }
    if (prop == "padding_right")
    {
        v = sad::db::Variant(this->paddingRight());
    }
    if (prop == "padding_bottom")
    {
        v = sad::db::Variant(this->paddingBottom());
    }
    return v;
}

void sad::layouts::Cell::setProperty(const sad::String& prop, const sad::db::Variant& v, bool update_grid)
{
    if (prop == "width")
    {
        sad::Maybe<sad::layouts::LengthValue> mv = v.get<sad::layouts::LengthValue>();
        if (mv.exists())
        {
            this->setWidth(mv.value(), update_grid);
        }
    }
    if (prop == "height")
    {
        sad::Maybe<sad::layouts::LengthValue> mv = v.get<sad::layouts::LengthValue>();
        if (mv.exists())
        {
            this->setHeight(mv.value(), update_grid);
        }
    }
    if (prop == "valign")
    {
        sad::Maybe<sad::layouts::VerticalAlignment> mv = v.get<sad::layouts::VerticalAlignment>();
        if (mv.exists())
        {
            this->setVerticalAlignment(mv.value(), update_grid);
        }
    }
    if (prop == "halign")
    {
        sad::Maybe<sad::layouts::HorizontalAlignment> mv = v.get<sad::layouts::HorizontalAlignment>();
        if (mv.exists())
        {
            this->setHorizontalAlignment(mv.value(), update_grid);
        }
    }
    if (prop == "stacking_type")
    {
        sad::Maybe<sad::layouts::StackingType> mv = v.get<sad::layouts::StackingType>();
        if (mv.exists())
        {
            this->setStackingType(mv.value(), update_grid);
        }
    }
    if (prop == "padding_top")
    {
        sad::Maybe<double> mv = v.get<double>();
        if (mv.exists())
        {
            this->setPaddingTop(mv.value(), update_grid);
        }
    }
    if (prop == "padding_left")
    {
        sad::Maybe<double> mv = v.get<double>();
        if (mv.exists())
        {
            this->setPaddingLeft(mv.value(), update_grid);
        }
    }
    if (prop == "padding_right")
    {
        sad::Maybe<double> mv = v.get<double>();
        if (mv.exists())
        {
            this->setPaddingRight(mv.value(), update_grid);
        }
    }
    if (prop == "padding_bottom")
    {
        sad::Maybe<double> mv = v.get<double>();
        if (mv.exists())
        {
            this->setPaddingBottom(mv.value(), update_grid);
        }
    }
}

void sad::layouts::Cell::setGrid(sad::layouts::Grid* g)
{
        m_grid = g;
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
    cell.Row = Row;
    cell.Col = Col;
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
    this->Row = cell.Row;
    this->Col = cell.Col;

    // Children locations are stored separately and assigned area is located here. No reason to update grid at all
}

void sad::layouts::Cell::update()
{
    // Compute a normalized area
    sad::layouts::Cell::NormalizedRectangle normalized = this->normalize(this->AssignedArea);
    sad::Point2D& minpoint = normalized._1();
    sad::Point2D& maxpoint = normalized._2();

    // Get rid of annoying paddings
    double assignedwidth =  (maxpoint.x() - minpoint.x());
    double assignedheight = (maxpoint.y() - minpoint.y());

    // Compute normalized children sizes
    this->computeNormalizedChildrenSizes();

    // Compute preferred size, which we can relate to, when layouting data
    // Normalized rectangles are already computed, so we could futher use them
    sad::Size2D size = this->preferredSize();
    size.Width -= this->paddingLeft() + this->paddingRight();
    size.Height -= this->paddingTop() + this->paddingBottom();
    switch(this->stackingType())
    {
	case sad::layouts::StackingType::LST_Horizontal: this->applyHorizontalAlignment(minpoint, maxpoint, assignedheight, size); break;
    case sad::layouts::StackingType::LST_Vertical  : this->applyVerticalAlignment(minpoint, maxpoint, assignedwidth, size); break;
    case sad::layouts::StackingType::LST_NoStacking: this->applyNoStackingAlinment(minpoint, maxpoint, assignedwidth, assignedheight, size); break;
    default: this->applyNoStackingAlinment(minpoint, maxpoint, assignedwidth, assignedheight, size); 
    };
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

unsigned int sad::layouts::Cell::rowSpan() const
{
    return m_row_span;
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

void sad::layouts::Cell::setChildren(const sad::Vector<sad::SceneNode*>& nodes, bool update_grid)
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

sad::Vector<sad::SceneNode*> sad::layouts::Cell::children() const
{
    sad::Vector<sad::SceneNode*> result;
    for(size_t i = 0; i < m_children.size(); i++)
    {
        result << m_children[i]->value();
    }
    return result;
}

void sad::layouts::Cell::setChildren(const sad::Vector<unsigned long long>& nodes, bool update_grid)
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

sad::Vector<unsigned long long> sad::layouts::Cell::childrenMajorIds() const
{
    sad::Vector<unsigned long long> result;
    for(size_t i = 0; i < m_children.size(); i++)
    {
        result << m_children[i]->majorId();
    }
    return result;
}

void sad::layouts::Cell::addChild(sad::SceneNode* node, bool update_grid)
{
    sad::db::TypedLink<sad::SceneNode>* link = new sad::db::TypedLink<sad::SceneNode>();
    link->setDatabase(m_db);
    link->setObject(node);
    m_children << link;
    tryNotify(update_grid);
}

void sad::layouts::Cell::addChild(unsigned long long major_id, bool update_grid)
{
    sad::db::TypedLink<sad::SceneNode>* link = new sad::db::TypedLink<sad::SceneNode>();
    link->setDatabase(m_db);
    link->setMajorId(major_id);
    m_children << link;
    tryNotify(update_grid);
}

void sad::layouts::Cell::insertChild(unsigned int pos, sad::SceneNode* node, bool update_grid)
{
    sad::db::TypedLink<sad::SceneNode>* link = new sad::db::TypedLink<sad::SceneNode>();
    link->setDatabase(m_db);
    link->setObject(node);
    if (pos >= m_children.size())
    {
        m_children << link;
    }
    else
    {
        m_children.insert(link, pos);
    }
    tryNotify(update_grid);
}

void sad::layouts::Cell::insertChild(unsigned int pos, unsigned long long major_id, bool update_grid)
{
    sad::db::TypedLink<sad::SceneNode>* link = new sad::db::TypedLink<sad::SceneNode>();
    link->setDatabase(m_db);
    link->setMajorId(major_id);
    if (pos >= m_children.size())
    {
        m_children << link;
    }
    else
    {
        m_children.insert(link, pos);
    }
    tryNotify(update_grid);
}

void sad::layouts::Cell::removeChild(unsigned int pos, bool update_grid)
{
    if (pos < m_children.size())
    {
        delete m_children[pos];
        m_children.removeAt(pos);
        tryNotify(update_grid);
    }
}

void sad::layouts::Cell::clear(bool update_grid)
{
    for(size_t i = 0; i < m_children.size(); i++)
    {
        delete m_children[i];
    }
    m_children.clear();
    tryNotify(update_grid);    
}

void sad::layouts::Cell::swapChildren(unsigned int pos1, unsigned int pos2, bool update_grid)
{
    if (pos1 < m_children.size() && pos2 < m_children.size())
    {
        sad::db::TypedLink<sad::SceneNode>* node = m_children[pos1];
        m_children[pos1] = m_children[pos2];
        m_children[pos2] = node;
        tryNotify(update_grid);
    }
}

size_t sad::layouts::Cell::childrenCount() const
{
    return m_children.size();
}

sad::SceneNode* sad::layouts::Cell::child(unsigned int pos) const
{
    sad::SceneNode* result = nullptr;
    if (pos < m_children.size())
    {
        result = m_children[pos]->value();
    }
    return result;
}

unsigned long long sad::layouts::Cell::childMajorId(unsigned int pos) const
{
    unsigned long long result = 0;
    if (pos < m_children.size())
    {
        result = m_children[pos]->majorId();
    }
    return result;
}

sad::Maybe<size_t> sad::layouts::Cell::find(sad::SceneNode* node) const
{
    sad::Maybe<size_t> result;
    for(size_t i = 0; i < m_children.size(); i++)
    {
        if (m_children[i]->value() == node)
        {
            result.setValue(i);
        }
    }
    return result;
}

sad::Maybe<size_t> sad::layouts::Cell::find(unsigned long long major_id) const
{
    sad::Maybe<size_t> result;
    for(size_t i = 0; i < m_children.size(); i++)
    {
        if (m_children[i]->majorId() == major_id)
        {
            result.setValue(i);
        }
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
        this->AssignedArea[i] += p;
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

sad::Size2D sad::layouts::Cell::preferredSize() const
{
    sad::Size2D result(
        this->paddingLeft() + this->paddingRight(),
        this->paddingTop() + this->paddingBottom()
    );
    const_cast<sad::layouts::Cell*>(this)->computeNormalizedChildrenSizes();
    const sad::Vector<sad::layouts::Cell::NormalizedRectangle>& rects = m_normalized_children;
    if (rects.size() == 0)
    {
        // Ignore padding if no children
        result.Width = 0;
        result.Height = 0;
    }
    double height = 0;
    double width = 0;
    if (this->stackingType() == sad::layouts::StackingType::LST_Horizontal)
    {
        for(size_t i = 0; i < rects.size(); i++)
        {
            width += (rects[i].p2().x() - rects[i].p1().x());
            height = std::max(height, (rects[i].p2().y() - rects[i].p1().y()));
        }
    }
    else
    {
        if (this->stackingType() == sad::layouts::StackingType::LST_Vertical) 
        {
            for(size_t i = 0; i < rects.size(); i++)
            {
                width = std::max(width, (rects[i].p2().x() - rects[i].p1().x()));
                height += (rects[i].p2().y() - rects[i].p1().y());
            }
        }
        else
        {
            for(size_t i = 0; i < rects.size(); i++)
            {
                width = std::max(width, (rects[i].p2().x() - rects[i].p1().x()));
                height = std::max(height, (rects[i].p2().y() - rects[i].p1().y()));
            }
        }
    }
    result.Width += width;
    result.Height += height;
    return result;
}

sad::Size2D sad::layouts::Cell::computedSize() const
{
    sad::Size2D result;
    if (m_width.Unit == sad::layouts::Unit::LU_Pixels)
    {
        result.Width = m_width.Value;
    }
    if (m_width.Unit == sad::layouts::Unit::LU_Percents)
    {
        if (m_grid)
        {
            if (m_grid->fixedWidth())
            {
                result.Width = m_width.Value / 100.0 * m_grid->area().width();
            }
        }
    }


    if (m_height.Unit == sad::layouts::Unit::LU_Pixels)
    {
        result.Height = m_height.Value;
    }
    if (m_height.Unit == sad::layouts::Unit::LU_Percents)
    {
        if (m_grid)
        {
            if (m_grid->fixedHeight())
            {
                result.Height = m_height.Value / 100.0 * m_grid->area().height();
            }
        }
    }

    return result;
}

sad::Size2D sad::layouts::Cell::maximalSize() const
{
    sad::Size2D result = this->computedSize();
    sad::Size2D clientsize = this->preferredSize();
    result.Width = std::max(result.Width, clientsize.Width);
    result.Height = std::max(result.Height, clientsize.Height);
    return result;
}

sad::layouts::Grid* sad::layouts::Cell::grid() const
{
    return m_grid;
}

// ========================================= PROTECTED METHODS =========================================

void sad::layouts::Cell::setRowSpan(unsigned int rows)
{
    m_row_span = rows;
    if (m_row_span < 1)
    {
        m_row_span = 1;
    }
}

void sad::layouts::Cell::setColSpan(unsigned int cols)
{
    m_col_span = cols;
    if (m_col_span < 1)
    {
        m_col_span = 1;
    }
}

void sad::layouts::Cell::computeNormalizedChildrenSizes()
{
    m_normalized_children.clear();
    sad::Vector<sad::Rect2D> regions;
    for(size_t i = 0; i < m_children.size(); i++)
    {
        sad::SceneNode* node = m_children[i]->value();
        if (node)
        {
            regions.clear();
            node->regions(regions);
            if (regions.size() > 0)
            {
                m_normalized_children << normalize(regions[0]);
            }
        }
    }
}

sad::layouts::Cell::NormalizedRectangle sad::layouts::Cell::normalize(const sad::Rect2D& r)
{
    sad::Pair<sad::Point2D, sad::Point2D> result(r.p0(), r.p0());
    normalize(result, r.p1());
    normalize(result, r.p2());
    normalize(result, r.p3());
    return result;
}

void sad::layouts::Cell::normalize(sad::layouts::Cell::NormalizedRectangle& result, const sad::Point2D& p)
{
    if (p.x() < result._1().x())
    {
        result._1().setX(p.x());
    }

    if (p.y() < result._1().y())
    {
        result._1().setY(p.y());
    }


    if (p.x() > result._2().x())
    {
        result._2().setX(p.x());
    }

    if (p.y() > result._2().y())
    {
        result._2().setY(p.y());
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

void sad::layouts::Cell::applyHorizontalAlignment(const sad::Point2D& minpoint,const sad::Point2D& maxpoint, double assignedheight, const sad::Size2D& size)
{
    double factor = 1.0;
    // Factoring is disabled - we allow cell to be overflowed
    /*
    if (size.Width > assignedwidth)
    {
        factor = assignedwidth / size.Width;
    }
    */
    // Compute starting position
    double startingpointx = 0;
    switch(this->horizontalAlignment())
    {
	case sad::layouts::HorizontalAlignment::LHA_Left:
        startingpointx = minpoint.x() + this->paddingLeft();
        break;
    case sad::layouts::HorizontalAlignment::LHA_Middle:
        startingpointx = this->paddingLeft() + (minpoint.x() + maxpoint.x() - this->paddingLeft() - this->paddingRight() - size.Width * factor) / 2;
        break;
    case sad::layouts::HorizontalAlignment::LHA_Right:
        startingpointx = maxpoint.x() - size.Width * factor - this->paddingRight();
        break;
    };
    size_t current_rectangle = 0;
    for(size_t i = 0; i < this->m_children.size(); i++)
    {
        sad::SceneNode* node = m_children[i]->value();
        if (node)
        {
            const sad::layouts::Cell::NormalizedRectangle& childrect = m_normalized_children[current_rectangle];
            double childheight = childrect.p2().y() - childrect.p1().y();
            double childwidth = childrect.p2().x() - childrect.p1().x();
            double posy = maxpoint.y() - this->paddingTop();
            if (this->verticalAlignment() == sad::layouts::VerticalAlignment::LVA_Bottom)
            {
                posy = minpoint.y() + this->paddingBottom() + childheight;
            }
            if (this->verticalAlignment() == sad::layouts::VerticalAlignment::LVA_Middle)
            {
                posy = minpoint.y() + this->paddingBottom() + (assignedheight - this->paddingTop() - this->paddingBottom() + childheight) / 2.0;
            }
            node->moveBy(sad::Point2D(startingpointx - childrect.p1().x(), posy - childrect.p2().y()));

            startingpointx += childwidth * factor;
            ++current_rectangle;
        }
    }
}

void sad::layouts::Cell::applyVerticalAlignment(const sad::Point2D& minpoint,const sad::Point2D& maxpoint, double assignedwidth,const sad::Size2D& size)
{
    double factor = 1.0;
    // Factoring is disabled - we allow cell to be overflowed
    /*
    if (size.Height > assignedheight)
    {
        factor = assignedheight / size.Height;
    }
    */
    // Compute starting position
    double startingpointy = 0;
    switch(this->verticalAlignment())
    {
        case sad::layouts::VerticalAlignment::LVA_Top:
            startingpointy = maxpoint.y() - this->paddingTop();
            break;
        case sad::layouts::VerticalAlignment::LVA_Middle:
            startingpointy = (minpoint.y() + maxpoint.y() - this->paddingBottom() - this->paddingTop() + size.Height * factor) / 2 + this->paddingBottom();
            break;
        case sad::layouts::VerticalAlignment::LVA_Bottom:
            startingpointy = minpoint.y() + size.Height * factor + this->paddingBottom();
            break;
    };
    size_t current_rectangle = 0;
    for(size_t i = 0; i < this->m_children.size(); i++)
    {
        sad::SceneNode* node = m_children[i]->value();
        if (node)
        {
            const sad::layouts::Cell::NormalizedRectangle& childrect = m_normalized_children[current_rectangle];
            double childheight = childrect.p2().y() - childrect.p1().y();
            double childwidth = childrect.p2().x() - childrect.p1().x();
            double posx = minpoint.x() + this->paddingLeft();
            if (this->horizontalAlignment() == sad::layouts::HorizontalAlignment::LHA_Right)
            {
                posx = maxpoint.x() - childwidth - this->paddingRight();
            }
            if (this->horizontalAlignment() == sad::layouts::HorizontalAlignment::LHA_Middle)
            {
                posx = minpoint.x() + this->paddingLeft() + (assignedwidth  - this->paddingLeft() - this->paddingRight()  - childwidth) / 2.0;
            }
            node->moveBy(sad::Point2D(posx - childrect.p1().x(), startingpointy - childrect.p2().y()));

            startingpointy -= childheight * factor;
            ++current_rectangle;
        }
    }
}

void sad::layouts::Cell::applyNoStackingAlinment(const sad::Point2D& minpoint, const sad::Point2D& maxpoint, double assignedwidth, double assignedheight,  const sad::Size2D& size)
{
    size_t current_rectangle = 0;
    double padx = this->paddingLeft() + this->paddingRight();
    double pady = this->paddingTop() + this->paddingBottom();
    for(size_t i = 0; i < this->m_children.size(); i++)
    {
        sad::SceneNode* node = m_children[i]->value();
        if (node)
        {
            const sad::layouts::Cell::NormalizedRectangle& childrect = m_normalized_children[current_rectangle];
            double childheight = childrect.p2().y() - childrect.p1().y();
            double childwidth = childrect.p2().x() - childrect.p1().x();
            double posx = minpoint.x() + this->paddingLeft();
            if (this->horizontalAlignment() == sad::layouts::HorizontalAlignment::LHA_Right)
            {
                posx = maxpoint.x() - childwidth - this->paddingRight();
            }
            if (this->horizontalAlignment() == sad::layouts::HorizontalAlignment::LHA_Middle)
            {
                posx = minpoint.x() + this->paddingLeft() + (assignedwidth - padx - childwidth) / 2.0;
            }

            double posy = maxpoint.y() - this->paddingTop();
            if (this->verticalAlignment() == sad::layouts::VerticalAlignment::LVA_Bottom)
            {
                posy = minpoint.y() + childheight + this->paddingBottom();
            }
            if (this->verticalAlignment() == sad::layouts::VerticalAlignment::LVA_Middle)
            {
                posy = minpoint.y() + this->paddingBottom() + (assignedheight - pady + childheight) / 2.0;
            }
            node->moveBy(sad::Point2D(posx - childrect.p1().x(), posy - childrect.p2().y()));

            ++current_rectangle;
        }
    }
}

// ============================ PRIVATE METHODS ============================

sad::layouts::Cell::Cell(const sad::layouts::Cell& o)
:   Rendered(o.Rendered),
    Row(0),
    Col(0),
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

