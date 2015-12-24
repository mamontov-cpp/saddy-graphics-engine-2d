#include "instanceschangename.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QLineEdit>

#include "../../core/editor.h"

#include "../../qstdstring.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationinstanceactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"


history::instances::ChangeName::ChangeName(
    sad::animations::Instance* d,
    int position, 
    const sad::String& oldvalue,
    const sad::String& newvalue

)
: history::instances::ChangeProperty<sad::String>(
      d,
      "name",
      oldvalue,
      newvalue
), m_position(position)
{
}

history::instances::ChangeName::~ChangeName()
{

}

void history::instances::ChangeName::updateItem(core::Editor* e, const sad::String&)
{
    e->uiBlocks()->uiAnimationInstanceBlock()->lstAnimationInstances->item(m_position)->setText(
        e->actions()->instanceActions()->nameForInstance(m_animation)
    );
}

void history::instances::ChangeName::tryUpdateUI(core::Editor* e, const sad::String& value)
{
    this->history::instances::ChangeProperty<sad::String>::tryUpdateUI(e, value);
    e->emitClosure(bind(this, &history::instances::ChangeName::updateItem, e, value));
    e->emitClosure(bind(e->actions()->instanceActions(), &gui::actions::AnimationInstanceActions::updateGroupInstanceList));	
}

void history::instances::ChangeName::updateUI(core::Editor* e, const sad::String& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiAnimationInstanceBlock()->txtAnimationInstanceName,
            &QLineEdit::setText,
            STD2QSTRING(value)
        )
    );
}
