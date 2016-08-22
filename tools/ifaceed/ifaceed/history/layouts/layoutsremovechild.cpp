#include "layoutsremovechild.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>


#include "../../core/editor.h"

#include "../../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/gridactions.h"


// ============================= PUBLIC METHODS =============================

history::layouts::RemoveChild::RemoveChild(
    sad::layouts::Grid* d,
    size_t row,
    size_t column,
    size_t pos,
    sad::SceneNode* node
) : m_grid(d), m_row(row), m_column(column), m_position(pos), m_node(node)
{
    m_grid->addRef();;
    m_node->addRef();
}

history::layouts::RemoveChild::~RemoveChild()
{
    m_grid->delRef();
    m_node->delRef();
}

void history::layouts::RemoveChild::commit(core::Editor * ob)
{
    if (!ob)
    {
        return;
    }
    ob->emitClosure(::bind(this, &history::layouts::RemoveChild::_commit, ob));
}

void history::layouts::RemoveChild::rollback(core::Editor * ob)
{
    if (!ob)
    {
        return;
    }
    ob->emitClosure(::bind(this, &history::layouts::RemoveChild::_rollback, ob));
}

void history::layouts::RemoveChild::commitWithoutUpdatingUI(core::Editor *e)
{
    if (!e)
    {
        return;
    }
    e->emitClosure(::bind(this, &history::layouts::RemoveChild::_commitWithoutUpdatingUI, e));
}

// =========================== PROTECTED METHODS ===========================

void history::layouts::RemoveChild::_commit(core::Editor* ob)
{
    this->_commitWithoutUpdatingUI(ob);
    if (ob->shared()->selectedGrid() == m_grid)
    {
        gui::actions::GridActions* ga = ob->actions()->gridActions();
        ga->cellEditor(m_row, m_column)->removeChild(m_position);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void history::layouts::RemoveChild::_rollback(core::Editor* ob)
{
    sad::layouts::Cell* cell = m_grid->cell(m_row, m_column);
    cell->insertChild(m_position, m_node->MajorId);

    gui::actions::GridActions* ga = ob->actions()->gridActions();
    ga->insertNodeToGridEntry(m_node, m_grid);
    
    ga->updateParentGridsRecursively(m_grid);
    ga->tryUpdateRegionsInChildren(m_grid);

    if (ob->shared()->selectedGrid() == m_node)
    {
        ga->updateRegion(true);
    }

    if (ob->shared()->selectedGrid() == m_grid)
    {
        ga->updateRegion(true);
        ga->cellEditor(m_row, m_column)->insertChild(m_node, m_position);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void history::layouts::RemoveChild::_commitWithoutUpdatingUI(core::Editor* ob)
{
    m_grid->cell(m_row, m_column)->removeChild(m_position);   

    gui::actions::GridActions* ga = ob->actions()->gridActions();
    ga->eraseNodeToGridEntry(m_node);

    ga->updateParentGridsRecursively(m_grid);
    ga->tryUpdateRegionsInChildren(m_grid);
    
    if (ob->shared()->selectedGrid() == m_grid)
    {
        ga->updateRegion(true);
    }

}
