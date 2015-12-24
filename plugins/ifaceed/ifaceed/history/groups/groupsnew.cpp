#include "groupsnew.h"

#include "../../core/editor.h"

#include "../../closuremethodcall.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationsgroupblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationgroupactions.h"

Q_DECLARE_METATYPE(sad::animations::Group*) //-V566


history::groups::New::New(sad::animations::Group* d) : m_node(d)
{
    m_node->addRef();
}

history::groups::New::~New()
{
    m_node->delRef();
}


void history::groups::New::commit(core::Editor * ob)
{
    m_node->Active = true;

    if(ob)
    {
        ob->emitClosure( bind(
            ob->actions()->groupActions(),
            &gui::actions::AnimationGroupActions::addGroupToList,
            m_node
        ));
    }
}

void history::groups::New::rollback(core::Editor * ob)
{
    m_node->Active = false;

    if (ob)
    {
        ob->emitClosure( bind(
            ob->actions()->groupActions(),
            &gui::actions::AnimationGroupActions::removeLastGroupFromList
        ));
    }
}


