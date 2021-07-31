#include <QDebug>
#include <QCheckBox>
#include <QLineEdit>
#include <QPlainTextEdit>
// ReSharper disable once CppUnusedIncludeDirective
#include <QRadioButton>

#include "customobjectactions.h"

#include <db/dbdatabase.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>


#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"
#include "../qstdstring.h"

#include "../mainpanelproxy.h"

#include "../core/editor.h"

#include "../core/typeconverters/qcolortosadacolor.h"

#include "../history/scenenodes/scenenodesnew.h"

#include "../history/customobject/customobjectchangeschema.h"

#include "../gui/table/delegate.h"
#include "../gui/table/delegatefactory.h"


#include "actions.h"
#include "scenenodeactions.h"
#include "sceneactions.h"


#include <geometry2d.h>

#include <db/custom/customobject.h>

#include "../uiblocks/uiblocks.h"
#include "../uiblocks/uiscenenodeblock.h"
#include "../uiblocks/uicustomobjectblock.h"
#include "../uiblocks/uispriteblock.h"
#include "../uiblocks/uilabelblock.h"

#include "../resourcetreewidget/resourcetreewidget.h"
#include "../anglewidget/anglewidget.h"
#include "../qt-palette-color-picker/colorpicker.h"
#include "../fontsizewidget/fontsizewidget.h"

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
        m_editor->emitClosure(bind(this, &gui::actions::CustomObjectActions::clearCustomObjectPropertiesTable));
        m_editor->actions()->sceneNodeActions()->updateUIForSelectedSceneNode();
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
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
        // Do it only in case of moving first
        if (m_editor->machine()->isInState("adding/customobject_diagonal") == false)
        {
            node->setVisible(m_editor->uiBlocks()->uiSceneNodeBlock()->cbSceneNodeVisible->isChecked());
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::CustomObjectActions::commitAdd(const sad::input::MousePressEvent& e)
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

    m_editor->actions()->sceneNodeActions()->selectLastSceneNode();
    if (m_editor->panelProxy()->isFastModeEnabled())
    {
        m_editor->incrementFastModeCounter();
        m_editor->emitClosure(bind(
            this,
            &gui::actions::CustomObjectActions::triggerAddFromFastMode
        ));        
        //if (m_editor->uiBlocks()->uiSpriteBlock()->rbTwoClicksPlaces->isChecked())
        //{
            //m_block_first_point_for_fast_mode = true;
        //}
    }
}


// ReSharper disable once CppMemberFunctionMayBeConst
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

// ReSharper disable once CppMemberFunctionMayBeConst
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


