#include "animationschangename.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QLineEdit>

#include "../../core/editor.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../qstdstring.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"


history::animations::ChangeName::ChangeName(
    sad::animations::Animation* d,
    const sad::String& oldvalue,
    const sad::String& newvalue

)
: history::animations::ChangeProperty<sad::String>(
      d,
      "name",
      oldvalue,
      newvalue
)
{
}

history::animations::ChangeName::~ChangeName()
{

}

void history::animations::ChangeName::tryUpdateUI(core::Editor* e, const sad::String& value)
{
    this->history::animations::ChangeProperty<sad::String>::tryUpdateUI(e, value);
    e->emitClosure(bind(e->actions()->animationActions(), &gui::actions::AnimationActions::updateAnimationName, m_animation));	
}

void history::animations::ChangeName::updateUI(core::Editor* e, const sad::String& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiAnimationBlock()->txtAnimationName,
            &QLineEdit::setText,
            STD2QSTRING(value.c_str())
        )
    );
}
