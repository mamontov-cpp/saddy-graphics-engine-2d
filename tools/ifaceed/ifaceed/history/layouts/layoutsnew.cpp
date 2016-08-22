#include "layoutsnew.h"

#include "../../core/editor.h"

#include "../../closuremethodcall.h"

#include ".././gui/actions/actions.h"
#include ".././gui/actions/gridactions.h"

#include "../../gui/rendergrids.h"

Q_DECLARE_METATYPE(sad::db::Object*) //-V566

history::layouts::New::New(sad::layouts::Grid* d) : m_grid(d)
{
    m_grid->addRef();
}

history::layouts::New::~New()
{
    m_grid->delRef();
}

void history::layouts::New::commit(core::Editor * ob)
{
    if (!ob)
    {
        return;
    }
    m_grid->setActive(true);

    gui::actions::GridActions* g_actions = ob->actions()->gridActions();
    ob->emitClosure(::bind(
        g_actions,
        &gui::actions::GridActions::addGridToGridList,
        m_grid
    ));

    ob->renderGrids()->add(m_grid);
}


void history::layouts::New::rollback(core::Editor * ob)
{
    m_grid->setActive(false);

    gui::actions::GridActions* g_actions = ob->actions()->gridActions();
    ob->emitClosure(::bind(
        g_actions,
        &gui::actions::GridActions::removeGridFromGridList,
        m_grid
    ));

    ob->renderGrids()->remove(m_grid);
}
