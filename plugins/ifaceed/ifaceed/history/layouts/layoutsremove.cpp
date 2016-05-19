#include "layoutsremove.h"

#include "../../closuremethodcall.h"

#include "../../core/editor.h"
#include "../../core/shared.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uilayoutblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"
#include "../../gui/actions/gridactions.h"

// ================================= PUBLIC METHODS =================================

history::layouts::Remove::Remove(
    sad::layouts::Grid *grid,
    int position,
    const sad::Vector<gui::GridPosition> &parents
)
: m_grid(grid), m_position(position), m_parent_nodes(parents)
{
    m_grid->addRef();
    m_cells = m_grid->cells();
}

history::layouts::Remove::~Remove()
{
    m_grid->delRef();
}

void history::layouts::Remove::commit(core::Editor * ob)
{
    if (!ob)
    {
        return;
    }
    ob->emitClosure(::bind(
        this,
        &history::layouts::Remove::_commit,
        ob
    ));
}

void history::layouts::Remove::rollback(core::Editor * ob)
{
    if (!ob)
    {
        return;
    }
    ob->emitClosure(::bind(
        this,
        &history::layouts::Remove::_rollback,
        ob
    ));
}

// ================================= PROTECTED METHODS =================================

void history::layouts::Remove::_commit(core::Editor * ob)
{
    gui::actions::GridActions* ga = ob->actions()->gridActions();
    m_grid->Active = false;
    for(size_t i = 0; i < m_grid->rows(); i++)
    {
        for(size_t j = 0; j < m_grid->columns(); j++)
        {
            sad::layouts::Cell* cell = m_grid->cell(i, j);
            if (cell)
            {
                for(size_t k = 0; k < cell->childrenCount(); k++)
                {
                    ga->eraseNodeToGridEntry(cell->child(k));
                }
                cell->clear();
            }
        }
    }
    m_grid->update();
    ga->removeGridFromGridList(m_grid);
    for(size_t i = 0; i < m_parent_nodes.size(); i++)
    {
        gui::GridPosition pos = m_parent_nodes[i];
        sad::layouts::Grid* g = pos .Grid;
        g->cell(pos.Row, pos.Col)->removeChild(pos.Pos);
        ga->eraseNodeToGridEntry(m_grid);
        ga->removeChildFromGridInUI(g, pos.Row, pos.Col, pos.Pos);
    }
}

void history::layouts::Remove::_rollback(core::Editor* e)
{
    gui::actions::GridActions* ga = e->actions()->gridActions();
    m_grid->Active = true;
    for(size_t i = 0; i < m_cells.size(); i++)
    {
        const sad::layouts::SerializableCell& scell = m_cells[i];
        const sad::Vector<unsigned long long>& children = scell.Children;
        for(size_t j = 0; j < children.size(); j++)
        {
            ga->insertChildToGrid(m_grid, scell.Row, scell.Col, j, children[j]);
        }
    }
    m_grid->update();
    ga->insertGridToGridList(m_grid, m_position);
    for(size_t i = 0; i < m_parent_nodes.size(); i++)
    {
        gui::GridPosition pos = m_parent_nodes[i];
        sad::layouts::Grid* g = pos .Grid;
        ga->insertChildToGrid(g, pos.Row, pos.Col, pos.Pos, m_grid);
    }
}
