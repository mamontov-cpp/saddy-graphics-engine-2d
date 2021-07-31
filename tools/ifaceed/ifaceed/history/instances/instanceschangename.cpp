#include "instanceschangename.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QLineEdit>

#include "../../core/editor.h"

#include "../../qstdstring.h"

#include "../../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationinstanceactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"


history::instances::ChangeName::ChangeName(
    sad::animations::Instance* d,
    int position, 
    const sad::String& old_value,
    const sad::String& new_value

)
: history::instances::ChangeProperty<sad::String>(
      d,
      "name",
      old_value,
      new_value
), m_position(position), m_should_update_text_field(true)
{
}

void history::instances::ChangeName::commitWithoutUpdatingUI(core::Editor* ob)
{
    m_should_update_text_field = false;
    this->commit(ob);
    m_should_update_text_field = true;
}

history::instances::ChangeName::~ChangeName()
{

}

// ReSharper disable once CppMemberFunctionMayBeConst
void history::instances::ChangeName::updateItem(core::Editor* e, const sad::String&)
{
    e->uiBlocks()->uiAnimationInstanceBlock()->lstAnimationInstances->item(m_position)->setText(
        e->actions()->instanceActions()->nameForInstance(m_animation)
    );
}

void history::instances::ChangeName::tryUpdateUI(core::Editor* e, const sad::String& value)
{
    this->history::instances::ChangeProperty<sad::String>::tryUpdateUI(e, value);
    e->emitClosure(::bind(this, &history::instances::ChangeName::updateItem, e, value));
    e->emitClosure(::bind(e->actions()->instanceActions(), &gui::actions::AnimationInstanceActions::updateGroupInstanceList));	
}

void history::instances::ChangeName::updateUI(core::Editor* e, const sad::String& value)
{
    if (m_should_update_text_field)
    {
        e->emitClosure( ::bind(
                this,
                &history::Command::blockedSetLineEditText,
                e->uiBlocks()->uiAnimationInstanceBlock()->txtAnimationInstanceName,
                STD2QSTRING(value)
            )
        );
    }
}
