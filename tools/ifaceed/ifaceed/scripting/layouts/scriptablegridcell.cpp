#include "scriptablegridcell.h"
#include "scriptablegrid.h"

#include "scriptablelengthvalue.h"

#include <renderer.h>

#include <layouts/grid.h>

#include <db/dbdatabase.h>

#include "../queryobject.h"
#include "../scripting.h"

#include "../core/editor.h"

#include "../../history/layouts/layoutschangecell.h"
#include "../../history/layouts/layoutsaddchild.h"
#include "../../history/layouts/layoutsremovechild.h"
#include "../../history/layouts/layoutsclearcell.h"
#include "../../history/layouts/layoutsswapchildren.h"

Q_DECLARE_METATYPE(scripting::layouts::ScriptableGrid*)
Q_DECLARE_METATYPE(scripting::layouts::ScriptableGridCell*)


// ================================== PUBLIC METHODS =========================

scripting::layouts::ScriptableGridCell::ScriptableGridCell(
    unsigned long long major_id,
    unsigned int row,
    unsigned int column,
    scripting::Scripting* s
) : m_majorid(major_id), m_row(row), m_column(column), m_scripting(s)
{

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
        m_scripting->context()->throwError(std::string("ScriptableGridCell.") + name.toStdString()  + ": Reference to a grid cell is not a valid instance");
        throw new dukpp03::ArgumentException();
    }
    return NULL;
}

bool scripting::layouts::ScriptableGridCell::swapChildrenWithCallName(const QString& callname, int pos1, int pos2) const
{
    bool result = false;
    sad::layouts::Cell* cell = this->cell(true, callname);
    if (cell)
    {
        if (pos1 >= 0 && pos2 >= 0 && pos1 < cell->children().size() && pos2 < cell->children().size())
        {
            result = true;
            if (pos1 != pos2)
            {
                core::Editor* editor = m_scripting->editor();
                history::layouts::SwapChildren* c = new history::layouts::SwapChildren(cell->grid(), m_row, m_column, pos1, pos2);
                c->commitWithoutUpdatingUI(editor);
                editor->currentBatchCommand()->add(c);
            }
        }
    }

    return result;    
}


QVector<unsigned long long>  scripting::layouts::ScriptableGridCell::children() const
{
    QVector<unsigned long long> result;
    sad::layouts::Cell* cell = this->cell(true, "children");
    if (cell)
    {
        sad::Vector<unsigned long long>  children = cell->childrenMajorIds();
        for(size_t i = 0; i < children.size(); i++)
        {
            result << children[i];
        }
    }
    return result;
}

dukpp03::Maybe<unsigned int> scripting::layouts::ScriptableGridCell::findChild(sad::SceneNode* o) const
{
    dukpp03::Maybe<unsigned int> result;
    sad::layouts::Cell* cell = this->cell(true, "findChild");
    if (cell)
    {
        sad::Maybe<size_t> res = cell->find(o);
        if (res.exists())
        {
            result.setValue(static_cast<unsigned int>(res.value()));
        }
    }
    return result;
}


bool scripting::layouts::ScriptableGridCell::addChild(sad::SceneNode* node) const
{
    bool result = false;
    sad::layouts::Cell* cell = this->cell(true, "addChild");
    if (cell)
    {
        core::Editor* editor = m_scripting->editor();
        gui::actions::GridActions* actions = editor->actions()->gridActions();
        if (actions->parentGridFor(node) == NULL)
        {
            sad::Maybe<sad::Rect2D> oldarea = node->getProperty<sad::Rect2D>("area");
            if (oldarea.exists())
            {
                result = true;
                history::layouts::AddChild* c = new history::layouts::AddChild(cell->grid(), m_row, m_column, node, oldarea.value());
                c->commit(editor);
                editor->currentBatchCommand()->add(c);
            }
        }
    }

    return result;
}

// ================================== PUBLIC SLOTS METHODS =========================

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


bool scripting::layouts::ScriptableGridCell::valid() const
{
    return cell(false) != NULL;
}

sad::Rect2D  scripting::layouts::ScriptableGridCell::area() const
{
    sad::Rect2D v;
    sad::layouts::Cell* c = cell(true, "area");
    if (c)
    {
        v = c->AssignedArea;
    }
    return v;
}

unsigned long long scripting::layouts::ScriptableGridCell::grid() const
{
    return m_majorid;
}


void scripting::layouts::ScriptableGridCell::setWidth(scripting::layouts::ScriptableLengthValue* value) const
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

scripting::layouts::ScriptableLengthValue* scripting::layouts::ScriptableGridCell::width() const
{
    sad::layouts::Cell* c = this->cell(true, "width");
    if (c)
    {
        return  new scripting::layouts::ScriptableLengthValue(c->width(), m_scripting);
    }
    return new scripting::layouts::ScriptableLengthValue(sad::layouts::LU_Auto, 0, m_scripting);
}

