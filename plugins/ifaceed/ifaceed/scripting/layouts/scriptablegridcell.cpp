#include "scriptablegridcell.h"

#include "scriptablelengthvalue.h"

#include <renderer.h>

#include <layouts/grid.h>

#include <db/dbdatabase.h>

#include "../queryobject.h"
#include "../scripting.h"

#include "../core/editor.h"

#include "../fromvalue.h"
#include "../tovalue.h"

#include "../../history/layouts/layoutschangecell.h"

// ================================== PUBLIC METHODS =========================

scripting::layouts::ScriptableGridCell::ScriptableGridCell(
    unsigned long long major_id,
    unsigned int row,
    unsigned int column,
    scripting::Scripting* s
) : m_majorid(major_id), m_row(row), m_column(column), m_scripting(s)
{

}

QString scripting::layouts::ScriptableGridCell::toString() const
{
    if (!valid())
    {
        return "ScriptableGridCell(<invalid>)";
    }
    QString result = QString("ScriptableGridCell(majorid : %1, row : %2, column: %3)")
                     .arg(m_majorid).arg(m_row).arg(m_column);
    return result;
}

scripting::layouts::ScriptableGridCell::~ScriptableGridCell()
{

}

sad::layouts::Cell* scripting::layouts::ScriptableGridCell::cell(bool throwexc, const QString& name) const
{
    sad::db::Database* db = sad::Renderer::ref()->database("");
    if (db)
    {
        sad::layouts::Grid* grid = db->objectByMajorId<sad::layouts::Grid>(m_majorid);
        if (grid)
        {
            if (grid->Active)
            {
                sad::layouts::Cell* cell = grid->cell(m_row, m_column);
                if (cell)
                {
                    return cell;
                }
            }
        }
    }
    if (throwexc)
    {
        m_scripting->engine()->currentContext()->throwError(QString("ScriptableGridCell.") + name  + ": Reference to a grid cell is not a valid instance");
    }
    return NULL;
}

// ================================== PUBLIC SLOTS METHODS =========================

bool scripting::layouts::ScriptableGridCell::valid() const
{
    return cell(false) != NULL;
}


void scripting::layouts::ScriptableGridCell::setWidth(scripting::layouts::ScriptableLengthValue* value)
{
    sad::layouts::Cell* c = this->cell(true, "setWidth");
    if (c)
    {
        history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_Width, sad::layouts::LengthValue>* cmd =
            new history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_Width, sad::layouts::LengthValue>(c->grid(), m_row, m_column, "width");
        cmd->setOldValue(c->width());
        cmd->setNewValue(value->toValue());
        cmd->markAsCouldChangeRegion();
        core::Editor* e = m_scripting->editor();
        cmd->commit(e);
        e->currentBatchCommand()->add(cmd);
    }
}

QScriptValue scripting::layouts::ScriptableGridCell::width() const
{
    sad::layouts::Cell* c = this->cell(true, "width");
    QScriptEngine* engine = m_scripting->engine();
    scripting::layouts::ScriptableLengthValue* lv;
    if (c)
    {
        lv = new scripting::layouts::ScriptableLengthValue(c->width(), m_scripting);
    }
    else
    {
        lv = new scripting::layouts::ScriptableLengthValue(sad::layouts::LU_Auto, 0, m_scripting);
    }
    return engine->newQObject(lv);
}

void scripting::layouts::ScriptableGridCell::setHeight(scripting::layouts::ScriptableLengthValue* value)
{
    sad::layouts::Cell* c = this->cell(true, "setHeight");
    if (c)
    {
        history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_Height, sad::layouts::LengthValue>* cmd =
            new history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_Height, sad::layouts::LengthValue>(c->grid(), m_row, m_column, "height");
        cmd->setOldValue(c->height());
        cmd->setNewValue(value->toValue());
        cmd->markAsCouldChangeRegion();
        core::Editor* e = m_scripting->editor();
        cmd->commit(e);
        e->currentBatchCommand()->add(cmd);
    }
}

QScriptValue scripting::layouts::ScriptableGridCell::height() const
{
    sad::layouts::Cell* c = this->cell(true, "height");
    QScriptEngine* engine = m_scripting->engine();
    scripting::layouts::ScriptableLengthValue* lv;
    if (c)
    {
        lv = new scripting::layouts::ScriptableLengthValue(c->height(), m_scripting);
    }
    else
    {
        lv = new scripting::layouts::ScriptableLengthValue(sad::layouts::LU_Auto, 0, m_scripting);
    }
    return engine->newQObject(lv);
}

QScriptValue scripting::layouts::ScriptableGridCell::children() const
{
    QScriptEngine* e = m_scripting->engine();
    QScriptValue val = e->newArray(0);
    sad::layouts::Cell* cell = this->cell(true, "children");
    if (cell)
    {
        val = scripting::FromValue<sad::Vector<unsigned long long> >::perform(cell->childrenMajorIds(), e);
    }
    return val;
}

