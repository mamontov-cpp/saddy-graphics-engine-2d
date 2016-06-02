#include "scriptablegrid.h"

#include <renderer.h>

#include <layouts/grid.h>

#include <db/dbdatabase.h>


#include "../scripting.h"

#include "../core/editor.h"

#include "../fromvalue.h"
#include "../tovalue.h"

#include "../gui/actions/actions.h"
#include "../gui/actions/gridactions.h"

#include "../../qstdstring.h"

// ==================================== PUBLIC METHODS ====================================

scripting::layouts::ScriptableGrid::ScriptableGrid(
        unsigned long long major_id,
        scripting::Scripting* s
) : m_majorid(major_id), m_scripting(s)
{
    
}

scripting::layouts::ScriptableGrid::~ScriptableGrid() 
{
    
}


QString scripting::layouts::ScriptableGrid::toString() const
{
    if (!valid())
    {
        return "ScriptableGrid(<invalid>)";
    }
    QString result = QString("ScriptableGrid(majorid : %1)")
                     .arg(m_majorid);
    return result;	
}

 sad::layouts::Grid* scripting::layouts::ScriptableGrid::grid(bool throwexc, const QString& name) const
{
    sad::db::Database* db = sad::Renderer::ref()->database("");
    if (db)
    {
        sad::layouts::Grid* grid = db->objectByMajorId<sad::layouts::Grid>(m_majorid);
        if (grid)
        {
            if (grid->Active)
            {
                return grid;
            }
        }
    }
    if (throwexc)
    {
        m_scripting->engine()->currentContext()->throwError(QString("ScriptableGrid::") + name  + ": Reference to a grid is not a valid instance");
    }
    return false;
}

// ==================================== PUBLIC SLOT METHODS ====================================

bool scripting::layouts::ScriptableGrid::valid() const
{
    return grid(false) != NULL;
}


QScriptValue scripting::layouts::ScriptableGrid::area() const
{
    sad::Rect2D v;
    sad::layouts::Grid* g = grid(true, "area");
    if (g)
    {
        v = g->area();
    }
    return scripting::FromValue<sad::Rect2D>::perform(v, m_scripting->engine());
}

void scripting::layouts::ScriptableGrid::setArea(const QScriptValue& newarea) const
{
    sad::Maybe<sad::Rect2D> rect_maybe = scripting::ToValue<sad::Rect2D>::perform(newarea);
    if (rect_maybe.exists() == false)
    {
        m_scripting->engine()->currentContext()->throwError("ScriptableGrid::setArea(): argument is not valid rectangle");
        return;
    }
    sad::layouts::Grid* g = grid(true, "setArea");
    if (g)
    {
        m_scripting->editor()->actions()->gridActions()->tryChangeAreaForGrid(g, rect_maybe.value(), false);
    }
}

unsigned long long scripting::layouts::ScriptableGrid::majorId() const
{
    unsigned long long result = 0;   
    sad::layouts::Grid* g = grid(true, "majorId");
    if (g)
    {
        result = g->MajorId;
    }
    return result;
}

unsigned long long scripting::layouts::ScriptableGrid::minorId() const
{
    unsigned long long result = 0;   
    sad::layouts::Grid* g = grid(true, "minorId");
    if (g)
    {
        result = g->MinorId;
    }
    return result;
}

QString scripting::layouts::ScriptableGrid::name() const
{
    QString result;   
    sad::layouts::Grid* g = grid(true, "name");
    if (g)
    {
        result = STD2QSTRING(g->objectName());
    }
    return result;
}

void scripting::layouts::ScriptableGrid::setName(const QString& name) const
{
    sad::layouts::Grid* g = grid(true, "setName");
    if (g)
    {
        m_scripting->editor()->actions()->gridActions()->tryChangeNameForGrid(g, Q2STDSTRING(name), false);
    }
}


bool scripting::layouts::ScriptableGrid::fixedWidth() const
{
    bool result = false;   
    sad::layouts::Grid* g = grid(true, "fixedWidth");
    if (g)
    {
        result = g->fixedWidth();
    }
    return result;    
}

void scripting::layouts::ScriptableGrid::setFixedWidth(bool fixed_width) const
{
    sad::layouts::Grid* g = grid(true, "setFixedWidth");
    if (g)
    {
        m_scripting->editor()->actions()->gridActions()->tryChangeFixedWidthForGrid(g, fixed_width, false);
    }
}

unsigned long scripting::layouts::ScriptableGrid::rows() const
{
    unsigned long result = 0;
    sad::layouts::Grid* g = grid(true, "rows");
    if (g)
    {
        result = g->rows();
    }
    return result;
}

void scripting::layouts::ScriptableGrid::setRows(int rows)
{
    if (rows <= 0)
    {
        m_scripting->engine()->currentContext()->throwError("ScriptableGrid::setRows: 0 is not a valid value for row count");
        return;
    }

    sad::layouts::Grid* g = grid(true, "setRows");
    if (g)
    {
        m_scripting->editor()->actions()->gridActions()->tryChangeRowCountForGrid(g, rows, false);
    }
}

unsigned long scripting::layouts::ScriptableGrid::columns() const
{
    unsigned long result = 0;
    sad::layouts::Grid* g = grid(true, "columns");
    if (g)
    {
        result = g->columns();
    }
    return result;
}

void scripting::layouts::ScriptableGrid::setColumns(int columns)
{
    if (columns <= 0)
    {
        m_scripting->engine()->currentContext()->throwError("ScriptableGrid::setColumns: 0 is not a valid value for row count");
        return;
    }

    sad::layouts::Grid* g = grid(true, "setColumns");
    if (g)
    {
        m_scripting->editor()->actions()->gridActions()->tryChangeColumnCountForGrid(g, columns, false);
    }
}
