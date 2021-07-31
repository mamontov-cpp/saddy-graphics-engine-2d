#include "uiblocks.h"
#include "uisceneblock.h"
#include "uiscenenodeblock.h"
#include "uilabelblock.h"
#include "uispriteblock.h"
#include "uicustomobjectblock.h"
#include "uiwayblock.h"
#include "uidialogueblock.h"
#include "uiphraseblock.h"
#include "uianimationblock.h"
#include "uianimationinstanceblock.h"
#include "uianimationsgroupblock.h"
#include "uicommonblock.h"
#include "uiconsoleblock.h"
#include "uilayoutblock.h"


gui::uiblocks::UIBlocks::UIBlocks() : m_ui_scene_block(new gui::uiblocks::UISceneBlock()),
m_ui_scene_node_block(new gui::uiblocks::UISceneNodeBlock()),
m_ui_label_block(new gui::uiblocks::UILabelBlock()),
m_ui_sprite_block(new gui::uiblocks::UISpriteBlock()),
m_ui_customobject_block(new gui::uiblocks::UICustomObjectBlock()),
m_ui_way_block(new gui::uiblocks::UIWayBlock()),
m_ui_dialogue_block(new gui::uiblocks::UIDialogueBlock()),
m_ui_phrase_block(new gui::uiblocks::UIPhraseBlock()),
m_ui_animation_block(new gui::uiblocks::UIAnimationBlock()),
m_ui_animation_instance_block(new gui::uiblocks::UIAnimationInstanceBlock()),
m_ui_animations_group_block(new gui::uiblocks::UIAnimationsGroupBlock()),
m_ui_common_block(new gui::uiblocks::UICommonBlock()),
m_ui_console_block(new gui::uiblocks::UIConsoleBlock()),
m_ui_layout_block(new gui::uiblocks::UILayoutBlock())
{

}

void gui::uiblocks::UIBlocks::init(QWidget* w) const
{
    m_ui_scene_block->init(w);
    m_ui_scene_node_block->init(w);
    m_ui_label_block->init(w);
    m_ui_sprite_block->init(w);
    m_ui_customobject_block->init(w);
    m_ui_way_block->init(w);
    m_ui_dialogue_block->init(w);
    m_ui_phrase_block->init(w);
    m_ui_animation_block->init(w);
    m_ui_animation_instance_block->init(w);
    m_ui_animations_group_block->init(w);
    m_ui_common_block->init(w);
    m_ui_console_block->init(w);
    m_ui_layout_block->init(w);
}

gui::uiblocks::UIBlocks::~UIBlocks()
{
    delete m_ui_scene_block;
    delete m_ui_scene_node_block;
    delete m_ui_label_block;
    delete m_ui_sprite_block;
    delete m_ui_customobject_block;
    delete m_ui_way_block;
    delete m_ui_dialogue_block;
    delete m_ui_phrase_block;
    delete m_ui_animation_block;
    delete m_ui_animation_instance_block;
    delete m_ui_animations_group_block;
    delete m_ui_common_block;
    delete m_ui_console_block;
    delete m_ui_layout_block;
}
