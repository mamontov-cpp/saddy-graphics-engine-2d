#include "animationschangeblinkingfrequency.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QDoubleSpinBox>

#include "../../core/editor.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeBlinkingFrequency::ChangeBlinkingFrequency(
    sad::animations::Animation* d,
    unsigned int old_value,
    unsigned int new_value

)
: history::animations::ChangeProperty<unsigned int>(
      d,
      "frequency",
      old_value,
      new_value
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
