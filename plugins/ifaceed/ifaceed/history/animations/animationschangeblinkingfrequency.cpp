#include "animationschangeblinkingfrequency.h"

#include "../../core/editor.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeBlinkingFrequency::ChangeBlinkingFrequency(
    sad::animations::Animation* d,
    unsigned int oldvalue,
    unsigned int newvalue

)
: history::animations::ChangeProperty<unsigned int>(
      d,
      "frequency",
      oldvalue,
      newvalue
)
{
}

history::animations::ChangeBlinkingFrequency::~ChangeBlinkingFrequency()
{

}

void history::animations::ChangeBlinkingFrequency::updateUI(core::Editor* e, const unsigned int& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiAnimationBlock()->sbBlinkingFrequency,
            &QSpinBox::setValue,
            value
        )
    );
}
