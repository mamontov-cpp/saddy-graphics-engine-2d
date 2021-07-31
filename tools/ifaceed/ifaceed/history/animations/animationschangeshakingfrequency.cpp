#include "animationschangeshakingfrequency.h"

#include "../../core/editor.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"

#include "../../blockedclosuremethodcall.h"

#include <QSpinBox>

history::animations::ChangeShakingFrequency::ChangeShakingFrequency(
    sad::animations::Animation* d,
    int old_value,
    int new_value

)
: history::animations::ChangeProperty<int>(
      d,
      "frequency",
      old_value,
      new_value
)
{
}

history::animations::ChangeShakingFrequency::~ChangeShakingFrequency()
{

}

void history::animations::ChangeShakingFrequency::updateUI(core::Editor* e, const int& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiAnimationBlock()->sbCameraShakingFrequency,
            &QSpinBox::setValue,
            value
        )
    );
}