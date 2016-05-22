#include "layoutsaddchild.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>


#include "../../core/editor.h"

#include "../../qstdstring.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uilayoutblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"
#include "../../gui/actions/gridactions.h"


// ============================= PUBLIC METHODS =============================

history::layouts::AddChild::AddChild(
    sad::layouts::Grid* d,
    size_t row,
    size_t column,
    sad::SceneNode* node,
    const sad::Rect2D& oldarea
) : m_grid(d), m_row(row), m_column(column), m_node(node), m_oldarea(oldarea)
{
    m_grid->addRef();;
    m_node->addRef();
}

history::layouts::AddChild::~AddChild()
{
    m_grid->delRef();
    m_node->delRef();
}

void history::layouts::AddChild::commit(core::Editor * ob)
{
    if (!ob)
    {
        return;
    }
    ob->emitClosure(::bind(this, &history::layouts::AddChild::_commit, ob));
}

void history::layouts::AddChild::rollback(core::Editor * ob)
{
    if (!ob)
    {
        return;
    }
    ob->emitClosure(::bind(this, &history::layouts::AddChild::_rollback, ob));
}

void history::layouts::AddChild::commitWithoutUpdatingUI(core::Editor *e)
{
    if (!e)
    {
        return;
    }
    e->emitClosure(::bind(this, &history::layouts::AddChild::_commitWithoutUpdatingUI, e));
}

// =========================== PROTECTED METHODS ===========================

void history::layouts::AddChild::_commit(core::Editor* ob)
{
    this->_commitWithoutUpdatingUI(ob);
    if (ob->shared()->selectedGrid() == m_grid)
    {
        gui::actions::GridActions* ga = ob->actions()->gridActions();
        ga->cellEditor(m_row, m_column)->addChild(m_node);
    }
}

void history::layouts::AddChild::_rollback(core::Editor* ob)
{
    sad::layouts::Cell* cell = m_grid->cell(m_row, m_column);
    size_t last_child_pos = cell->childrenCount() - 1;
    cell->removeChild(last_child_pos);
    m_node->setProperty("area", m_oldarea);
    if (ob->shared()->selectedObject() == m_node)
    {
        ob->actions()->sceneNodeActions()->updateRegionForNode();
    }
    gui::actions::GridActions* ga = ob->actions()->gridActions();
    ga->eraseNodeToGridEntry(m_node);
    if (ob->shared()->selectedGrid() == m_node)
    {
        ga->updateRegion(true);
    }

    if (ob->shared()->selectedGrid() == m_grid)
    {
        ga->updateRegion(true);
        ga->cellEditor(m_row, m_column)->removeChild(last_child_pos);
    }

	ga->updateParentGridsRecursively(m_grid);
}

void history::layouts::AddChild::_commitWithoutUpdatingUI(core::Editor* ob)
{
    m_grid->cell(m_row, m_column)->addChild(m_node->MajorId);   
    if (ob->shared()->selectedObject() == m_node)
    {
        ob->actions()->sceneNodeActions()->updateRegionForNode();
    }
    gui::actions::GridActions* ga = ob->actions()->gridActions();
    ga->insertNodeToGridEntry(m_node, m_grid);
    if (ob->shared()->selectedGrid() == m_node)
    {
        ga->updateRegion(true);
    }

    if (ob->shared()->selectedGrid() == m_grid)
    {
        ga->updateRegion(true);
    }

	ga->updateParentGridsRecursively(m_grid);
}
