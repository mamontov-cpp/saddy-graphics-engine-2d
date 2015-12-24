#include "wayschangename.h"

#include <QLineEdit>

#include "../../core/editor.h"

#include "../../qstdstring.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/wayactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiwayblock.h"

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
            e->actions()->wayActions(),
            &gui::actions::WayActions::updateWayName,
            this->m_way
        )
    );
}
void history::ways::ChangeName::updateUI(core::Editor* e, const sad::String& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiWayBlock()->txtWayName,
            &QLineEdit::setText,
            STD2QSTRING(value.c_str())
        )
    );
}

