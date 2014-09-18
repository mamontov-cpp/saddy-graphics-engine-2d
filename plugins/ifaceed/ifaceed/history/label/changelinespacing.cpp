#include "changelinespacing.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../gui/labelactions.h"

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
            e->panel()->UI()->dsbLineSpacingRatio,
            &QDoubleSpinBox::setValue,
            static_cast<double>(value)
        )
    );
    e->panel()->labelActions()->updateRegionForLabel();
}
