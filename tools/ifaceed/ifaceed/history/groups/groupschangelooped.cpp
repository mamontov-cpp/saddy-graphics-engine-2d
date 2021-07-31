#include "groupschangelooped.h"

#include "../../core/editor.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../blockedclosuremethodcall.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationsgroupblock.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QCheckBox>

history::groups::ChangeLooped::ChangeLooped(
    sad::animations::Group* d,
    bool old_value,
    bool new_value
)
: history::groups::ChangeProperty<bool>(
      d,
      "looped",
      old_value,
      new_value
)
{
}

history::groups::ChangeLooped::~ChangeLooped()
{

}

void history::groups::ChangeLooped::updateUI(core::Editor* e, const bool& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiAnimationsGroupBlock()->cbAnimationsGroupLooped,
            &QCheckBox::setCheckState,
            ( value ? Qt::Checked : Qt::Unchecked)
        )
    );
}
