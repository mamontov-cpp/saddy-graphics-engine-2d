#include <QDebug>
#include <QCheckBox>
#include <QRadioButton>

#include "customobjectactions.h"

#include <db/dbdatabase.h>

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"
#include "../qstdstring.h"

#include "../mainpanelproxy.h"

#include "../core/editor.h"

#include "../core/typeconverters/qcolortosadacolor.h"

#include "../history/scenenodes/scenenodesnew.h"

#include "../history/customobject/customobjectchangeschema.h"

#include "actions.h"
#include "scenenodeactions.h"


#include <sprite2d.h>
#include <geometry2d.h>

#include <db/custom/customobject.h>

#include "../uiblocks/uiblocks.h"
#include "../uiblocks/uiscenenodeblock.h"
#include "../uiblocks/uicustomobjectblock.h"

#include "../resourcetreewidget/resourcetreewidget.h"


// ===============================  PUBLIC METHODS ===============================

gui::actions::CustomObjectActions::CustomObjectActions(QObject* parent) : QObject(parent), gui::actions::AbstractActions()
{

}

gui::actions::CustomObjectActions::~CustomObjectActions()
{
    
}

void gui::actions::CustomObjectActions::cancelAdd()
{
    core::Shared* s = m_editor->shared();
    sad::SceneNode* node = s->activeObject();
    sad::Renderer::ref()->lockRendering();
    s->setActiveObject(NULL);
    node->scene()->remove(node);
    sad::Renderer::ref()->unlockRendering();
    
    sad::hfsm::Machine* m = m_editor->machine();
    m->enterState(m->previousState());
    if (m->isInState("adding") || (m->isInState("selected") && s->selectedObject() == NULL))
    {
        m->enterState("idle");
    }
    else
    {
        m_editor->emitClosure(bind(m_editor->panelProxy(), &MainPanelProxy::clearCustomObjectPropertiesTable));
        m_editor->panelProxy()->updateUIForSelectedItem();
    }
}

void gui::actions::CustomObjectActions::moveCenterOfObject(const sad::input::MouseMoveEvent & e)
{
    const sad::Point2D& p = e.pos2D();
    sad::SceneNode* node = m_editor->shared()->activeObject();
    if (node)
    {
        sad::db::custom::Object* o = static_cast<sad::db::custom::Object*>(node);
        sad::Rect2D area = o->area();
        sad::Point2D oldmiddle = (area[0] + area[2]) / 2.0;
        sad::moveBy(p - oldmiddle, area);
        o->setArea(area);
        m_editor->actions()->sceneNodeActions()->updateRegionForNode();
    }
}

void gui::actions::CustomObjectActions::commitAdd(const sad::input::MousePressEvent& e)
{
    core::Shared* s = m_editor->shared();
    sad::SceneNode* node = s->activeObject();
    sad::Renderer::ref()->database("")->table("scenenodes")->add(node);
    history::Command* c = new history::scenenodes::New(node);
    s->setActiveObject(NULL);
    s->setSelectedObject(node);
    m_editor->history()->add(c);
    c->commit(m_editor);
    m_editor->machine()->enterState("selected");

	m_editor->actions()->sceneNodeActions()->selectLastSceneNode();
}

void gui::actions::CustomObjectActions::placeFirstPoint(const sad::input::MousePressEvent& e)
{
    const sad::Point2D& p = e.pos2D();
    core::Shared* s = m_editor->shared();
    sad::SceneNode* node = s->activeObject();
    sad::db::custom::Object* o = static_cast<sad::db::custom::Object*>(node);
    o->setArea(sad::Rect2D(p, p));
    o->setVisible(true);

    m_editor->shared()->toggleActiveBorder(true);

    m_editor->machine()->enterState("adding/customobject_diagonal/point_placed");
    m_editor->panelProxy()->highlightState("Click, where you want lower point to be placed");

    m_editor->actions()->sceneNodeActions()->updateRegionForNode();
}

void gui::actions::CustomObjectActions::moveLowerPoint(const sad::input::MouseMoveEvent & e)
{
    const sad::Point2D& p = e.pos2D();
    sad::SceneNode* node = m_editor->shared()->activeObject();
    if (node)
    {
        sad::db::custom::Object* o = static_cast<sad::db::custom::Object*>(node);
        sad::Rect2D area = o->area();
        area = sad::Rect2D(p, area[3]);
        o->setArea(area);
		m_editor->actions()->sceneNodeActions()->updateRegionForNode();
    }
}

// ===============================  PUBLIC SLOTS METHODS ===============================

void gui::actions::CustomObjectActions::add()
{
    bool valid = true;
    valid = valid && m_panel->currentScene() != NULL;
    valid = valid && m_editor->uiBlocks()->uiCustomObjectBlock()->rtwCustomObjectSchemas->selectedResourceName().exists();
    valid = valid && m_editor->machine()->isInState("adding") == false;
    if (valid)
    {
        m_editor->emitClosure(blocked_bind(
            m_panel->UI()->awSceneNodeAngle,
            &gui::anglewidget::AngleWidget::setValue,
            0
        ));
        m_editor->emitClosure(blocked_bind(
            m_panel->UI()->clpSceneNodeColor,
            &gui::colorpicker::ColorPicker::setSelectedColor,
            QColor(255, 255, 255)
        ));
        m_editor->emitClosure(blocked_bind(
            m_panel->UI()->cbSceneNodeVisible,
            &QCheckBox::setCheckState,
            Qt::Checked
        ));
        m_editor->emitClosure(blocked_bind(
            m_panel->UI()->cbFlipX,
            &QCheckBox::setCheckState,
            Qt::Unchecked
        ));
        m_editor->emitClosure(blocked_bind(
            m_panel->UI()->cbFlipY,
            &QCheckBox::setCheckState,
            Qt::Unchecked
        ));

        m_editor->cleanupBeforeAdding();

        if (m_editor->uiBlocks()->uiCustomObjectBlock()->rbCustomObjectPlaceAndRotate->isChecked())
        {
            this->addBySimplePlacing();
        }
        else
        {
            this->addByDiagonalPlacing();
        }
    }
}