void scripting::layouts::ScriptableGridCell::setHeight(scripting::layouts::ScriptableLengthValue* value) const
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

scripting::layouts::ScriptableLengthValue*  scripting::layouts::ScriptableGridCell::height() const
{
    sad::layouts::Cell* c = this->cell(true, "height");
    if (c)
    {
        return new scripting::layouts::ScriptableLengthValue(c->height(), m_scripting);
    }
    return new scripting::layouts::ScriptableLengthValue(sad::layouts::LU_Auto, 0, m_scripting);
}

void scripting::layouts::ScriptableGridCell::setHorizontalAlignment(unsigned int v) const
{
    sad::layouts::Cell* c = this->cell(true, "setHorizontalAlignment");
    sad::Maybe<sad::layouts::HorizontalAlignment> ha_maybe;
    if ((v == sad::layouts::LHA_Left) || (v == sad::layouts::LHA_Middle) || (v == sad::layouts::LHA_Right))
    {
        ha_maybe.setValue(static_cast<sad::layouts::HorizontalAlignment>(v));
    }
    if (!ha_maybe.exists())
    {
        m_scripting->context()->throwError("ScriptableGridCell.setHorizontalAlignment: argument is not a valid E.layouts.HorizontalAlignment member");
        throw dukpp03::ArgumentException();
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


unsigned int scripting::layouts::ScriptableGridCell::horizontalAlignment() const
{
    sad::layouts::Cell* c = this->cell(true, "horizontalAlignment");
    if (!c)
    {
        m_scripting->context()->throwError("ScriptableGridCell.horizontalAlignment: attempt to get alignment for invalid cell");
        throw dukpp03::ArgumentException();
    }
    return c->horizontalAlignment();
}

void scripting::layouts::ScriptableGridCell::setVerticalAlignment(unsigned int v) const
{
    sad::layouts::Cell* c = this->cell(true, "setVerticalAlignment");
    sad::Maybe<sad::layouts::VerticalAlignment> va_maybe;
    if ((v == sad::layouts::LVA_Bottom) || (v == sad::layouts::LVA_Top) || (v == sad::layouts::LVA_Middle))
    {
        va_maybe.setValue(static_cast<sad::layouts::VerticalAlignment>(v));
    }
    if (!va_maybe.exists())
    {
        m_scripting->context()->throwError("ScriptableGridCell.setVerticalAlignment: argument is not a valid E.layouts.VerticalAlignment member");
        throw dukpp03::ArgumentException();
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

unsigned int scripting::layouts::ScriptableGridCell::verticalAlignment() const
{
    sad::layouts::Cell* c = this->cell(true, "verticalAlignment");
    if (!c)
    {
        m_scripting->context()->throwError("ScriptableGridCell.verticalAlignment: attempt to get alignment for invalid cell");
        throw dukpp03::ArgumentException();
    }
    return c->verticalAlignment();
}

void scripting::layouts::ScriptableGridCell::setStackingType(unsigned int v) const
{
    sad::layouts::Cell* c = this->cell(true, "setStackingType");
    sad::Maybe<sad::layouts::StackingType> st_maybe;
    if ((v == sad::layouts::LST_Horizontal) || (v == sad::layouts::LST_Vertical) || (v == sad::layouts::LST_NoStacking))
    {
        st_maybe.setValue(static_cast<sad::layouts::StackingType>(v));
    }
    if (!st_maybe.exists())
    {
        m_scripting->context()->throwError("ScriptableGridCell.setStackingType: argument is not a valid E.layouts.StackingType member");
        throw dukpp03::ArgumentException();
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

unsigned int scripting::layouts::ScriptableGridCell::stackingType() const
{
    sad::layouts::Cell* c = this->cell(true, "stackingType");
    if (!c)
    {
        m_scripting->context()->throwError("ScriptableGridCell.stackingType: attempt to get stacking type for invalid cell");
        throw dukpp03::ArgumentException();
    }
    return c->stackingType();
}

void scripting::layouts::ScriptableGridCell::setPaddingTop(double v) const
{
    tryChangePadding("setPaddingTop", v);
}

double scripting::layouts::ScriptableGridCell::paddingTop() const
{
    sad::layouts::Cell* c = this->cell(true, "paddingTop");
    if (c)
    {
        return c->paddingTop();
    }
    return 0;
}

void scripting::layouts::ScriptableGridCell::setPaddingBottom(double v) const
{
    tryChangePadding("setPaddingBottom", v);
}

double scripting::layouts::ScriptableGridCell::paddingBottom() const
{
    sad::layouts::Cell* c = this->cell(true, "paddingBottom");
    if (c)
    {
        return c->paddingBottom();
    }
    return 0;
}

void scripting::layouts::ScriptableGridCell::setPaddingLeft(double v) const
{
    tryChangePadding("setPaddingLeft", v);
}

double scripting::layouts::ScriptableGridCell::paddingLeft() const
{
    sad::layouts::Cell* c = this->cell(true, "paddingLeft");
    if (c)
    {
        return c->paddingLeft();
    }
    return 0;
}

void scripting::layouts::ScriptableGridCell::setPaddingRight(double v) const
{
    tryChangePadding("setPaddingRight", v);
}

double scripting::layouts::ScriptableGridCell::paddingRight() const
{
    sad::layouts::Cell* c = this->cell(true, "paddingRight");
    if (c)
    {
        return c->paddingRight();
    }
    return 0;
}

bool  scripting::layouts::ScriptableGridCell::removeChild(int pos) const
{
    bool result = false;
    sad::layouts::Cell* cell = this->cell(true, "removeChild");
    if (cell)
    {
        if (pos >=0  || pos < cell->children().size())
        {
            result = true;
            core::Editor* editor = m_scripting->editor();
            history::layouts::RemoveChild* c = new history::layouts::RemoveChild(cell->grid(), m_row, m_column, static_cast<size_t>(pos), cell->child(pos));
            c->commit(editor);
            editor->currentBatchCommand()->add(c);
        }
    }
    return result;
}

void scripting::layouts::ScriptableGridCell::clearChildren() const
{
    sad::layouts::Cell* cell = this->cell(true, "clearChildren");
    if (cell)
    {
        if (cell->children().size())
        {
            core::Editor* editor = m_scripting->editor();
            history::layouts::ClearCell* c = new history::layouts::ClearCell(cell->grid(), m_row, m_column, cell->children());
            c->commit(editor);
            editor->currentBatchCommand()->add(c);
        }
    }
}

bool scripting::layouts::ScriptableGridCell::swapChildren(int pos1, int pos2) const
{
    return swapChildrenWithCallName("swapChildren", pos1, pos2);
}

bool scripting::layouts::ScriptableGridCell::moveBack(int pos) const
{
    return swapChildrenWithCallName("moveBack", pos, pos + 1);    
}

bool scripting::layouts::ScriptableGridCell::moveFront(int pos) const
{
    return swapChildrenWithCallName("moveFront", pos - 1, pos);        
}

// ================================== PROTECTED METHODS ==================================

history::Command* scripting::layouts::ScriptableGridCell::commandForPadding(
    const QString& callname,
    const QString& propname,
    double newvalue
) const
{
    sad::layouts::Cell* c = this->cell(true, callname);
    if (c)
    {
        if (propname == "padding_top")
        {
            history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_TopPadding, double>* cmd =
                new history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_TopPadding, double>(c->grid(), m_row, m_column, propname.toStdString());
            cmd->setOldValue(c->paddingTop());
            cmd->setNewValue(newvalue);
            cmd->markAsCouldChangeRegion();
            return cmd;
        }
        if (propname == "padding_bottom")
        {
            history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_BottomPadding, double>* cmd =
                new history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_BottomPadding, double>(c->grid(), m_row, m_column, propname.toStdString());
            cmd->setOldValue(c->paddingBottom());
            cmd->setNewValue(newvalue);
            cmd->markAsCouldChangeRegion();
            return cmd;
        }
        if (propname == "padding_left")
        {
            history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_LeftPadding, double>* cmd =
                new history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_LeftPadding, double>(c->grid(), m_row, m_column, propname.toStdString());
            cmd->setOldValue(c->paddingLeft());
            cmd->setNewValue(newvalue);
            cmd->markAsCouldChangeRegion();
            return cmd;
        }
        if (propname == "padding_right")
        {
            history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_RightPadding, double>* cmd =
                new history::layouts::ChangeCell<gui::actions::GridActions::GCAUO_RightPadding, double>(c->grid(), m_row, m_column, propname.toStdString());
            cmd->setOldValue(c->paddingRight());
            cmd->setNewValue(newvalue);
            cmd->markAsCouldChangeRegion();
            return cmd;
        }
        return NULL;
    }
    return NULL;
}

void scripting::layouts::ScriptableGridCell::tryChangePadding(const QString& callname, double newvalue) const
{
    QString propname;
    QString locs[4] = {"top", "bottom" , "left", "right"};
    for(size_t i = 0; i < 4; i++)
    {
       QString tloc = locs[i];
       tloc[0] = tloc[0].toUpper();

       QString currentcallname = "setPadding";
       currentcallname.append(tloc);
       if (callname == currentcallname)
       {
           propname = "padding_";
           propname.append(locs[i]);
       }
    }
    if (propname.length())
    {
        history::Command* c = this->commandForPadding(callname, propname, newvalue);
        if (c)
        {
            core::Editor* e = m_scripting->editor();
            c->commit(e);
            e->currentBatchCommand()->add(c);
        }
    }
}
