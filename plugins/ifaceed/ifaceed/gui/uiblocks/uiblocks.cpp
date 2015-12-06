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


gui::uiblocks::UIBlocks::UIBlocks() : m_ui_scene_block(new gui::iblocks::UISceneBlock()),
m_ui_scenenode_block(new gui::iblocks::UISceneNodeBlock()),
m_ui_label_block(new gui::iblocks::UILabelBlock()),
m_ui_sprite_block(new gui::iblocks::UISpriteBlock()),
m_ui_customobject_block(new gui::iblocks::UICustomObjectBlock()),
m_ui_way_block(new gui::iblocks::UIWayBlock()),
m_ui_dialogue_block(new gui::iblocks::UIDialogueBlock()),
m_ui_phrase_block(new gui::iblocks::UIPhraseBlock()),
m_ui_animation_block(new gui::iblocks::UIAnimationBlock()),
m_ui_animationinstance_block(new gui::iblocks::UIAnimationInstanceBlock()),
m_ui_animationsgroup_block(new gui::iblocks::UIAnimationsGroupBlock()),
m_ui_common_block(new gui::iblocks::UICommonBlock()),
m_ui_console_block(new gui::iblocks::UIConsoleBlock())
{

}

void gui::uiblocks::UIBlocks::init(QWidget* w)
{
    m_ui_scene_block->init(w);
    m_ui_scenenode_block->init(w);
    m_ui_label_block->init(w);
    m_ui_sprite_block->init(w);
    m_ui_customobject_block->init(w);
    m_ui_way_block->init(w);
    m_ui_dialogue_block->init(w);
    m_ui_phrase_block->init(w);
    m_ui_animation_block->init(w);
    m_ui_animationinstance_block->init(w);
    m_ui_animationsgroup_block->init(w);
    m_ui_common_block->init(w);
    m_ui_console_block->init(w);
}

void gui::uiblocks::UIBlocks::~UIBlocks()
{
    delete m_ui_scene_block;
    delete m_ui_scenenode_block;
    delete m_ui_label_block;
    delete m_ui_sprite_block;
    delete m_ui_customobject_block;
    delete m_ui_way_block;
    delete m_ui_dialogue_block;
    delete m_ui_phrase_block;
    delete m_ui_animation_block;
    delete m_ui_animationinstance_block;
    delete m_ui_animationsgroup_block;
    delete m_ui_common_block;
    delete m_ui_console_block;
}