void gui::CustomObjectActions::addBySimplePlacing()
{
    sad::db::custom::Object* object = this->makeNewCustomObject();

    m_panel->currentScene()->add(object);
    m_panel->editor()->shared()->setActiveObject(object);
    m_panel->editor()->shared()->toggleActiveBorder(true);

    m_panel->editor()->machine()->enterState("adding/customobject");
    m_panel->highlightState("Click, where you want object to be placed");

    m_panel->sceneNodeActions()->updateRegionForNode();
    m_panel->fillCustomObjectProperties(object);
}


void gui::CustomObjectActions::addByDiagonalPlacing()
{
    sad::db::custom::Object* object = this->makeNewCustomObject();
    object->setVisible(false);

    m_panel->currentScene()->add(object);
    m_panel->editor()->shared()->setActiveObject(object);
    m_panel->editor()->shared()->toggleActiveBorder(false);

    m_panel->editor()->machine()->enterState("adding/customobject_diagonal");
    m_panel->highlightState("Click, where you want upper point to be placed");

    m_panel->sceneNodeActions()->updateRegionForNode();
    m_panel->fillCustomObjectProperties(object);
}

void gui::CustomObjectActions::schemaChanged(sad::String s)
{
    if (m_panel->editor()->shared()->activeObject() != NULL)
    {
        sad::Renderer::ref()->lockRendering();
        m_panel->editor()->shared()->activeObject()->setProperty("schema", s);
        m_panel->sceneNodeActions()->updateRegionForNode();
        m_panel->fillCustomObjectProperties(m_panel->editor()->shared()->activeObject());
        sad::Renderer::ref()->unlockRendering();
    }
    else
    {
        sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<sad::String> oldvalue = node->getProperty<sad::String>("schema");
            if (oldvalue.exists() && node->metaData()->canBeCastedTo("sad::db::custom::Object"))
            {
                if (oldvalue.value() != s)
                {
                    sad::db::custom::Object* o  = static_cast<sad::db::custom::Object*>(
                        m_panel->editor()->shared()->selectedObject()
                    );
                    history::Command* c = new history::customobject::ChangeSchema(
                        o, oldvalue.value(), s
                    );
                    m_panel->editor()->history()->add(c);
                    c->commit(m_panel->editor());						
                }
            }
        }
    }
}

sad::db::custom::Object* gui::CustomObjectActions::makeNewCustomObject()
{
    sad::db::custom::Object* object = new sad::db::custom::Object();
    object->setTreeName(sad::Renderer::ref(), "");
    object->setSchemaName(m_panel->UI()->rtwCustomObjectSchemas->selectedResourceName().value());
    sad::AColor acolor;
    core::typeconverters::QColorToSadAColor::convert(m_panel->UI()->clpSceneNodeColor->selectedColor(), acolor);
    object->setColor(acolor);
    object->setFontSize(m_panel->UI()->fswLabelFontSize->value());
    object->setLineSpacingRatio(m_panel->UI()->dsbLineSpacingRatio->value());
    object->setString(Q2STDSTRING(m_panel->UI()->txtLabelText->toPlainText()));

    object->setMaximalLineWidth(m_panel->UI()->spbMaximalLineWidth->value());
    object->setBreakTextFromIndex(m_panel->UI()->cmbLabelBreakText->currentIndex());
    object->setOverflowStrategyFromIndex(m_panel->UI()->cmbLabelOverflowStrategy->currentIndex());
    object->setTextEllipsisPositionAsIndex(m_panel->UI()->cmbLabelTextEllipsis->currentIndex());

    object->setMaximalLinesCount(m_panel->UI()->spbMaximalLinesCount->value());
    object->setOverflowStrategyForLinesFromIndex(m_panel->UI()->cmbLabelOverflowStrategyForLines->currentIndex());
    object->setTextEllipsisPositionForLinesAsIndex(m_panel->UI()->cmbLabelTextEllipsisForLines->currentIndex());
    tryCopySelectedObjectCustomProperties(object);
    sad::Rect2D area = object->area();
    const sad::Settings & settings = sad::Renderer::ref()->settings();

    sad::Point2D middle = (area[0] + area[2]) / 2.0;
    sad::Point2D screeenmiddle =  sad::Point2D(settings.width() / 2.0, settings.height() / 2.0);
    sad::moveBy(screeenmiddle - middle, area);
    object->setArea(area);

    QString name = m_panel->UI()->txtObjectName->text();
    if (name.length())
    {
        object->setObjectName(Q2STDSTRING(name));
    }
    return object;
}

void gui::CustomObjectActions::tryCopySelectedObjectCustomProperties(sad::db::custom::Object* object)
{
    sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
    if (node)
    {
        if (node->isInstanceOf("sad::db::custom::Object"))
        {
            sad::db::custom::Object* selected_object = static_cast<sad::db::custom::Object*>(node);
            if (selected_object->schemaName() == object->schemaName())
            {
                object->copyCustomPropertyValuesFrom(selected_object);
            }
        }
    }
}
