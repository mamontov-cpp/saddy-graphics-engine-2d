#include "labelactions.h"

#include <QPlainTextEdit>
#include <QCheckBox>
#include <QLineEdit>

#include "../resourcetreewidget/resourcetreewidget.h"

#include "../qt-palette-color-picker/colorpicker.h"

#include "../fontsizewidget//fontsizewidget.h"

#include "../anglewidget/anglewidget.h"

#include <db/dbdatabase.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../mainpanelproxy.h"

#include "../uiblocks/uiblocks.h"
#include "../uiblocks/uilabelblock.h"
#include "../uiblocks/uiscenenodeblock.h"


#include "../../blockedclosuremethodcall.h"

#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../core/typeconverters/qcolortosadacolor.h"

#include "actions.h"
#include "sceneactions.h"
#include "scenenodeactions.h"

#include "../../history/scenenodes/scenenodesnew.h"

#include "../../history/label/changefontname.h"
#include "../../history/label/changefontsize.h"
#include "../../history/label/changetext.h"
#include "../../history/label/changelinespacing.h"
#include "../../history/label/changemaximallinewidth.h"
#include "../../history/label/changebreaktext.h"
#include "../../history/label/changeoverflowstrategy.h"
#include "../../history/label/changetextellipsis.h"
#include "../../history/label/changemaximallinescount.h"
#include "../../history/label/changeoverflowstrategyforlines.h"
#include "../../history/label/changetextellipsisforlines.h"
#include "../../history/label/changehasformatting.h"


#include <label.h>



// ===============================  PUBLIC METHODS ===============================

gui::actions::LabelActions::LabelActions(QObject* parent) : QObject(parent)
{

}

