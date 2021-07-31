#include "animationschangename.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QLineEdit>

#include "../../core/editor.h"

#include "../../closuremethodcall.h"

#include "../../qstdstring.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"


history::animations::ChangeName::ChangeName(
    sad::animations::Animation* d,
    const sad::String& old_value,
    const sad::String& new_value

)
: history::animations::ChangeProperty<sad::String>(
      d,
      "name",
      old_value,
      new_value
),
m_should_update_text_field(true)
{
}


void history::animations::ChangeName::commitWithoutUpdatingUI(core::Editor* ob)
{
    if (ob)
    {
        m_should_update_text_field = false;
        this->commit(ob);
        m_should_update_text_field = true;
    }
}

history::animations::ChangeName::~ChangeName()
{

}

void history::animations::ChangeName::tryUpdateUI(core::Editor* e, const sad::String& value)
{
    this->history::animations::ChangeProperty<sad::String>::tryUpdateUI(e, value);
    e->emitClosure(::bind(e->actions()->animationActions(), &gui::actions::AnimationActions::updateAnimationName, m_animation));	
}

void history::animations::ChangeName::updateUI(core::Editor* e, const sad::String& value)
{
    if (m_should_update_text_field)
    {
        e->emitClosure( ::bind(
                this,
                &history::Command::blockedSetLineEditText,
                e->uiBlocks()->uiAnimationBlock()->txtAnimationName,
                STD2QSTRING(value.c_str())
            )
        );
    }
}
