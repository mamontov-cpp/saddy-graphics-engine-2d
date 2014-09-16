#include "scenenodeschangename.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../gui/labelactions.h"

history::scenenodes::ChangeName::ChangeName(
    sad::SceneNode* d,
    const sad::String& oldvalue,
    const sad::String& newvalue

) : m_node(d), m_oldvalue(oldvalue), m_newvalue(newvalue)
{
    m_node->addRef();
}

history::scenenodes::ChangeName::~ChangeName()
{
    m_node->delRef();
}

 void history::scenenodes::ChangeName::commit(core::Editor * ob)
{
    m_node->setObjectName(m_newvalue);
	tryUpdateUI(ob, m_newvalue);
}

 void history::scenenodes::ChangeName::rollback(core::Editor * ob)
{
    m_node->setObjectName(m_oldvalue);
    tryUpdateUI(ob, m_oldvalue);
}


void history::scenenodes::ChangeName::tryUpdateUI(core::Editor* e, const sad::String& value)
{
    if (m_node  == e->shared()->selectedObject() && e->machine()->isInState("selected"))
    {
        e->emitClosure( blocked_bind(
                e->panel()->UI()->txtSceneName,
                &QLineEdit::setText,
                QString(value.c_str())
            )
        );
    }
	if (m_node->scene() == e->panel()->currentScene())
	{
		e->emitClosure(bind(e->panel(), &MainPanel::updateSceneNodeName, m_node));
	}
}
