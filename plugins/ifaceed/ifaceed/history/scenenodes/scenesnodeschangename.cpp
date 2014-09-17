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

)
: history::scenenodes::PropertyChanged<sad::String>(
      d,
      "name",
      oldvalue,
      newvalue
)
{
}

history::scenenodes::ChangeName::~ChangeName()
{

}

void history::scenenodes::ChangeName::tryUpdateUI(core::Editor* e, const sad::String& value)
{
    this->history::scenenodes::PropertyChanged<sad::String>::tryUpdateUI(e, value);
	if (m_node->scene() == e->panel()->currentScene())
	{
		e->emitClosure(bind(e->panel(), &MainPanel::updateSceneNodeName, m_node));
	}
}

void history::scenenodes::ChangeName::updateUI(core::Editor* e, const sad::String& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->txtSceneName,
            &QLineEdit::setText,
            QString(value.c_str())
        )
    );
}
