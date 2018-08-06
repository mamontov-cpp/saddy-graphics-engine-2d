#include "inventorynode.h"

#include "../game/inventory.h"

#include <stdexcept>


DECLARE_SOBJ_INHERITANCE(nodes::InventoryNode, sad::SceneNode);

#define PADDING 18

// ========================================== PUBLIC METHODS ==========================================

nodes::InventoryNode::InventoryNode(game::Inventory* inventory) : m_inventory(inventory)
{
    m_background = new sad::Sprite2D();
    m_label = new sad::Label();
    m_slot  = new sad::Sprite2D();


    m_label->addRef();
    m_background->addRef();
    m_slot->addRef();
}

nodes::InventoryNode::~InventoryNode()
{
    m_slot->delRef();
    m_background->delRef();
    m_label->delRef();
}

void nodes::InventoryNode::render()
{
    m_background->render();
    m_label->render();

    double topy = 600 - m_label->area().height();

    double width = m_slot->area().width();
    double height = m_slot->area().height();

    // Move lower to ensure that we don't intersect label
    topy -= height;

    double total_width = (width + PADDING) * game::Inventory::Width - PADDING;
    double startx = 400 - total_width / 2.0;

    for(int i = 0; i < game::Inventory::Height; i++)
    {
        for(int j = 0; j < game::Inventory::Width; j++)
        {
            double y = topy - (height + PADDING) * i;
            double x = startx + (width + PADDING) * j;
            m_slot->setArea(sad::Rect2D(x, y + height, x + width, y));
            m_slot->render();
        }
    }
}

void nodes::InventoryNode::rendererChanged()
{
    m_background->rendererChanged();
    m_label->rendererChanged();
    m_slot->rendererChanged();

    m_background->setTreeName("");
    m_background->set("white_square");
    m_background->setArea(sad::Rect2D(0, 0, 800, 600));
    m_background->setColor(sad::AColor(160, 80, 0));

    m_label->setTreeName("");
    m_label->setFontName("Arcade Classic");
    m_label->setSize(40);
    m_label->setString("I N V E N T O R Y");
    m_label->setColor(sad::AColor(255, 255, 255));

    sad::Rect2D area = m_label->area();
    // Upper top level
    m_label->setArea(sad::Rect2D(400 - area.width() / 2, 600, 400 + area.width() / 2, 600 - area.height()));

    m_slot->setTreeName("");
    m_slot->set("slots/slot");
}

void nodes::InventoryNode::setScene(sad::Scene* scene)
{
    m_background->setScene(scene);
    m_label->setScene(scene);
    m_slot->setScene(scene);
}


// ========================================== PRIVATE METHODS ==========================================  

nodes::InventoryNode::InventoryNode(const nodes::InventoryNode&) : m_background(NULL), m_label(NULL), m_slot(NULL), m_inventory(NULL) // NOLINT(bugprone-copy-constructor-init)
{
    throw std::logic_error("The object is non-copyable");
}

nodes::InventoryNode& nodes::InventoryNode::operator=(const nodes::InventoryNode&)
{
    throw std::logic_error("The object is non-copyable");
    // ReSharper disable once CppUnreachableCode
    return *this;
}