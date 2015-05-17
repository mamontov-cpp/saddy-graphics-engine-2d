#include "wayschangename.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::ways::ChangeName::ChangeName(
  sad::p2d::app::Way* d,
  const sad::String& oldvalue,
  const sad::String& newvalue
)
: history::ways::ChangeProperty<sad::String>(
      d,
      "name",
      oldvalue,
      newvalue
)
{
}

history::ways::ChangeName::~ChangeName()
{

}

void history::ways::ChangeName::tryUpdateUI(core::Editor* e, const sad::String& value)
{
    this->history::ways::ChangeProperty<sad::String>::tryUpdateUI(e, value);
    e->emitClosure( blocked_bind(
            e->panel(),
            &MainPanel::updateWayName,
            this->m_way
        )
    );
}
void history::ways::ChangeName::updateUI(core::Editor* e, const sad::String& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->txtWayName,
            &QLineEdit::setText,
            value.c_str()
        )
    );
}

