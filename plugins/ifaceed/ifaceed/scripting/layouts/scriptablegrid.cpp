#include "scriptablegrid.h"

#include "scriptablegridcell.h"

#include <renderer.h>

#include <layouts/grid.h>

#include <db/dbdatabase.h>

#include "../queryobject.h"
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
        m_scripting->engine()->currentContext()->throwError(QString("ScriptableGrid.") + name  + ": Reference to a grid is not a valid instance");
    }
    return NULL;
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
        m_scripting->engine()->currentContext()->throwError("ScriptableGrid.setArea(): argument is not valid rectangle");
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

bool scripting::layouts::ScriptableGrid::fixedHeight() const
{
    bool result = false;
    sad::layouts::Grid* g = grid(true, "fixedHeight");
    if (g)
    {
        result = g->fixedHeight();
    }
    return result;
}

void scripting::layouts::ScriptableGrid::setFixedHeight(bool fixed_height) const
{
    sad::layouts::Grid* g = grid(true, "setFixedHeight");
    if (g)
    {
        m_scripting->editor()->actions()->gridActions()->tryChangeFixedHeightForGrid(g, fixed_height, false);
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

void scripting::layouts::ScriptableGrid::setRows(int rows) const
{
    if (rows <= 0)
    {
        m_scripting->engine()->currentContext()->throwError("ScriptableGrid.setRows: 0 is not a valid value for row count");
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

void scripting::layouts::ScriptableGrid::setColumns(int columns) const
{
    if (columns <= 0)
    {
        m_scripting->engine()->currentContext()->throwError("ScriptableGrid.setColumns: 0 is not a valid value for row count");
        return;
    }

    sad::layouts::Grid* g = grid(true, "setColumns");
    if (g)
    {
        m_scripting->editor()->actions()->gridActions()->tryChangeColumnCountForGrid(g, columns, false);
    }
}

double scripting::layouts::ScriptableGrid::paddingTop() const
{
    double result = 0;
    sad::layouts::Grid* g = grid(true, "paddingTop");
    if (g)
    {
        result = g->paddingTop();
    }
    return result;
}

double scripting::layouts::ScriptableGrid::paddingBottom() const
{
    double result = 0;
    sad::layouts::Grid* g = grid(true, "paddingBottom");
    if (g)
    {
        result = g->paddingBottom();
    }
    return result;
}

double scripting::layouts::ScriptableGrid::paddingLeft() const
{
    double result = 0;
    sad::layouts::Grid* g = grid(true, "paddingLeft");
    if (g)
    {
        result = g->paddingLeft();
    }
    return result;
}


double scripting::layouts::ScriptableGrid::paddingRight() const
{
    double result = 0;
    sad::layouts::Grid* g = grid(true, "paddingRight");
    if (g)
    {
        result = g->paddingRight();
    }
    return result;
}


void scripting::layouts::ScriptableGrid::setPaddingTop(double v, bool p) const
{
    sad::layouts::Grid* g = grid(true, "setPaddingTop");
    if (g)
    {
        gui::actions::GridActions* ga = m_scripting->editor()->actions()->gridActions();
        ga->tryChangePaddingForGrid(
            gui::actions::GridActions::GGAUO_TopPadding,
            g,
            v,
            p,
            false
       );
    }
}

void scripting::layouts::ScriptableGrid::setPaddingBottom(double v, bool p) const
{
    sad::layouts::Grid* g = grid(true, "setPaddingBottom");
    if (g)
    {
        gui::actions::GridActions* ga = m_scripting->editor()->actions()->gridActions();
        ga->tryChangePaddingForGrid(
            gui::actions::GridActions::GGAUO_BottomPadding,
            g,
            v,
            p,
            false
       );
    }
}

void scripting::layouts::ScriptableGrid::setPaddingLeft(double v, bool p) const
{
    sad::layouts::Grid* g = grid(true, "setPaddingLeft");
    if (g)
    {
        gui::actions::GridActions* ga = m_scripting->editor()->actions()->gridActions();
        ga->tryChangePaddingForGrid(
            gui::actions::GridActions::GGAUO_LeftPadding,
            g,
            v,
            p,
            false
       );
    }
}

void scripting::layouts::ScriptableGrid::setPaddingRight(double v, bool p) const
{
    sad::layouts::Grid* g = grid(true, "setPaddingRight");
    if (g)
    {
        gui::actions::GridActions* ga = m_scripting->editor()->actions()->gridActions();
        ga->tryChangePaddingForGrid(
            gui::actions::GridActions::GGAUO_RightPadding,
            g,
            v,
            p,
            false
       );
    }
}

void scripting::layouts::ScriptableGrid::setPaddingTop(double v) const
{
    this->setPaddingTop(v, true);
}

void scripting::layouts::ScriptableGrid::setPaddingBottom(double v) const
{
    this->setPaddingBottom(v, true);
}

void scripting::layouts::ScriptableGrid::setPaddingLeft(double v) const
{
    this->setPaddingLeft(v, true);
}

void scripting::layouts::ScriptableGrid::setPaddingRight(double v) const
{
    this->setPaddingRight(v, true);
}

QScriptValue scripting::layouts::ScriptableGrid::cell(int row, int column)
{
    QScriptValue val = m_scripting->engine()->nullValue();
    sad::layouts::Grid* g = grid(true, "cell");
    if (g)
    {
        if (row < 0 || column < 0 || row >= g->rows() || column >= g->columns())
        {
            QString errorMessage = "ScriptableGrid.cell: There are no such cell %1, %2 in grid";
            errorMessage = errorMessage.arg(row).arg(column);
            m_scripting->engine()->currentContext()->throwError(errorMessage);
        }
        else
        {
            val = m_scripting->engine()->newQObject(new scripting::layouts::ScriptableGridCell(g->MajorId, row, column, m_scripting));
        }
    }
    return val;
}

bool scripting::layouts::ScriptableGrid::merge(int row, int column, int rowspan, int colspan)
{
    sad::layouts::Grid* g = grid(true, "merge");
    if (g)
    {
        gui::actions::GridActions* ga = m_scripting->editor()->actions()->gridActions();
        return ga->tryPeformMergeOrSplit(true, g, row, column, rowspan, colspan, false);
    }
    return false;
}

bool scripting::layouts::ScriptableGrid::split(int row, int column, int rowspan, int colspan)
{
    sad::layouts::Grid* g = grid(true, "split");
    if (g)
    {
        gui::actions::GridActions* ga = m_scripting->editor()->actions()->gridActions();
        return ga->tryPeformMergeOrSplit(false, g, row, column, rowspan, colspan, false);
    }
    return false;
}

QScriptValue scripting::layouts::ScriptableGrid::findChild(const QScriptValue& o)
{
    QScriptEngine* e = m_scripting->engine();
    QScriptValue val = e->nullValue();
    sad::layouts::Grid* g = grid(true, "findChild");
    if (g)
    {
        sad::Maybe<sad::SceneNode*> maybe_obj = scripting::query<sad::SceneNode*>(o);
        if (maybe_obj.exists())
        {

            sad::Maybe<sad::layouts::Grid::SearchResult> res = g->find(maybe_obj.value());
            if (res.exists())
            {
                sad::layouts::Cell* c = g->cell(res.value().p1());
                val = e->newArray(2);
                QScriptValue source = e->newQObject(new scripting::layouts::ScriptableGridCell(g->MajorId, c->Row, c->Col, m_scripting));
                val.setProperty(0, source);
                val.setProperty(1, QScriptValue(res.value().p2()));
            }
        }
        else
        {
            m_scripting->engine()->currentContext()->throwError("ScriptableGrid.findChild: argument is not a reference to a scene node");
        }
    }
    return val;
}

QScriptValue scripting::layouts::ScriptableGrid::children() const
{
    QScriptEngine* e = m_scripting->engine();
    QScriptValue val = e->newArray(0);
    sad::layouts::Grid* g = grid(true, "children");
    if (g)
    {
        val = scripting::FromValue<sad::Vector<unsigned long long> >::perform(g->childrenMajorIds(), e);
    }
    return val;
}

