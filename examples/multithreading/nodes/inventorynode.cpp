#include "inventorynode.h"
#include "inventorypopup.h"

#include "../game/inventory.h"

#include <stdexcept>

#include <pipeline/pipeline.h>


DECLARE_SOBJ_INHERITANCE(nodes::InventoryNode, sad::SceneNode)
// A padding between icons
#define PADDING 18
// A resize padding, to include user error, when dragging items to make it easier for user to  drag items
#define ERROR_AREA_PADDING 24

const double nodes::InventoryNode::IconSize = 48;

// ========================================== PUBLIC METHODS ==========================================

nodes::InventoryNode::InventoryNode(game::Inventory* inventory) : m_inventory(inventory),
m_computed_inventory_slots_places(false)
{
    m_background = new sad::Sprite2D();
    m_label = new sad::Label();
    m_slot  = new sad::Sprite2D();
    m_basket_item = new sad::Sprite2D();
    m_popup = new nodes::InventoryPopup();
    m_popup->setVisible(false);

    m_inventory->setNode(this);
    m_inventory->eachExisting([](int row, int column, game::Item* item) {
        item->setSprite(new sad::Sprite2D());
        item->sprite()->set(item->icon());
    });

    m_label->addRef();
    m_background->addRef();
    m_slot->addRef();
    m_basket_item->addRef();
    m_popup->addRef();
}

nodes::InventoryNode::~InventoryNode()
{
    m_popup->delRef();
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

    double halfwidth = nodes::InventoryNode::IconSize / 2;
    double halfheight = nodes::InventoryNode::IconSize / 2;

    for(int i = 0; i < game::Inventory::Height; i++)
    {
        for(int j = 0; j < game::Inventory::Width; j++)
        {
            double y = topy - (height + PADDING) * i;
            double x = startx + (width + PADDING) * j;
            m_slot->setArea(sad::Rect2D(x, y + height, x + width, y));
            m_slot->render();
            // If still not placed icons on slots, do it now
            if (!m_computed_inventory_slots_places)
            {
                game::Item* item = m_inventory->item(i, j);
                if (item)
                {
                    if (item->sprite())
                    {
                        x += width / 2;
                        y += height / 2;

                        item->sprite()->setArea(sad::Rect2D(x - halfwidth, y - halfheight, x + halfwidth, y + halfheight));
                    }
                }
            }
        }
    }

    
    double bin_y = topy - (height + PADDING) * (game::Inventory::Height + 1);
    double bin_x = startx +  (width + PADDING) * (game::Inventory::Width - 1);
    m_slot->setArea(sad::Rect2D(bin_x, bin_y + height, bin_x + width, bin_y));
    m_slot->render();

    if (!m_computed_inventory_slots_places)
    {
        m_inventory->setBasketArea(m_slot->area());
    }
    m_computed_inventory_slots_places = true;

    bin_y += height / 2;
    bin_x += width / 2;

    m_basket_item->setArea(sad::Rect2D(bin_x - halfwidth, bin_y - halfheight, bin_x + halfwidth, bin_y + halfheight));
    m_basket_item->render();
}

void nodes::InventoryNode::rendererChanged()
{
    m_background->rendererChanged();
    m_label->rendererChanged();
    m_slot->rendererChanged();
    m_basket_item->rendererChanged();
    m_popup->rendererChanged();

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

    // Update settings for inventory nodes
    m_inventory->eachExisting([](int row, int column, game::Item* item) {
        if (item->sprite())
        {
            item->sprite()->rendererChanged();
            item->sprite()->setTreeName("");
        }
    });
}

