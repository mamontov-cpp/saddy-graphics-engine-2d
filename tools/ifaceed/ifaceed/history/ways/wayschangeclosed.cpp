#include "wayschangeclosed.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../blockedclosuremethodcall.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiwayblock.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QCheckBox>

history::ways::ChangeClosed::ChangeClosed(
    sad::Way* d,
    bool old_value,
    bool new_value

)
: history::ways::ChangeProperty<bool>(
      d,
      "closed",
      old_value,
      new_value
)
{
}

history::ways::ChangeClosed::~ChangeClosed()
{

}

void history::ways::ChangeClosed::updateUI(core::Editor* e, const bool& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiWayBlock()->cbWayClosed,
            &QCheckBox::setCheckState,
            (value) ? Qt::Checked : Qt::Unchecked
        )
    );
}

