#include "layoutschangename.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QLineEdit>

#include "../../core/editor.h"

#include "../../qstdstring.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uilayoutblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/gridactions.h"


Q_DECLARE_METATYPE(sad::layouts::Grid*) //-V566

history::layouts::ChangeName::ChangeName(
    sad::layouts::Grid* d,
    const sad::String& old_value,
    const sad::String& new_value

) : m_grid(d), 
    m_old_value(old_value),
    m_new_value(new_value)
{
    m_grid->addRef();
}

history::layouts::ChangeName::~ChangeName()
{
    m_grid->delRef();
}

void history::layouts::ChangeName::commit(core::Editor * ob)
{
    if (ob)
    {
        m_grid->setProperty("name", m_new_value);    
        ob->emitClosure(::bind(this, &history::layouts::ChangeName::tryUpdateUIWithEditableFields, ob));
    }
}
void history::layouts::ChangeName::rollback(core::Editor * ob)
{
    if (ob)
    {
        m_grid->setProperty("name", m_old_value);
        ob->emitClosure(::bind(this, &history::layouts::ChangeName::tryUpdateUIWithEditableFields, ob));
    }
}

void history::layouts::ChangeName::commitWithoutUpdatingUI(core::Editor * e)
{
    if (e)
    {
        m_grid->setProperty("name", m_new_value);
        e->emitClosure(::bind(this, &history::layouts::ChangeName::tryUpdateUI, e));
    }
}


// ReSharper disable once CppMemberFunctionMayBeConst
void history::layouts::ChangeName::tryUpdateUI(core::Editor* e)
{
    gui::uiblocks::UILayoutBlock* blk = e->uiBlocks()->uiLayoutBlock();
    gui::actions::GridActions* a = e->actions()->gridActions();
    int itemIndex = a->findInList(blk->lstLayoutGridList, m_grid);
    QString name = a->viewableObjectName(m_grid);
    if (itemIndex > -1)
    {
        blk->lstLayoutGridList->item(itemIndex)->setText(name);
    }
    sad::layouts::Grid* g = e->shared()->selectedGrid();
    if (g != m_grid)
    {
        if (g)
        {
            sad::Maybe<sad::layouts::Grid::SearchResult> result = g->find(m_grid);
            if (result.exists())
            {
                sad::layouts::Cell* cell = g->cell(result.value().p1());
                a->cellEditor(cell->Row, cell->Col)->updateChildName(
                    result.value().p2(),
                    name
                );
            }
        }
    }
}


void history::layouts::ChangeName::tryUpdateUIWithEditableFields(core::Editor * e)
{
    this->tryUpdateUI(e);
    if (e->shared()->selectedGrid() == m_grid)
    {
        gui::uiblocks::UILayoutBlock* blk = e->uiBlocks()->uiLayoutBlock();
        QLineEdit* edit = blk->txtLayoutGridName;
        this->blockedSetLineEditText(edit, STD2QSTRING(m_grid->objectName()));
    }
}
