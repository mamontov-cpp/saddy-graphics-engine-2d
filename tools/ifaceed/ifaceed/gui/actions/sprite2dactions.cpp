#include <QRadioButton>
#include <QLineEdit>

#include "sprite2dactions.h"

#include <db/dbdatabase.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../blockedclosuremethodcall.h"

#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../core/typeconverters/qcolortosadacolor.h"

#include "../../history/scenenodes/scenenodesnew.h"

#include "../../history/sprite2d/changeoptions.h"
#include "../../history/sprite2d/makebackground.h"
#include "../../history/sprite2d/changeflipx.h"
#include "../../history/sprite2d/changeflipy.h"

#include "../resourcetreewidget/resourcetreewidget.h"

#include "../qt-palette-color-picker/colorpicker.h"

#include "../anglewidget/anglewidget.h"

#include "../mainpanelproxy.h"

#include "../uiblocks/uiblocks.h"
#include "../uiblocks/uiscenenodeblock.h"
#include "../uiblocks/uispriteblock.h"


#include "actions.h"
#include "sceneactions.h"
#include "scenenodeactions.h"
#include "gridactions.h"

#include <sprite2d.h>

// ===============================  PUBLIC METHODS ===============================

gui::actions::Sprite2DActions::Sprite2DActions(QObject* parent) : QObject(parent), m_block_first_point_for_fast_mode(false)
{

}

