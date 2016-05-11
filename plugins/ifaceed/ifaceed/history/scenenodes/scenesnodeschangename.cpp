#include "scenenodeschangename.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QLineEdit>

#include "../../core/editor.h"

#include "../../qstdstring.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uisceneblock.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/labelactions.h"
#include "../../gui/actions/sceneactions.h"
#include "../../gui/actions/scenenodeactions.h"

Q_DECLARE_METATYPE(sad::db::Object*) //-V566

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
    if (m_node->scene() == e->actions()->sceneActions()->currentScene())
    {
        e->emitClosure(bind(e->actions()->sceneNodeActions(), &gui::actions::SceneNodeActions::updateSceneNodeName, m_node));
    }
    e->emitClosure( bind(this, &history::scenenodes::ChangeName::updateDependent, e));
}

void history::scenenodes::ChangeName::updateUI(core::Editor* e, const sad::String& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiSceneBlock()->txtSceneName,
            &QLineEdit::setText,
            STD2QSTRING(value.c_str())
        )
    );
}

// ReSharper disable once CppMemberFunctionMayBeConst
void history::scenenodes::ChangeName::updateDependent(core::Editor * e)
{
    gui::actions::SceneNodeActions* sn_actions = e->actions()->sceneNodeActions();
    QComboBox* c = e->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject;
    int pos = sn_actions->findInComboBox<sad::db::Object*>(c, m_node);
    if (pos > - 1)
    {
        c->setItemText(pos, sn_actions->fullNameForNode(m_node));
    }
}