void gui::actions::CustomObjectActions::fillCustomObjectProperties(
    sad::SceneNode* node	
)
{
    this->clearCustomObjectPropertiesTable();
    if (node)
    {
        if (node->metaData()->canBeCastedTo("sad::db::custom::Object"))
        {
            sad::db::custom::Object* o = static_cast<sad::db::custom::Object*>(node);
            const sad::Hash<sad::String, sad::db::Property*>& db = o->schemaProperties();
            for(sad::Hash<sad::String, sad::db::Property*>::const_iterator it = db.const_begin();
                it != db.const_end();
                ++it)
            {
                gui::table::Delegate* d = m_editor->panelProxy()->delegateFactory()->create(STD2QSTRING(it.value()->baseType().c_str()));
                if (d)
                {
                    d->makeLinkedTo(m_editor->uiBlocks()->uiCustomObjectBlock()->twCustomObjectProperties, m_editor);
                    d->setPropertyName(STD2QSTRING(it.key().c_str()));
                    d->linkToCustomObject(o);
                    d->add();				
                }
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::CustomObjectActions::clearCustomObjectPropertiesTable()
{
    QTableWidget* twCustomObjectProperties = m_editor->uiBlocks()->uiCustomObjectBlock()->twCustomObjectProperties; 
    for(size_t i = 0; i < twCustomObjectProperties->rowCount(); i++)
    {
        QVariant  v = twCustomObjectProperties->item(i, 0)->data(Qt::UserRole);
        gui::table::Delegate* d = v.value<gui::table::Delegate*>();
        if (d)
        {
            d->disconnectSlots();
            delete d;
        }
    }
    twCustomObjectProperties->clear();
    twCustomObjectProperties->setRowCount(0);	
}

// ReSharper disable once CppMemberFunctionMayBeConst
gui::table::Delegate* gui::actions::CustomObjectActions::delegateForCustomObjectProperty(const QString& name)
{
    QTableWidget* twCustomObjectProperties = m_editor->uiBlocks()->uiCustomObjectBlock()->twCustomObjectProperties; 
    
     for(size_t i = 0; i < twCustomObjectProperties->rowCount(); i++)
     {
         if (twCustomObjectProperties->item(i, 0)->text() == name) {
             QVariant  v = twCustomObjectProperties->item(i, 0)->data(Qt::UserRole);
             gui::table::Delegate* d = v.value<gui::table::Delegate*>();
             return d;
         }
     }
     return nullptr;
}

void gui::actions::CustomObjectActions::updateCustomObjectPropertyValue(
     sad::SceneNode* node,
     const sad::String& name,
     const sad::db::Variant& value
)
{
    if (m_editor->isNodeSelected(node))
    {
        m_custom_object_property_name = name;
        m_custom_object_property_value = value;
        QTimer::singleShot(0, this, SLOT(updateCustomObjectPropertyValueNow()));
    }
}

// ===============================  PUBLIC SLOTS METHODS ===============================

void gui::actions::CustomObjectActions::add()
{
    bool valid = true;
    valid = valid && m_editor->actions()->sceneActions()->currentScene() != nullptr;
    valid = valid && m_editor->uiBlocks()->uiCustomObjectBlock()->rtwCustomObjectSchemas->selectedResourceName().exists();
    valid = valid && m_editor->machine()->isInState("adding") == false;
    if (valid)
    {
        m_editor->emitClosure(blocked_bind(
            m_editor->uiBlocks()->uiSceneNodeBlock()->awSceneNodeAngle,
            &gui::anglewidget::AngleWidget::setValue,
            0
        ));
        m_editor->emitClosure(blocked_bind(
            m_editor->uiBlocks()->uiSceneNodeBlock()->clpSceneNodeColor,
            &gui::colorpicker::ColorPicker::setSelectedColor,
            QColor(255, 255, 255)
        ));
        m_editor->emitClosure(blocked_bind(
            m_editor->uiBlocks()->uiSceneNodeBlock()->cbSceneNodeVisible,
            &QCheckBox::setCheckState,
            Qt::Checked
        ));
        m_editor->emitClosure(blocked_bind(
            m_editor->uiBlocks()->uiSpriteBlock()->cbFlipX,
            &QCheckBox::setCheckState,
            Qt::Unchecked
        ));
        m_editor->emitClosure(blocked_bind(
            m_editor->uiBlocks()->uiSpriteBlock()->cbFlipY,
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

void gui::actions::CustomObjectActions::addBySimplePlacing()
{
    sad::db::custom::Object* object = this->makeNewCustomObject();

    m_editor->actions()->sceneActions()->currentScene()->add(object);
    m_editor->shared()->setActiveObject(object);
    m_editor->shared()->toggleActiveBorder(true);

    m_editor->machine()->enterState("adding/customobject");
    m_editor->panelProxy()->highlightState("Click, where you want object to be placed");

    m_editor->actions()->sceneNodeActions()->updateRegionForNode();
    this->fillCustomObjectProperties(object);
}


void gui::actions::CustomObjectActions::addByDiagonalPlacing()
{
    sad::db::custom::Object* object = this->makeNewCustomObject();
    object->setVisible(false);

    m_editor->actions()->sceneActions()->currentScene()->add(object);
    m_editor->shared()->setActiveObject(object);
    m_editor->shared()->toggleActiveBorder(false);

    m_editor->machine()->enterState("adding/customobject_diagonal");
    m_editor->panelProxy()->highlightState("Click, where you want upper point to be placed");

    m_editor->actions()->sceneNodeActions()->updateRegionForNode();
    this->fillCustomObjectProperties(object);
}

void gui::actions::CustomObjectActions::triggerAddFromFastMode()
{
    core::Shared* s = m_editor->shared();
    s->setTriggeredByFastMode(true);        
    this->add();
    s->setTriggeredByFastMode(false);            
}

void gui::actions::CustomObjectActions::schemaChanged(sad::String s)
{
    if (m_editor->shared()->activeObject() != nullptr)
    {
        sad::Renderer::ref()->lockRendering();
        m_editor->shared()->activeObject()->setProperty("schema", s);
        m_editor->actions()->sceneNodeActions()->updateRegionForNode();
        this->fillCustomObjectProperties(m_editor->shared()->activeObject());
        sad::Renderer::ref()->unlockRendering();
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<sad::String> old_value = node->getProperty<sad::String>("schema");
            if (old_value.exists() && node->metaData()->canBeCastedTo("sad::db::custom::Object"))
            {
                if (old_value.value() != s)
                {
                    sad::db::custom::Object* o  = static_cast<sad::db::custom::Object*>(
                        m_editor->shared()->selectedObject()
                    );
                    history::Command* c = new history::customobject::ChangeSchema(
                        o, old_value.value(), s
                    );
                    m_editor->history()->add(c);
                    c->commit(m_editor);						
                }
            }
        }
    }
}

void gui::actions::CustomObjectActions::updateCustomObjectPropertyValueNow()
{
    gui::table::Delegate* d = this->delegateForCustomObjectProperty(STD2QSTRING(m_custom_object_property_name.c_str()));
    if (d)
    {
        d->set(m_custom_object_property_value);
    }
}

// =================================================== PRIVATE METHODS ===================================================

sad::db::custom::Object* gui::actions::CustomObjectActions::makeNewCustomObject()
{
    gui::uiblocks::UILabelBlock* lblk = m_editor->uiBlocks()->uiLabelBlock();
    gui::uiblocks::UICustomObjectBlock* cublk = m_editor->uiBlocks()->uiCustomObjectBlock();
    gui::uiblocks::UISceneNodeBlock* scblk = m_editor->uiBlocks()->uiSceneNodeBlock();

    sad::db::custom::Object* object = new sad::db::custom::Object();
    object->setTreeName(sad::Renderer::ref(), "");
    object->setSchemaName(cublk->rtwCustomObjectSchemas->selectedResourceName().value());
    sad::AColor acolor;
    core::typeconverters::QColorToSadAColor::convert(scblk->clpSceneNodeColor->selectedColor(), acolor);
    object->setColor(acolor);
    object->setFontSize(lblk->fswLabelFontSize->value());
    object->setLineSpacingRatio(lblk->dsbLineSpacingRatio->value());
    object->setString(Q2STDSTRING(lblk->txtLabelText->toPlainText()));

    object->setMaximalLineWidth(lblk->spbMaximalLineWidth->value());
    object->setBreakTextFromIndex(lblk->cmbLabelBreakText->currentIndex());
    object->setOverflowStrategyFromIndex(lblk->cmbLabelOverflowStrategy->currentIndex());
    object->setTextEllipsisPositionAsIndex(lblk->cmbLabelTextEllipsis->currentIndex());

    object->setMaximalLinesCount(lblk->spbMaximalLinesCount->value());
    object->setOverflowStrategyForLinesFromIndex(lblk->cmbLabelOverflowStrategyForLines->currentIndex());
    object->setTextEllipsisPositionForLinesAsIndex(lblk->cmbLabelTextEllipsisForLines->currentIndex());
    object->setHasFormatting(lblk->cbLabelHasFormatting->isChecked());
    tryCopySelectedObjectCustomProperties(object);
    sad::Rect2D area = object->area();
    const sad::Settings & settings = sad::Renderer::ref()->settings();

    sad::Point2D middle = (area[0] + area[2]) / 2.0;
    sad::Point2D screeenmiddle =  sad::Point2D(settings.width() / 2.0, settings.height() / 2.0);
    sad::moveBy(screeenmiddle - middle, area);
    object->setArea(area);

    QString name = m_editor->uiBlocks()->uiSceneNodeBlock()->txtObjectName->text();
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
                object->setVisible(false);
            }
        }
        object->setObjectName(Q2STDSTRING(name));
    }
    return object;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::CustomObjectActions::tryCopySelectedObjectCustomProperties(sad::db::custom::Object* object)
{
    sad::SceneNode* node = m_editor->shared()->selectedObject();
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
