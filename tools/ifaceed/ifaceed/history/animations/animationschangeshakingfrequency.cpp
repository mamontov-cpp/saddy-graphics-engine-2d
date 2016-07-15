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
    int oldvalue,
    int newvalue

)
: history::animations::ChangeProperty<int>(
      d,
      "frequency",
      oldvalue,
      newvalue
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