QScriptValue scripting::layouts::ScriptableGridCell::findChild(const QScriptValue& o)
{
    QScriptEngine* e = m_scripting->engine();
    QScriptValue val = e->nullValue();
    sad::layouts::Cell* cell = this->cell(true, "findChild");
    if (cell)
    {
        sad::Maybe<sad::SceneNode*> maybe_obj = scripting::query<sad::SceneNode*>(o);
        if (maybe_obj.exists())
        {

            sad::Maybe<size_t> res = cell->find(maybe_obj.value());
            if (res.exists())
            {
                val = QScriptValue(res.value());
            }
        }
        else
        {
            m_scripting->engine()->currentContext()->throwError("ScriptableGridCell.findChild: argument is not a reference to a scene node");
        }
    }
    return val;
}


void scripting::layouts::ScriptableGridCell::setHorizontalAlignment(const QScriptValue& v)
{
    sad::layouts::Cell* c = this->cell(true, "setHorizontalAlignment");
    sad::Maybe<sad::layouts::HorizontalAlignment> ha_maybe = scripting::ToValue<sad::layouts::HorizontalAlignment>::perform(v);
    if (!ha_maybe.exists())
    {
        m_scripting->engine()->currentContext()->throwError("ScriptableGridCell.setHorizontalAlignment: argument is not a valid E.layouts.HorizontalAlignment member");
    }
    if (c && ha_maybe.exists())
    {
        history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_HorizontalAlignment, sad::layouts::HorizontalAlignment>* cmd =
            new history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_HorizontalAlignment, sad::layouts::HorizontalAlignment>(c->grid(), m_row, m_column, "halign");
        cmd->setOldValue(c->horizontalAlignment());
        cmd->setNewValue(ha_maybe.value());
        core::Editor* e = m_scripting->editor();
        cmd->commit(e);
        e->currentBatchCommand()->add(cmd);
    }
}


QScriptValue scripting::layouts::ScriptableGridCell::horizontalAlignment() const
{
    sad::layouts::Cell* c = this->cell(true, "horizontalAlignment");
    QScriptEngine* engine = m_scripting->engine();
    QScriptValue lv = engine->nullValue();
    if (c)
    {
       lv = scripting::FromValue<sad::layouts::HorizontalAlignment>::perform(c->horizontalAlignment(), engine);
    }
    return lv;
}

void scripting::layouts::ScriptableGridCell::setVerticalAlignment(const QScriptValue& v)
{
    sad::layouts::Cell* c = this->cell(true, "setVerticalAlignment");
    sad::Maybe<sad::layouts::VerticalAlignment> va_maybe = scripting::ToValue<sad::layouts::VerticalAlignment>::perform(v);
    if (!va_maybe.exists())
    {
        m_scripting->engine()->currentContext()->throwError("ScriptableGridCell.setVerticalAlignment: argument is not a valid E.layouts.VerticalAlignment member");
    }
    if (c && va_maybe.exists())
    {
        history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_VerticalAlignment, sad::layouts::VerticalAlignment>* cmd =
            new history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_VerticalAlignment, sad::layouts::VerticalAlignment>(c->grid(), m_row, m_column, "valign");
        cmd->setOldValue(c->verticalAlignment());
        cmd->setNewValue(va_maybe.value());
        core::Editor* e = m_scripting->editor();
        cmd->commit(e);
        e->currentBatchCommand()->add(cmd);
    }
}

QScriptValue scripting::layouts::ScriptableGridCell::verticalAlignment() const
{
    sad::layouts::Cell* c = this->cell(true, "verticalAlignment");
    QScriptEngine* engine = m_scripting->engine();
    QScriptValue lv = engine->nullValue();
    if (c)
    {
       lv = scripting::FromValue<sad::layouts::VerticalAlignment>::perform(c->verticalAlignment(), engine);
    }
    return lv;
}

void scripting::layouts::ScriptableGridCell::setStackingType(const QScriptValue& v)
{
    sad::layouts::Cell* c = this->cell(true, "setStackingType");
    sad::Maybe<sad::layouts::StackingType> st_maybe = scripting::ToValue<sad::layouts::StackingType>::perform(v);
    if (!st_maybe.exists())
    {
        m_scripting->engine()->currentContext()->throwError("ScriptableGridCell.setStackingType: argument is not a valid E.layouts.StackingType member");
    }
    if (c && st_maybe.exists())
    {
        history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_StackingType, sad::layouts::StackingType>* cmd =
            new history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_StackingType, sad::layouts::StackingType>(c->grid(), m_row, m_column, "stacking_type");
        cmd->setOldValue(c->stackingType());
        cmd->setNewValue(st_maybe.value());
        cmd->markAsCouldChangeRegion();
        core::Editor* e = m_scripting->editor();
        cmd->commit(e);
        e->currentBatchCommand()->add(cmd);
    }
}

QScriptValue scripting::layouts::ScriptableGridCell::stackingType() const
{
    sad::layouts::Cell* c = this->cell(true, "stackingType");
    QScriptEngine* engine = m_scripting->engine();
    QScriptValue lv = engine->nullValue();
    if (c)
    {
       lv = scripting::FromValue<sad::layouts::StackingType>::perform(c->stackingType(), engine);
    }
    return lv;
}