gui::actions::LabelActions::~LabelActions()
{
    
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::LabelActions::cancelAddLabel()
{
    core::Shared* s = this->m_editor->shared();
    sad::SceneNode* label = s->activeObject();
    sad::Renderer::ref()->lockRendering();
    s->setActiveObject(nullptr);
    label->scene()->remove(label);
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
void gui::actions::LabelActions::moveLabel(const sad::input::MouseMoveEvent & e)
{
    const sad::Point2D& p = e.pos2D();
    sad::SceneNode* node = m_editor->shared()->activeObject();
    if (node)
    {
        sad::Maybe<sad::Rect2D> mayberect = node->getProperty<sad::Rect2D>("area");
        if (mayberect.exists())
        {
            sad::Rect2D r = mayberect.value();
            double width = (r[1].x() - r[0].x()) / 2.0;
            double height = (fabs(r[2].y() - r[0].y())) / 2.0;

            r = sad::Rect2D(p.x() - width, p.y() + height, p.x() + width, p.y() - height);
            node->setProperty("area", r);
            m_editor->actions()->sceneNodeActions()->updateRegionForNode();
            node->setVisible(m_editor->uiBlocks()->uiSceneNodeBlock()->cbSceneNodeVisible->isChecked());
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::LabelActions::commitLabelAdd(const sad::input::MousePressEvent&)
{
    core::Shared* s = m_editor->shared();
    sad::SceneNode* node = s->activeObject();
    sad::Renderer::ref()->database("")->table("scenenodes")->add(node);
    history::Command* c = new history::scenenodes::New(node);
    s->setActiveObject(nullptr);
    s->setSelectedObject(node);
    m_editor->history()->add(c);
    c->commit(m_editor);
    QString oldName = m_editor->panelProxy()->getSceneNodeNameFromUI();
    m_editor->machine()->enterState("selected");
    
    m_editor->emitClosure(blocked_bind(
        m_editor->actions()->sceneNodeActions(),
        &gui::actions::SceneNodeActions::selectLastSceneNode
    ));

    if (m_editor->panelProxy()->isFastModeEnabled())
    {
        m_editor->incrementFastModeCounter();
        s->setTriggeredByFastMode(true);
        this->addLabel();
        s->setTriggeredByFastMode(false);
    }        
}

// ===============================  PUBLIC SLOTS METHODS ===============================

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::LabelActions::addLabel()
{
    bool valid = true;	
    gui::actions::SceneActions* s_actions = m_editor->actions()->sceneActions();

    gui::uiblocks::UILabelBlock* lblk = m_editor->uiBlocks()->uiLabelBlock();
    gui::uiblocks::UISceneNodeBlock* snblk = m_editor->uiBlocks()->uiSceneNodeBlock();

    gui::actions::SceneNodeActions* sn_actions = m_editor->actions()->sceneNodeActions();

    valid = valid && s_actions->currentScene() != nullptr;
    valid = valid && lblk->txtLabelText->toPlainText().length() != 0;
    valid = valid && lblk->rtwLabelFont->selectedResourceName().exists();
    valid = valid && m_editor->machine()->isInState("adding") == false;
    if (valid)
    {
        // Cleanup last adding item if was in that state
        bool blocked = snblk->cbSceneNodeVisible->blockSignals(true);
        snblk->cbSceneNodeVisible->setCheckState(Qt::Checked);
        snblk->cbSceneNodeVisible->blockSignals(blocked);

        m_editor->cleanupBeforeAdding();

        // Create and initialize label
        sad::Label* label = new sad::Label();
        label->setTreeName("");
        label->setFontName(lblk->rtwLabelFont->selectedResourceName().value());

        const sad::Settings & settings = sad::Renderer::ref()->settings();
        label->setPoint(settings.width() / 2.0, settings.height() / 2.0);
        label->setString(Q2STDSTRING(lblk->txtLabelText->toPlainText()));
        label->setSize(lblk->fswLabelFontSize->value());
        label->setLineSpacingRatio(lblk->dsbLineSpacingRatio->value());
        label->setAngle(snblk->awSceneNodeAngle->value());

        label->setMaximalLineWidth(lblk->spbMaximalLineWidth->value());
        label->setBreakTextFromIndex(lblk->cmbLabelBreakText->currentIndex());
        label->setOverflowStrategyFromIndex(lblk->cmbLabelOverflowStrategy->currentIndex());
        label->setTextEllipsisPositionAsIndex(lblk->cmbLabelTextEllipsis->currentIndex());

        label->setMaximalLinesCount(lblk->spbMaximalLinesCount->value());
        label->setOverflowStrategyForLinesFromIndex(lblk->cmbLabelOverflowStrategyForLines->currentIndex());
        label->setTextEllipsisPositionForLinesAsIndex(lblk->cmbLabelTextEllipsisForLines->currentIndex());
        label->setHasFormatting(lblk->cbLabelHasFormatting->isChecked());

        sad::AColor clr;
        core::typeconverters::QColorToSadAColor::convert(snblk->clpSceneNodeColor->selectedColor(), clr);
        label->setColor(clr);

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
                    label->setVisible(false);
                }
            }
            label->setObjectName(Q2STDSTRING(name));
        }

        // Actually add label
        s_actions->currentScene()->add(label);
        m_editor->shared()->setActiveObject(label);
        m_editor->shared()->toggleActiveBorder(true);

        m_editor->machine()->enterState("adding/label");
        m_editor->panelProxy()->highlightState("Click, where you want label to be placed");

        sn_actions->updateRegionForNode();
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::LabelActions::labelFontChanged(sad::String s)
{
    if (m_editor->shared()->activeObject() != nullptr)
    {
        sad::Renderer::ref()->lockRendering();
        m_editor->shared()->activeObject()->setProperty("font", s);
        sad::Renderer::ref()->unlockRendering();
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<sad::String> old_value = node->getProperty<sad::String>("font");
            if (old_value.exists())
            {
                if (old_value.value() != s)
                {
                    sad::Renderer::ref()->lockRendering();
                    node->setProperty("font", s);
                    m_editor->actions()->sceneNodeActions()->tryUpdateParentGridForNode(node);
                    sad::Renderer::ref()->unlockRendering();
                    m_editor->history()->add(new history::label::ChangeFontName(node, old_value.value(), s));
                }
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::LabelActions::labelSizeChanged(unsigned int s)
{
    gui::actions::SceneNodeActions* sn_actions = m_editor->actions()->sceneNodeActions();

    if (m_editor->shared()->activeObject() != nullptr)
    {
        sad::Renderer::ref()->lockRendering();
        m_editor->shared()->activeObject()->setProperty("fontsize", s);
        sad::Renderer::ref()->unlockRendering();

        sn_actions->updateRegionForNode();
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<unsigned int> old_value = node->getProperty<unsigned int>("fontsize");
            if (old_value.exists())
            {
                if (old_value.value() != s)
                {
                    sad::Renderer::ref()->lockRendering();
                    node->setProperty("fontsize", s);
                    m_editor->actions()->sceneNodeActions()->tryUpdateParentGridForNode(node);
                    sad::Renderer::ref()->unlockRendering();
                    sn_actions->updateRegionForNode();
                    m_editor->history()->add(new history::label::ChangeFontSize(node, old_value.value(), s));
                }
            }
        }
    }
}


// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::LabelActions::labelTextChanged()
{
    gui::uiblocks::UILabelBlock* lblk = m_editor->uiBlocks()->uiLabelBlock();
    
    gui::actions::SceneNodeActions* sn_actions = m_editor->actions()->sceneNodeActions();

    sad::String new_value = Q2STDSTRING(lblk->txtLabelText->toPlainText());
    if (m_editor->shared()->activeObject() != nullptr)
    {
        m_editor->shared()->activeObject()->setProperty("text", new_value);
        sn_actions->updateRegionForNode();
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<sad::String> old_value = node->getProperty<sad::String>("text");
            if (old_value.exists())
            {
                if (old_value.value() != new_value)
                {
                    node->setProperty("text", new_value);
                    sn_actions->updateRegionForNode();
                    m_editor->actions()->sceneNodeActions()->tryUpdateParentGridForNode(node);
                    m_editor->history()->add(new history::label::ChangeText(node, old_value.value(), new_value));
                }
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::LabelActions::labelLineSpacingChanged(double new_value)
{
    gui::actions::SceneNodeActions* sn_actions = m_editor->actions()->sceneNodeActions();
 
    if (m_editor->shared()->activeObject() != nullptr)
    {
        m_editor->shared()->activeObject()->setProperty("linespacing", new_value);
        sn_actions->updateRegionForNode();
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<float> old_value = node->getProperty<float>("linespacing");
            if (old_value.exists())
            {
                if (sad::is_fuzzy_equal(old_value.value(), new_value, 0.0001) == false)
                {
                    node->setProperty("linespacing", new_value);
                    sn_actions->updateRegionForNode();
                    m_editor->actions()->sceneNodeActions()->tryUpdateParentGridForNode(node);
                    m_editor->history()->add(new history::label::ChangeLineSpacing(node, old_value.value(), new_value));
                }
            }
        }
    }
}

void gui::actions::LabelActions::labelMaximalLineWidthChanged(int new_value)
{
    unsignedIntPropertyChanged(
        new_value, 
        "maximallinewidth", 
        &gui::actions::LabelActions::command<history::label::ChangeMaximalLineWidth>
    );    
}

void gui::actions::LabelActions::labelBreakTextChanged(int new_value)
{
    unsignedIntPropertyChanged(
        new_value, 
        "breaktext", 
        &gui::actions::LabelActions::command<history::label::ChangeBreakText>
    );    
}

void gui::actions::LabelActions::labelOverflowStrategyChanged(int new_value)
{
    unsignedIntPropertyChanged(
        new_value, 
        "overflowstrategy", 
        &gui::actions::LabelActions::command<history::label::ChangeOverflowStrategy>
    );  
}

void gui::actions::LabelActions::labelTextEllipsisChanged(int new_value)
{
    unsignedIntPropertyChanged(
        new_value, 
        "textellipsisposition", 
        &gui::actions::LabelActions::command<history::label::ChangeTextEllipsis>
    );  
}

void gui::actions::LabelActions::labelMaximalLinesCountChanged(int new_value)
{
    unsignedIntPropertyChanged(
        new_value, 
        "maximallinescount", 
        &gui::actions::LabelActions::command<history::label::ChangeMaximalLinesCount>
    );    
}

void gui::actions::LabelActions::labelOverflowStrategyForLinesChanged(int new_value)
{
    unsignedIntPropertyChanged(
        new_value, 
        "overflowstrategyforlines", 
        &gui::actions::LabelActions::command<history::label::ChangeOverflowStrategyForLines>
    ); 
}

void gui::actions::LabelActions::labelTextEllipsisForLinesChanged(int new_value)
{
    unsignedIntPropertyChanged(
        new_value, 
        "textellipsispositionforlines", 
        &gui::actions::LabelActions::command<history::label::ChangeTextEllipsisForLines>
    ); 
}

void gui::actions::LabelActions::labelHasFormattingChanged(bool state)
{
    gui::actions::SceneNodeActions* sn_actions = m_editor->actions()->sceneNodeActions();

    bool new_value = state;
    if (m_editor->shared()->activeObject() != nullptr)
    {
        m_editor->shared()->activeObject()->setProperty("hasformatting", new_value);
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<bool> old_value = node->getProperty<bool>("hasformatting");
            if (old_value.exists())
            {
                if (new_value != old_value.value())
                {
                    node->setProperty("hasformatting", new_value);
                    sn_actions->updateRegionForNode();
                    m_editor->actions()->sceneNodeActions()->tryUpdateParentGridForNode(node);
                    m_editor->history()->add(new history::label::ChangeHasFormatting(
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
void gui::actions::LabelActions::unsignedIntPropertyChanged(
    int new_value,
    const sad::String& prop,
    gui::actions::LabelActions::CommandMaker maker
)
{
    gui::actions::SceneNodeActions* sn_actions = m_editor->actions()->sceneNodeActions();

    unsigned int nv = static_cast<unsigned int>(new_value);
    if (m_editor->shared()->activeObject() != nullptr)
    {
        m_editor->shared()->activeObject()->setProperty(prop, nv);
        sn_actions->updateRegionForNode();
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<unsigned int> old_value = node->getProperty<unsigned int>(prop);
            if (old_value.exists())
            {
                if (old_value.value() != nv)
                {
                    node->setProperty(prop, new_value);
                    sn_actions->updateRegionForNode();
                    m_editor->actions()->sceneNodeActions()->tryUpdateParentGridForNode(node);
                    history::Command* p = (this->*maker)(node, old_value.value(), new_value);
                    m_editor->history()->add(p);
                }
            }
        }
    }
}