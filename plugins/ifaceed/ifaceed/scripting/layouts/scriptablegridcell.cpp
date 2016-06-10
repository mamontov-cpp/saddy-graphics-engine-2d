#include "scriptablegridcell.h"

#include "scriptablelengthvalue.h"

#include <renderer.h>

#include <layouts/grid.h>

#include <db/dbdatabase.h>

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
        m_scripting->engine()->currentContext()->throwError(QString("ScriptableGridCell::") + name  + ": Reference to a grid cell is not a valid instance");
    }
    return NULL;
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

// ================================== PUBLIC SLOTS METHODS =========================

bool scripting::layouts::ScriptableGridCell::valid() const
{
    return cell(false) != NULL;
}
