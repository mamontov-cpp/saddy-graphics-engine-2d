#include "animationschangeblinkingfrequency.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

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
            e->panel()->UI()->sbBlinkingFrequency,
            &QSpinBox::setValue,
            value
        )
    );
}
