#include "animationschangeshakingfrequency.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"

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
            e->panel()->UI()->sbCameraShakingFrequency,
            &QSpinBox::setValue,
            value
        )
    );
}