void nodes::InventoryNode::setScene(sad::Scene* scene)
{
    this->sad::SceneNode::setScene(scene);

    m_background->setScene(scene);
    m_label->setScene(scene);
    m_slot->setScene(scene);
    m_basket_item->setScene(scene);
    m_popup->setScene(scene);

    // Update settings for inventory nodes
    m_inventory->eachExisting([scene](int row, int column, game::Item* item) {
        if (item->sprite())
        {
            item->sprite()->setScene(scene);
            scene->add(item->sprite());
        }
    });
    if (scene->renderer())
    {
        scene->renderer()->pipeline()->appendTask([this, scene]() {
            scene->setLayer(this, 0);
        });
    }
}

void nodes::InventoryNode::clearInventorySprites() const
{
    m_inventory->eachExisting([](int row, int column, game::Item* item) {
        sad::Sprite2D* sprite = item->sprite();
        if (sprite)
        {
            sad::Scene* scene = sprite->scene();
            if (scene)
            {
                scene->remove(sprite);
            }
        }
    });
}

void nodes::InventoryNode::tryMakeSpriteAndStore(int i, int j, game::Item* item)
{
    if (item)
    { 
        this->scene()->renderer()->pipeline()->appendTask([=]() {
            if (!item->sprite())
            { 
                sad::Sprite2D* sprite = new sad::Sprite2D();
                this->scene()->add(sprite);
                sprite->setScene(this->scene());
                sprite->rendererChanged();
                sprite->setTreeName("");
                sprite->set(item->icon());
                sprite->setArea(this->getInventoryIconPosition(i, j));
                item->setSprite(sprite);
            }
            else
            {
                item->sprite()->setArea(this->getInventoryIconPosition(i, j));
            }
        });
    }
}

void nodes::InventoryNode::eraseSprite(int i, int j)
{
    game::Item* item = m_inventory->item(i, j);
    if (item)
    {
        sad::Sprite2D* sprite = item->sprite();
        if (sprite)
        {
            this->scene()->renderer()->pipeline()->appendTask([=]() {
                sprite->scene()->remove(sprite);
            });
        }
    }
}


sad::Rect2D nodes::InventoryNode::getInventoryIconPosition(int row, int column) const 
{
    return this->getPositionInSlot(row, column, nodes::InventoryNode::IconSize, nodes::InventoryNode::IconSize);
}

sad::Rect2D nodes::InventoryNode::getInventorySlotPosition(int row, int column) const
{
    double width = m_slot->area().width() + ERROR_AREA_PADDING;
    double height = m_slot->area().height() + ERROR_AREA_PADDING;
    return this->getPositionInSlot(row, column, width, height);
}

nodes::InventoryPopup* nodes::InventoryNode::popup() const
{
    return m_popup;
}

// ========================================== PRIVATE METHODS ==========================================

sad::Rect2D nodes::InventoryNode::getPositionInSlot(int row, int column, double input_width, double input_height) const
{
    double topy = 600 - m_label->area().height();

    double width = m_slot->area().width();
    double height = m_slot->area().height();

    // Move lower to ensure that we don't intersect label
    topy -= height;

    double total_width = (width + PADDING) * game::Inventory::Width - PADDING;
    double startx = 400 - total_width / 2.0;

    double y = topy - (height + PADDING) * row;
    double x = startx + (width + PADDING) * column;

    x += width / 2.0;
    y += height / 2.0;

    double halfwidth = input_width / 2.0;
    double halfheight = input_height / 2.0;


    return { x - halfwidth, y - halfheight, x + halfwidth, y + halfheight };
}

nodes::InventoryNode::InventoryNode(const nodes::InventoryNode&) 
: m_inventory(NULL), 
m_background(NULL), 
m_label(NULL),
m_slot(NULL), 
m_basket_item(NULL), 
m_popup(NULL),
m_computed_inventory_slots_places(false)
// NOLINT(bugprone-copy-constructor-init)
{
    throw std::logic_error("The object is non-copyable");
}

nodes::InventoryNode& nodes::InventoryNode::operator=(const nodes::InventoryNode&)
{
    throw std::logic_error("The object is non-copyable");
    // ReSharper disable once CppUnreachableCode
    return *this;
}
