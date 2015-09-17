#include "groupschangename.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"
#include "../../qstdstring.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"


history::groups::ChangeName::ChangeName(
    sad::animations::Group* d,
    int position, 
    const sad::String& oldvalue,
    const sad::String& newvalue

)
: history::groups::ChangeProperty<sad::String>(
      d,
      "name",
      oldvalue,
      newvalue
), m_position(position)
{
}

history::groups::ChangeName::~ChangeName()
{

}

void history::groups::ChangeName::updateItem(core::Editor* e, const sad::String&)
{
    e->panel()->UI()->lstAnimationsGroup->item(m_position)->setText(
        e->panel()->nameForGroup(m_animation)
    );
}

void history::groups::ChangeName::tryUpdateUI(core::Editor* e, const sad::String& value)
{
    this->history::groups::ChangeProperty<sad::String>::tryUpdateUI(e, value);
    e->emitClosure(bind(this, &history::groups::ChangeName::updateItem, e, value));	
}

void history::groups::ChangeName::updateUI(core::Editor* e, const sad::String& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->txtAnimationsGroupName,
            &QLineEdit::setText,
            STD2QSTRING(value)
        )
    );
}
