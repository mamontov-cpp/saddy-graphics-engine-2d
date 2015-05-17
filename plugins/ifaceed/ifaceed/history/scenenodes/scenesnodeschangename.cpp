#include "scenenodeschangename.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../gui/labelactions.h"

Q_DECLARE_METATYPE(sad::db::Object*)

history::scenenodes::ChangeName::ChangeName(
    sad::SceneNode* d,
    const sad::String& oldvalue,
    const sad::String& newvalue

)
: history::scenenodes::ChangeProperty<sad::String>(
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
    this->history::scenenodes::ChangeProperty<sad::String>::tryUpdateUI(e, value);
	if (m_node->scene() == e->panel()->currentScene())
	{
		e->emitClosure(bind(e->panel(), &MainPanel::updateSceneNodeName, m_node));
	}
	e->emitClosure( bind(this, &history::scenenodes::ChangeName::updateDependent, e));
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

void history::scenenodes::ChangeName::updateDependent(core::Editor * e)
{
	MainPanel* p = e->panel();
	int pos = p->findInComboBox<sad::db::Object*>(p->UI()->cmbAnimationInstanceObject, m_node);
	if (pos > - 1)
	{
		p->UI()->cmbAnimationInstanceObject->setItemText(pos, p->fullNameForNode(m_node));
	}
}