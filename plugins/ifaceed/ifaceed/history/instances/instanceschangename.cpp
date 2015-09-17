#include "instanceschangename.h"

#include "../../core/editor.h"

#include "../../gui/instanceactions.h"

#include "../../mainpanel.h"
#include "../../qstdstring.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"


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
    e->panel()->UI()->lstAnimationInstances->item(m_position)->setText(
        e->panel()->nameForInstance(m_animation)
    );
}

void history::instances::ChangeName::tryUpdateUI(core::Editor* e, const sad::String& value)
{
    this->history::instances::ChangeProperty<sad::String>::tryUpdateUI(e, value);
    e->emitClosure(bind(this, &history::instances::ChangeName::updateItem, e, value));
    e->emitClosure(bind(e->panel()->instanceActions(), &gui::InstanceActions::updateGroupInstanceList));	
}

void history::instances::ChangeName::updateUI(core::Editor* e, const sad::String& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->txtAnimationInstanceName,
            &QLineEdit::setText,
            STD2QSTRING(value)
        )
    );
}
