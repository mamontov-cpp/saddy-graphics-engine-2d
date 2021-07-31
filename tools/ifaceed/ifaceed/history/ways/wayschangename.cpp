#include "wayschangename.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QLineEdit>

#include "../../core/editor.h"

#include "../../qstdstring.h"

#include "../../closuremethodcall.h"
#include "../../blockedclosuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/wayactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiwayblock.h"

history::ways::ChangeName::ChangeName(
  sad::p2d::app::Way* d,
  const sad::String& old_value,
  const sad::String& new_value
)
: history::ways::ChangeProperty<sad::String>(
      d,
      "name",
      old_value,
      new_value
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
    e->emitClosure( ::bind(
            this, 
            &history::Command::blockedSetLineEditText,
            e->uiBlocks()->uiWayBlock()->txtWayName,
            STD2QSTRING(value.c_str())
        )
    );
}