gui::actions::Sprite2DActions::~Sprite2DActions()
{
    
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::Sprite2DActions::cancelAddSprite()
{
    core::Shared* s = this->m_editor->shared();
    sad::SceneNode* node = s->activeObject();
    sad::Renderer::ref()->lockRendering();
    s->setActiveObject(nullptr);
    node->scene()->remove(node);
    sad::Renderer::ref()->unlockRendering();
    
    sad::hfsm::Machine* m = m_editor->machine();
    m->enterState(m->previousState());
    if (m->isInState("adding") || (m->isInState("selected") && s->selectedObject() == nullptr))
    {
        m->enterState("idle");
    }
    else
    {
        m_editor->actions()->sceneNodeActions()->updateUIForSelectedSceneNode();
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::Sprite2DActions::moveCenterOfSprite(const sad::input::MouseMoveEvent & e)
{
    const sad::Point2D& p = e.pos2D();
    sad::SceneNode* node = m_editor->shared()->activeObject();
    if (node)
    {
        static_cast<sad::Sprite2D*>(node)->setMiddle(p);
        m_editor->actions()->sceneNodeActions()->updateRegionForNode();
        // Do it only in case of moving sprite
        if (m_editor->machine()->isInState("adding/sprite_diagonal") == false)
        {
            node->setVisible(m_editor->uiBlocks()->uiSceneNodeBlock()->cbSceneNodeVisible->isChecked());
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::Sprite2DActions::commitAdd(const sad::input::MousePressEvent& e)
{
    core::Shared* s = m_editor->shared();
    sad::SceneNode* node = s->activeObject();
    sad::Renderer::ref()->database("")->table("scenenodes")->add(node);
    history::Command* c = new history::scenenodes::New(node);
    s->setActiveObject(nullptr);
    s->setSelectedObject(node);
    m_editor->history()->add(c);
    c->commit(m_editor);
    m_editor->machine()->enterState("selected");

    m_editor->emitClosure(blocked_bind(
        m_editor->actions()->sceneNodeActions(),
        &gui::actions::SceneNodeActions::selectLastSceneNode
    ));

    if (m_editor->panelProxy()->isFastModeEnabled())
    {
        m_editor->incrementFastModeCounter();
        s->setTriggeredByFastMode(true);
        this->add();
        s->setTriggeredByFastMode(false);
        if (m_editor->uiBlocks()->uiSpriteBlock()->rbTwoClicksPlaces->isChecked())
        {
            m_block_first_point_for_fast_mode = true;
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::Sprite2DActions::placeFirstPointForSprite(const sad::input::MousePressEvent& e)
{
    if (m_block_first_point_for_fast_mode)
    {
        m_block_first_point_for_fast_mode = false;
        return;
    }
    const sad::Point2D& p = e.pos2D();
    core::Shared* s = m_editor->shared();
    sad::SceneNode* node = s->activeObject();
    sad::Sprite2D* sprite = static_cast<sad::Sprite2D*>(node);
    sprite->setArea(sad::Rect2D(p, p));
    sprite->setVisible(true);

    m_editor->shared()->toggleActiveBorder(true);

    m_editor->machine()->enterState("adding/sprite_diagonal/point_placed");
    m_editor->panelProxy()->highlightState("Click, where you want lower point to be placed");

    m_editor->actions()->sceneNodeActions()->updateRegionForNode();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::Sprite2DActions::moveLowerPointOfSprite(const sad::input::MouseMoveEvent & e)
{
    const sad::Point2D& p = e.pos2D();
    sad::SceneNode* node = m_editor->shared()->activeObject();
    if (node)
    {
        sad::Sprite2D* sprite = static_cast<sad::Sprite2D*>(node);
        sad::Rect2D area = sprite->area();
        area = sad::Rect2D(p, area[3]);
        sprite->setArea(area);
        m_editor->actions()->sceneNodeActions()->updateRegionForNode();
    }
}


// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::Sprite2DActions::setSceneNodeAsBackground(sad::SceneNode* node, bool from_editor)
{
    if (m_editor->actions()->gridActions()->isInGrid(node))
    {
        if (from_editor)
        {
            QMessageBox::critical(nullptr, "Node canot be set as background", "This node could not be set as background, because it\'s within grid.");
        }
        return;
    }
    const sad::Settings& settings = sad::Renderer::ref()->settings();
    sad::Rect2D newrect(
        sad::Point2D(0, 0),
        sad::Point2D(settings.width(), settings.height())
    );

    sad::Maybe<sad::Rect2D> oldrect = node->getProperty<sad::Rect2D>("area");
    sad::Maybe<float> oldangle = node->getProperty<float>("angle");
    sad::Maybe<unsigned int> oldlayer = node->getProperty<unsigned int>("layer");

    if (oldrect.exists() && oldangle.exists() && oldlayer.exists())
    {
        history::Command* c = new history::sprite2d::MakeBackground(
            node,
            oldrect.value(),
            oldangle.value(),
            oldlayer.value(),
            newrect,
            0,
            0
        );
        if (from_editor) 
        {
            m_editor->history()->add(c);
        }
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
        c->commit(m_editor);
    }
}

// ===============================  PUBLIC SLOTS METHODS ===============================

void gui::actions::Sprite2DActions::add()
{
    bool valid = true;

    gui::actions::SceneActions* s_actions = m_editor->actions()->sceneActions();

    gui::uiblocks::UISpriteBlock* sblk = m_editor->uiBlocks()->uiSpriteBlock();
    gui::uiblocks::UISceneNodeBlock* snblk = m_editor->uiBlocks()->uiSceneNodeBlock();


    valid = valid && s_actions->currentScene() != nullptr;
    valid = valid && sblk->rtwSpriteSprite->selectedResourceName().exists();
    valid = valid && m_editor->machine()->isInState("adding") == false;
    if (valid)
    {
        m_editor->emitClosure(blocked_bind(
            snblk->cbSceneNodeVisible,
            &QCheckBox::setCheckState,
            Qt::Checked
        ));
        
        m_editor->cleanupBeforeAdding();

        if (sblk->rbPlaceAndRotate->isChecked())
        {
            this->addBySimplePlacing();
        }
        else
        {
            this->addByDiagonalPlacing();
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::Sprite2DActions::addBySimplePlacing()
{
    gui::actions::SceneActions* s_actions = m_editor->actions()->sceneActions();

    gui::uiblocks::UISpriteBlock* sblk = m_editor->uiBlocks()->uiSpriteBlock();
    gui::uiblocks::UISceneNodeBlock* snblk = m_editor->uiBlocks()->uiSceneNodeBlock();

    sad::Sprite2D* sprite = new sad::Sprite2D();
    sprite->setTreeName("");
    sprite->set(sblk->rtwSpriteSprite->selectedResourceName().value());
    sprite->setAngle(snblk->awSceneNodeAngle->value());
    core::typeconverters::QColorToSadAColor c;
    sad::AColor clr;
    c.convert(snblk->clpSceneNodeColor->selectedColor(), clr);
    sprite->setColor(clr);
    const sad::Settings & settings = sad::Renderer::ref()->settings();
    sprite->setMiddle(sad::Point2D(settings.width() / 2.0, settings.height() / 2.0));
    sprite->setFlipX(sblk->cbFlipX->checkState() == Qt::Checked);
    sprite->setFlipY(sblk->cbFlipY->checkState() == Qt::Checked);
    // Set scene before setting size to make sure, that size will be set property and not flashed
    // by onOptionsChange callback
    sprite->setScene(s_actions->currentScene());
    sprite->rendererChanged();
    sad::SceneNode* node = m_editor->shared()->selectedObject();
    if (node)
    {
        if (node->metaData()->name() == "sad::Sprite2D")
        {
            sad::Size2D size = static_cast<sad::Sprite2D*>(node)->size();
            sprite->setSize(size);
        }
    }

    QString name = snblk->txtObjectName->text();
    if (m_editor->panelProxy()->isFastModeEnabled() && !(m_editor->shared()->triggeredByFastMode()))
    {
        m_editor->shared()->setNameForFastMode(name);
    }
    if (name.length())
    {
        if (m_editor->panelProxy()->isFastModeEnabled())
        {
            name = m_editor->shared()->nameForFastMode() + QString::number(m_editor->fastModeCounter());
            if (m_editor->shared()->triggeredByFastMode())
            {
                sprite->setVisible(false);
            }
        }
        sprite->setObjectName(Q2STDSTRING(name));
    }

    s_actions->currentScene()->add(sprite);
    m_editor->shared()->setActiveObject(sprite);
    m_editor->shared()->toggleActiveBorder(true);

    m_editor->machine()->enterState("adding/sprite");
    m_editor->panelProxy()->highlightState("Click, where you want sprite to be placed");

    m_editor->actions()->sceneNodeActions()->updateRegionForNode();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::Sprite2DActions::addByDiagonalPlacing()
{
    gui::actions::SceneActions* s_actions = m_editor->actions()->sceneActions();

    gui::uiblocks::UISpriteBlock* sblk = m_editor->uiBlocks()->uiSpriteBlock();
    gui::uiblocks::UISceneNodeBlock* snblk = m_editor->uiBlocks()->uiSceneNodeBlock();



    m_editor->emitClosure(blocked_bind(
            snblk->awSceneNodeAngle,
            &gui::anglewidget::AngleWidget::setValue,
            0
    ));

    sad::Sprite2D* sprite = new sad::Sprite2D();
    sprite->setTreeName("");
    sprite->set(sblk->rtwSpriteSprite->selectedResourceName().value());

    const sad::Settings & settings = sad::Renderer::ref()->settings();
    sprite->setMiddle(sad::Point2D(settings.width() / 2.0, settings.height() / 2.0));

    core::typeconverters::QColorToSadAColor c;
    sad::AColor clr;
    c.convert(snblk->clpSceneNodeColor->selectedColor(), clr);
    sprite->setColor(clr);

    QString name = snblk->txtObjectName->text();
    if (m_editor->panelProxy()->isFastModeEnabled() && !(m_editor->shared()->triggeredByFastMode()))
    {
        m_editor->shared()->setNameForFastMode(name);
    }
    if (name.length())
    {
        if (m_editor->panelProxy()->isFastModeEnabled())
        {
            name = m_editor->shared()->nameForFastMode() + QString::number(m_editor->fastModeCounter());
            if (m_editor->shared()->triggeredByFastMode())
            {
                sprite->setVisible(false);
            }
        }
        sprite->setObjectName(Q2STDSTRING(name));
    }

    sprite->setFlipX(sblk->cbFlipX->checkState() == Qt::Checked);
    sprite->setFlipY(sblk->cbFlipY->checkState() == Qt::Checked);
    sprite->setVisible(false);

    s_actions->currentScene()->add(sprite);
    m_editor->shared()->setActiveObject(sprite);
    m_editor->shared()->toggleActiveBorder(false);

    m_editor->machine()->enterState("adding/sprite_diagonal");
    m_editor->panelProxy()->highlightState("Click, where you want upper point to be placed");

    m_editor->actions()->sceneNodeActions()->updateRegionForNode();
}


// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::Sprite2DActions::spriteOptionsChanged(sad::String s)
{
    if (m_editor->shared()->activeObject() != nullptr)
    {
        sad::Renderer::ref()->lockRendering();
        m_editor->shared()->activeObject()->setProperty("options", s);
        m_editor->shared()->activeObject()->rendererChanged();
        m_editor->actions()->sceneNodeActions()->updateRegionForNode();
        sad::Renderer::ref()->unlockRendering();
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<sad::String> old_value = node->getProperty<sad::String>("options");
            sad::Maybe<sad::Rect2D> oldrect = node->getProperty<sad::Rect2D>("area");
            if (old_value.exists() && oldrect.exists())
            {
                if (old_value.value() != s)
                {
                    sad::Renderer::ref()->lockRendering();
                    node->setProperty("options", s);
                    node->rendererChanged();
                    m_editor->actions()->sceneNodeActions()->updateRegionForNode();
                    m_editor->tryUpdateParentGridForNode(node);
                    sad::Renderer::ref()->unlockRendering();
                    m_editor->history()->add(
                        new history::sprite2d::ChangeOptions(
                            node, 
                            oldrect.value(), 
                            old_value.value(),
                            s
                        )
                    );
                }
            }
        }
    }
}

void gui::actions::Sprite2DActions::makeBackground()
{
    
    sad::SceneNode* node = m_editor->shared()->selectedObject();
    if (m_editor->shared()->activeObject() == nullptr && node)
    {
        setSceneNodeAsBackground(node, true);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::Sprite2DActions::flipXChanged(bool state)
{
    bool new_value = state;
    if (m_editor->shared()->activeObject() != nullptr)
    {
        m_editor->shared()->activeObject()->setProperty("flipx", new_value);
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<bool> old_value = node->getProperty<bool>("flipx");
            if (old_value.exists())
            {
                if (new_value != old_value.value())
                {
                    node->setProperty("flipx", new_value);
                    m_editor->history()->add(history::sprite2d::changeFlipX(
                        m_editor,
                        node, 
                        old_value.value(), 
                        new_value
                    ));
                }
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::Sprite2DActions::flipYChanged(bool state)
{
    bool new_value = state;
    if (m_editor->shared()->activeObject() != nullptr)
    {
        m_editor->shared()->activeObject()->setProperty("flipy", new_value);
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<bool> old_value = node->getProperty<bool>("flipy");
            if (old_value.exists())
            {
                if (new_value != old_value.value())
                {
                    node->setProperty("flipy", new_value);
                    m_editor->history()->add(history::sprite2d::changeFlipY(
                        m_editor,
                        node, 
                        old_value.value(), 
                        new_value
                    ));
                }
            }
        }
    }
}
