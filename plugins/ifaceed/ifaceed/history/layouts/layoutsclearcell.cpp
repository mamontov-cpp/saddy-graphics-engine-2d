#include "layoutsclearcell.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>


#include "../../core/editor.h"

#include "../../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/gridactions.h"


// ============================= PUBLIC METHODS =============================

history::layouts::ClearCell::ClearCell(
    sad::layouts::Grid* d,
    size_t row,
    size_t column,
	const sad::Vector<sad::SceneNode*>& nodes
) : m_grid(d), m_row(row), m_column(column), m_children(nodes)
{
    m_grid->addRef();
	for(size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->addRef();
	}
}

history::layouts::ClearCell::~ClearCell()
{
    m_grid->delRef();
    for(size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->delRef();
	}
}

void history::layouts::ClearCell::commit(core::Editor * ob)
{
    if (!ob)
    {
        return;
    }
    ob->emitClosure(::bind(this, &history::layouts::ClearCell::_commit, ob));
}

void history::layouts::ClearCell::rollback(core::Editor * ob)
{
    if (!ob)
    {
        return;
    }
    ob->emitClosure(::bind(this, &history::layouts::ClearCell::_rollback, ob));
}

void history::layouts::ClearCell::commitWithoutUpdatingUI(core::Editor *e)
{
    if (!e)
    {
        return;
    }
    e->emitClosure(::bind(this, &history::layouts::ClearCell::_commitWithoutUpdatingUI, e));
}

// =========================== PROTECTED METHODS ===========================

void history::layouts::ClearCell::_commit(core::Editor* ob)
{
    this->_commitWithoutUpdatingUI(ob);
    if (ob->shared()->selectedGrid() == m_grid)
    {
        gui::actions::GridActions* ga = ob->actions()->gridActions();
        ga->cellEditor(m_row, m_column)->clearChildren();
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void history::layouts::ClearCell::_rollback(core::Editor* ob)
{
    sad::layouts::Cell* cell = m_grid->cell(m_row, m_column);
	gui::actions::GridActions* ga = ob->actions()->gridActions();
	for(size_t i = 0;  i < m_children.size(); i++)
	{
		cell->addChild(m_children[i]->MajorId);
		ga->insertNodeToGridEntry(m_children[i], m_grid);
	}

	ga->updateParentGridsRecursively(m_grid);
	ga->tryUpdateRegionsInChildren(m_grid);

    if (ob->shared()->selectedGrid() == m_grid)
    {
        ga->updateRegion(true);
        gui::layouts::LayoutCellEdit* edit = ga->cellEditor(m_row, m_column);
		for(size_t i = 0; i < m_children.size(); i++)
		{
			edit->addChild(m_children[i]);
		}
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void history::layouts::ClearCell::_commitWithoutUpdatingUI(core::Editor* ob)
{
    m_grid->cell(m_row, m_column)->clear();   

	gui::actions::GridActions* ga = ob->actions()->gridActions();

	for(size_t i = 0; i < m_children.size(); i++)
	{
		ga->eraseNodeToGridEntry(m_children[i]);
	}

	ga->updateParentGridsRecursively(m_grid);
	ga->tryUpdateRegionsInChildren(m_grid);

    if (ob->shared()->selectedGrid() == m_grid)
    {
        ga->updateRegion(true);
    }
}
