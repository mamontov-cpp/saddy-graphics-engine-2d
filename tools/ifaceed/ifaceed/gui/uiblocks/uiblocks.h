/* \file uiblocks.h
    
    Contains data about all UIBlocks, used in the program
*/    
#pragma once

class QWidget;

namespace gui 
{

namespace uiblocks
{


class UISceneBlock;
class UISceneNodeBlock;
class UILabelBlock;
class UISpriteBlock;
class UICustomObjectBlock;
class UIWayBlock;
class UIDialogueBlock;
class UIPhraseBlock;
class UIAnimationBlock;
class UIAnimationInstanceBlock;
class UIAnimationsGroupBlock;
class UICommonBlock;
class UIConsoleBlock;
class UILayoutBlock;



/*! A list of all UIBlocks of editor
 */
class UIBlocks
{
public:
    /*! Constructs new UIBlocks
     */
    UIBlocks();
    /*! Inits blocks with children of main widget
        \param[in] w widget
     */
    void init(QWidget* w);
    /*! Destroys object
     */
    ~UIBlocks();
    /*! Returns UI group for Scene group of widgets
        \return UI group for Scene objects
    */
    inline gui::uiblocks::UISceneBlock* uiSceneBlock()
    {
        return m_ui_scene_block;
    }
    /*! Returns UI group for SceneNode group of widgets
        \return UI group for SceneNode objects
    */
    inline gui::uiblocks::UISceneNodeBlock* uiSceneNodeBlock()
    {
        return m_ui_scenenode_block;
    }
    /*! Returns UI group for Label group of widgets
        \return UI group for Label objects
    */
    inline gui::uiblocks::UILabelBlock* uiLabelBlock()
    {
        return m_ui_label_block;
    }
    /*! Returns UI group for Sprite group of widgets
        \return UI group for Sprite objects
    */
    inline gui::uiblocks::UISpriteBlock* uiSpriteBlock()
    {
        return m_ui_sprite_block;
    }
    /*! Returns UI group for CustomObject group of widgets
        \return UI group for CustomObject objects
    */
    inline gui::uiblocks::UICustomObjectBlock* uiCustomObjectBlock()
    {
        return m_ui_customobject_block;
    }
    /*! Returns UI group for Way group of widgets
        \return UI group for Way objects
    */
    inline gui::uiblocks::UIWayBlock* uiWayBlock()
    {
        return m_ui_way_block;
    }
    /*! Returns UI group for Dialogue group of widgets
        \return UI group for Dialogue objects
    */
    inline gui::uiblocks::UIDialogueBlock* uiDialogueBlock()
    {
        return m_ui_dialogue_block;
    }
    /*! Returns UI group for Phrase group of widgets
        \return UI group for Phrase objects
    */
    inline gui::uiblocks::UIPhraseBlock* uiPhraseBlock()
    {
        return m_ui_phrase_block;
    }
    /*! Returns UI group for Animation group of widgets
        \return UI group for Animation objects
    */
    inline gui::uiblocks::UIAnimationBlock* uiAnimationBlock()
    {
        return m_ui_animation_block;
    }
    /*! Returns UI group for AnimationInstance group of widgets
        \return UI group for AnimationInstance objects
    */
    inline gui::uiblocks::UIAnimationInstanceBlock* uiAnimationInstanceBlock()
    {
        return m_ui_animationinstance_block;
    }
    /*! Returns UI group for AnimationsGroup group of widgets
        \return UI group for AnimationsGroup objects
    */
    inline gui::uiblocks::UIAnimationsGroupBlock* uiAnimationsGroupBlock()
    {
        return m_ui_animationsgroup_block;
    }
    /*! Returns UI group for Common group of widgets
        \return UI group for Common objects
    */
    inline gui::uiblocks::UICommonBlock* uiCommonBlock()
    {
        return m_ui_common_block;
    }
    /*! Returns UI group for Console group of widgets
        \return UI group for Console objects
    */
    inline gui::uiblocks::UIConsoleBlock* uiConsoleBlock()
    {
        return m_ui_console_block;
    }
    /*! Returns UI group for Layout group of widgets
        \return UI group for Layout objects
    */
    inline gui::uiblocks::UILayoutBlock* uiLayoutBlock()
    {
        return m_ui_layout_block;
    }
protected:
    /*! An UI group for Scene group of widgets
    */
    gui::uiblocks::UISceneBlock* m_ui_scene_block;
    /*! An UI group for SceneNode group of widgets
    */
    gui::uiblocks::UISceneNodeBlock* m_ui_scenenode_block;
    /*! An UI group for Label group of widgets
    */
    gui::uiblocks::UILabelBlock* m_ui_label_block;
    /*! An UI group for Sprite group of widgets
    */
    gui::uiblocks::UISpriteBlock* m_ui_sprite_block;
    /*! An UI group for CustomObject group of widgets
    */
    gui::uiblocks::UICustomObjectBlock* m_ui_customobject_block;
    /*! An UI group for Way group of widgets
    */
    gui::uiblocks::UIWayBlock* m_ui_way_block;
    /*! An UI group for Dialogue group of widgets
    */
    gui::uiblocks::UIDialogueBlock* m_ui_dialogue_block;
    /*! An UI group for Phrase group of widgets
    */
    gui::uiblocks::UIPhraseBlock* m_ui_phrase_block;
    /*! An UI group for Animation group of widgets
    */
    gui::uiblocks::UIAnimationBlock* m_ui_animation_block;
    /*! An UI group for AnimationInstance group of widgets
    */
    gui::uiblocks::UIAnimationInstanceBlock* m_ui_animationinstance_block;
    /*! An UI group for AnimationsGroup group of widgets
    */
    gui::uiblocks::UIAnimationsGroupBlock* m_ui_animationsgroup_block;
    /*! An UI group for Common group of widgets
    */
    gui::uiblocks::UICommonBlock* m_ui_common_block;
    /*! An UI group for Console group of widgets
    */
    gui::uiblocks::UIConsoleBlock* m_ui_console_block;
    /*! An UI group for Layout group of widgets
    */
    gui::uiblocks::UILayoutBlock* m_ui_layout_block;
};

}

}
