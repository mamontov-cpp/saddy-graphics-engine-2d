#include "scriptablegrid.h"

#include "scriptablegridcell.h"

#include <renderer.h>

#include <layouts/grid.h>

#include <db/dbdatabase.h>

#include "../queryobject.h"
#include "../scripting.h"

#include "../core/editor.h"

#include "../gui/actions/actions.h"
#include "../gui/actions/gridactions.h"

#include "../../qstdstring.h"

Q_DECLARE_METATYPE(scripting::layouts::ScriptableGrid*)
Q_DECLARE_METATYPE(scripting::layouts::ScriptableGridCell*)


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
        m_scripting->context()->throwError(std::string("ScriptableGrid.") + name.toStdString()  + ": Reference to a grid is not a valid instance");
        throw dukpp03::ArgumentException();
    }
    return NULL;
}

void scripting::layouts::ScriptableGrid::setArea(sad::Rect2D new_area) const
{
    if (!sad::isAABB(new_area))
    {
        m_scripting->context()->throwError("setArea: rectangle is not axis-aligned");
        throw new dukpp03::ArgumentException();
    }
    sad::layouts::Grid* g = grid(true, "setArea");
    if (g)
    {
        m_scripting->editor()->actions()->gridActions()->tryChangeAreaForGrid(g, new_area, false);
    }
}

dukpp03::Maybe<QVector<QVariant> > scripting::layouts::ScriptableGrid::findChild(sad::SceneNode* o)
{
    dukpp03::Maybe<QVector<QVariant> > result;
    sad::layouts::Grid* g = grid(true, "findChild");
    if (g)
    {
        sad::Maybe<sad::layouts::Grid::SearchResult> res = g->find(o);
        if (res.exists())
        {
            sad::layouts::Cell* c = g->cell(res.value().p1());

            QVector<QVariant> result_vector;

            QVariant first_variant;
            first_variant.setValue(new scripting::layouts::ScriptableGridCell(g->MajorId, c->Row, c->Col, m_scripting));

            QVariant second_variant;
            second_variant.setValue(static_cast<unsigned int>(res.value().p2()));

            result_vector << first_variant << second_variant;
            result.setValue(result_vector);
        }
    }
    return result;
}


scripting::layouts::ScriptableGridCell* scripting::layouts::ScriptableGrid::cell(int row, int column)
{
    sad::layouts::Grid* g = grid(true, "cell");
    if (g)
    {
        if (row < 0 || column < 0 || row >= g->rows() || column >= g->columns())
        {
            QString errorMessage = "ScriptableGrid.cell: There are no such cell %1, %2 in grid";
            errorMessage = errorMessage.arg(row).arg(column);
            m_scripting->context()->throwError(errorMessage.toStdString());
            throw new dukpp03::ArgumentException();
        }
        else
        {
            return new scripting::layouts::ScriptableGridCell(g->MajorId, row, column, m_scripting);
        }
    }
    else
    {
        m_scripting->context()->throwError("ScriptableGrid.cell: Attempt to get cell for non-existing grid");
        throw new dukpp03::ArgumentException();
    }
    return NULL;
}

QVector<unsigned long long> scripting::layouts::ScriptableGrid::children() const
{
    QVector<unsigned long long> result;
    sad::layouts::Grid* g = grid(true, "children");
    if (g)
    {
        sad::Vector<unsigned long long> major_ids = g->childrenMajorIds();
        for (size_t i = 0; i < major_ids.size(); i++)
        {
            result << major_ids[i];
        }
    }
    return result;
}

// ==================================== PUBLIC SLOT METHODS ====================================

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

bool scripting::layouts::ScriptableGrid::valid() const
{
    return grid(false) != NULL;
}


sad::Rect2D scripting::layouts::ScriptableGrid::area() const
{
    sad::Rect2D v;
    sad::layouts::Grid* g = grid(true, "area");
    if (g)
    {
        v = g->area();
    }
    return v;
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
        m_scripting->context()->throwError("ScriptableGrid.setRows: amount of rows cannot be lesser or equal to 0");
        throw new dukpp03::ArgumentException();
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
        m_scripting->context()->throwError("ScriptableGrid.setColumns: amount of columns cannot be lesser or equal to 0");
        throw new dukpp03::ArgumentException();
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
