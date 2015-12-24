#include "groupsremove.h"

#include "../../core/editor.h"

#include "../../closuremethodcall.h"


#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationsgroupblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationgroupactions.h"

Q_DECLARE_METATYPE(sad::animations::Group*) //-V566


history::groups::Remove::Remove(sad::animations::Group* d, int position) : m_node(d), m_position(position)
{
    m_node->addRef();
}

history::groups::Remove::~Remove()
{
    m_node->delRef();
}


void history::groups::Remove::commit(core::Editor * ob)
{
    m_node->Active = false;

    if(ob)
    {
        ob->emitClosure( bind(
            ob->actions()->groupActions(),
            &gui::actions::AnimationGroupActions::removeGroupFromList,
            m_position
        ));
    }
}

void history::groups::Remove::rollback(core::Editor * ob)
{
    m_node->Active = true;

    if (ob)
    {
        ob->emitClosure( bind(
            ob->actions()->groupActions(),
            &gui::actions::AnimationGroupActions::insertGroupToList,
            m_position,
            m_node
        ));
    }
}


