#include "animationschangename.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../qstdstring.h"

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
    e->emitClosure(bind(e->panel(), &MainPanel::updateAnimationName, m_animation));	
}

void history::animations::ChangeName::updateUI(core::Editor* e, const sad::String& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->txtAnimationName,
            &QLineEdit::setText,
            STD2QSTRING(value)
        )
    );
}
