#include "changelinespacing.h"

#include <QDoubleSpinBox>

#include "../../core/editor.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uilabelblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"


#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::label::ChangeLineSpacing::ChangeLineSpacing(
    sad::SceneNode* d,
    float oldvalue,
    float newvalue
)
: history::scenenodes::ChangeProperty<float>(
      d,
      "linespacing",
      oldvalue,
      newvalue
)
{

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
