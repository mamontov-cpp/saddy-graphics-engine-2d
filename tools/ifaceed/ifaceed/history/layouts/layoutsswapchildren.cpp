#include "layoutsswapchildren.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>


#include "../../core/editor.h"

#include "../../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"
#include "../../gui/actions/gridactions.h"


// ============================= PUBLIC METHODS =============================

history::layouts::SwapChildren::SwapChildren(
    sad::layouts::Grid* d,
    size_t row,
    size_t column,
    size_t pos1,
    size_t pos2
) : m_grid(d), m_row(row), m_column(column), m_pos1(pos1), m_pos2(pos2)
{
    m_grid->addRef();
}

history::layouts::SwapChildren::~SwapChildren()
{
    m_grid->delRef();
}

void history::layouts::SwapChildren::commit(core::Editor * ob)
{
    if (!ob)
    {
        return;
    }
    ob->emitClosure(::bind(this, &history::layouts::SwapChildren::_commit, ob));
}

void history::layouts::SwapChildren::rollback(core::Editor * ob)
{
    if (!ob)
    {
        return;
    }
    ob->emitClosure(::bind(this, &history::layouts::SwapChildren::_rollback, ob));
}

void history::layouts::SwapChildren::commitWithoutUpdatingUI(core::Editor *e)
{
    if (!e)
    {
        return;
    }
    e->emitClosure(::bind(this, &history::layouts::SwapChildren::_commitWithoutUpdatingUI, e));
}

// =========================== PROTECTED METHODS ===========================

void history::layouts::SwapChildren::_commit(core::Editor* ob)
{
    this->_commitWithoutUpdatingUI(ob);
    if (ob->shared()->selectedGrid() == m_grid)
    {
        gui::actions::GridActions* ga = ob->actions()->gridActions();
        ga->cellEditor(m_row, m_column)->swapChildren(m_pos1, m_pos2);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void history::layouts::SwapChildren::_rollback(core::Editor* ob)
{
    sad::layouts::Cell* cell = m_grid->cell(m_row, m_column);
    gui::actions::GridActions* ga = ob->actions()->gridActions();
    cell->swapChildren(m_pos1, m_pos2);
    if (ob->isNodeSelected(cell->child(m_pos1)))
    {
        ob->actions()->sceneNodeActions()->updateRegionForNode();
    }
    if (ob->isNodeSelected(cell->child(m_pos2)))
    {
        ob->actions()->sceneNodeActions()->updateRegionForNode();
    }

    if (ob->shared()->selectedGrid() == m_grid)
    {
        gui::layouts::LayoutCellEdit* edit = ga->cellEditor(m_row, m_column);
        edit->swapChildren(m_pos1, m_pos2);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void history::layouts::SwapChildren::_commitWithoutUpdatingUI(core::Editor* ob)
{
    sad::layouts::Cell* cell = m_grid->cell(m_row, m_column);
    gui::actions::GridActions* ga = ob->actions()->gridActions();
    cell->swapChildren(m_pos1, m_pos2);
    if (ob->isNodeSelected(cell->child(m_pos1)))
    {
        ob->actions()->sceneNodeActions()->updateRegionForNode();
    }
    if (ob->isNodeSelected(cell->child(m_pos2)))
    {
        ob->actions()->sceneNodeActions()->updateRegionForNode();
    }
}
