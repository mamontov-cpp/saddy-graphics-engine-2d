#include "inventorynode.h"

#include "../game/inventory.h"

#include <stdexcept>


DECLARE_SOBJ_INHERITANCE(nodes::InventoryNode, sad::SceneNode);
// A padding between icons
#define PADDING 18
// An icon size
#define ICON_SIZE (48)

// ========================================== PUBLIC METHODS ==========================================

nodes::InventoryNode::InventoryNode(game::Inventory* inventory) : m_inventory(inventory)
{
    m_background = new sad::Sprite2D();
    m_label = new sad::Label();
    m_slot  = new sad::Sprite2D();
    m_basket_item = new sad::Sprite2D();


    m_label->addRef();
    m_background->addRef();
    m_slot->addRef();
    m_basket_item->addRef();
}

nodes::InventoryNode::~InventoryNode()
{
    m_basket_item->delRef();
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

    double bin_y = topy - (height + PADDING) * (game::Inventory::Height + 1);
    double bin_x = startx +  (width + PADDING) * (game::Inventory::Width - 1);
    m_slot->setArea(sad::Rect2D(bin_x, bin_y + height, bin_x + width, bin_y));
    m_slot->render();

    bin_y += height / 2;
    bin_x += width / 2;

    double halfwidth = ICON_SIZE / 2;
    double halfheight = ICON_SIZE / 2;
    m_basket_item->setArea(sad::Rect2D(bin_x - halfwidth, bin_y - halfheight, bin_x + halfwidth, bin_y + halfheight));
    m_basket_item->render();
}

void nodes::InventoryNode::rendererChanged()
{
    m_background->rendererChanged();
    m_label->rendererChanged();
    m_slot->rendererChanged();
    m_basket_item->rendererChanged();

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

    m_basket_item->setTreeName("");
    m_basket_item->set("icons_list/E_Wood04ng");
}

void nodes::InventoryNode::setScene(sad::Scene* scene)
{
    m_background->setScene(scene);
    m_label->setScene(scene);
    m_slot->setScene(scene);
    m_basket_item->setScene(scene);
}

void nodes::InventoryNode::clearInventorySprites()
{
    // TODO:
}

void nodes::InventoryNode::tryMakeSpriteAndStore(int i, int j, game::Item* item)
{
    // TODO:
}


void nodes::InventoryNode::swapSpritePositions(int row1, int column1, int row2, int column2)
{
    // TODO:
}

void nodes::InventoryNode::eraseSprite(int i, int j)
{
    // TODO:
}


// ========================================== PRIVATE METHODS ==========================================

nodes::InventoryNode::InventoryNode(const nodes::InventoryNode&) : m_inventory(NULL), m_background(NULL), m_label(NULL), m_slot(NULL), m_basket_item(NULL)  // NOLINT(bugprone-copy-constructor-init)
{
    throw std::logic_error("The object is non-copyable");
}

nodes::InventoryNode& nodes::InventoryNode::operator=(const nodes::InventoryNode&)
{
    throw std::logic_error("The object is non-copyable");
    // ReSharper disable once CppUnreachableCode
    return *this;
}
