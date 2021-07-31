#include "changelinespacing.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QDoubleSpinBox>

#include "../../core/editor.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uilabelblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"


#include "../../blockedclosuremethodcall.h"

history::label::ChangeLineSpacing::ChangeLineSpacing(
    sad::SceneNode* d,
    float old_value,
    float new_value
)
: history::scenenodes::ChangeProperty<float>(
      d,
      "linespacing",
      old_value,
      new_value
)
{
    m_affects_parent_grid = true;
}

history::label::ChangeLineSpacing::~ChangeLineSpacing()
{

}

void history::label::ChangeLineSpacing::updateUI(
    core::Editor* e,
    const float& value
)
{    
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiLabelBlock()->dsbLineSpacingRatio,
            &QDoubleSpinBox::setValue,
            static_cast<double>(value)
        )
    );
    e->actions()->sceneNodeActions()->updateRegionForNode();
}
