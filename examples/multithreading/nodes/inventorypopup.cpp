#include "inventorypopup.h"

#include <geometry2d.h>

DECLARE_SOBJ_INHERITANCE(nodes::InventoryPopup, sad::SceneNode);

// An inner padding between label  and overlay
#define INNER_PADDING 14
// An outer padding label between underlay and overlay
#define OUTER_PADDING 2

// ========================================== PUBLIC METHODS ==========================================

nodes::InventoryPopup::InventoryPopup()
{
    m_background_underlay = new sad::Sprite2D();
    m_background_overlay = new sad::Sprite2D();
    m_label = new sad::Label();

    m_background_underlay->addRef();
    m_background_overlay->addRef();
    m_label->addRef();
}

nodes::InventoryPopup::~InventoryPopup()
{
    m_label->delRef();
    m_background_overlay->delRef();
    m_background_underlay->delRef();
}

void nodes::InventoryPopup::render()
{
    if (this->active() && this->visible())
    {
        m_background_underlay->render();
        m_background_overlay->render();
        m_label->render();
    }
}

void nodes::InventoryPopup::rendererChanged()
{
    m_background_underlay->rendererChanged();
    m_background_overlay->rendererChanged();
    m_label->rendererChanged();


    m_background_underlay->setTreeName("");
    m_background_underlay->set("white_square");
    m_background_underlay->setColor(sad::AColor(255, 222, 189));

    m_background_overlay->setTreeName("");
    m_background_overlay->set("white_square");
    m_background_overlay->setColor(sad::AColor(160, 80, 0));

    m_label->setTreeName("");
    m_label->setFontName("Liberation Sans");
    m_label->setSize(32);
    m_label->setMaximalLineWidth(20);
    m_label->setBreakText(sad::Label::LBT_BREAK_WORD);
    m_label->setColor(sad::AColor(255, 255, 255));
}

void nodes::InventoryPopup::setScene(sad::Scene* scene)
{
    m_background_underlay->setScene(scene);
    m_background_overlay->setScene(scene);
    m_label->setScene(scene);
}


void nodes::InventoryPopup::setDescription(const sad::String& name, const sad::String& description)
{
    sad::String str = name + "\n\n" + description;
    if (str != m_label->string())
    {
        m_label->setString(str);
        this->recomputePopupPosition();
    }
}

void nodes::InventoryPopup::showAt(const sad::Point2D& p)
{
    if (!sad::equal(p, m_pivot_point))
    {
        m_pivot_point = p;
        this->recomputePopupPosition();
        this->setVisible(true);
    }
}


// ========================================== PRIVATE METHODS ==========================================

void nodes::InventoryPopup::recomputePopupPosition()
{
    sad::Renderer* r = m_label->scene()->renderer();
    double screen_width = r->settings().width();

    const sad::Rect2D& rect_area = m_label->area();
    double width = rect_area.width();
    double height = rect_area.height();

    double outer_width  = width + (INNER_PADDING + OUTER_PADDING) * 2;
    double outer_height = height + (INNER_PADDING + OUTER_PADDING) * 2;

    double px = m_pivot_point.x();
    double py = m_pivot_point.y();

    if (px + outer_width <= screen_width)
    {
        if (py - outer_height >= 0)
        {
            // Consider pivot as top-left point
            setAreaForPopup(px, py, outer_width, outer_height);
        }
        else
        {
            // Consider pivot as bottom-left point
            setAreaForPopup(px, py + outer_height, outer_width, outer_height);
        }
    }
    else
    {
        if (py - outer_width >= 0)
        {
            // Consider as top-right point
            setAreaForPopup(px - outer_width, py, outer_width, outer_height);
        }
        else
        {
            // Consider as botom-right point
            setAreaForPopup(px - outer_width, py + outer_height, outer_width, outer_height);
        }
    }
}

void nodes::InventoryPopup::setAreaForPopup(double px,  double py, double width, double height)
{
    m_background_underlay->setArea(sad::Rect2D(px, py - height, px + width, py));
    m_background_overlay->setArea(sad::Rect2D(
        px + OUTER_PADDING,
        py - height + OUTER_PADDING,
        px + width - 2 * OUTER_PADDING,
        py - OUTER_PADDING
    ));
    m_label->setArea(sad::Rect2D(
        px + OUTER_PADDING + INNER_PADDING,
        py - height + (OUTER_PADDING + INNER_PADDING),
        px + width - 2 * (OUTER_PADDING + INNER_PADDING),
        py -  (OUTER_PADDING + INNER_PADDING)
    ));
}

nodes::InventoryPopup::InventoryPopup(const nodes::InventoryPopup&) : m_background_underlay(NULL), m_background_overlay(NULL), m_label(NULL) // NOLINT(bugprone-copy-constructor-init)
{
    throw std::logic_error("The object is non-copyable");
}

nodes::InventoryPopup& nodes::InventoryPopup::operator=(const nodes::InventoryPopup&)
{
    throw std::logic_error("The object is non-copyable");
    // ReSharper disable once CppUnreachableCode
    return *this;
}
