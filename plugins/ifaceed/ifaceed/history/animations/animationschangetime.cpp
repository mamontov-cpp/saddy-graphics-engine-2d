#include "animationschangetime.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeTime::ChangeTime(
    sad::animations::Animation* d,
    double oldvalue,
    double newvalue

)
: history::animations::ChangeProperty<double>(
      d,
      "time",
      oldvalue,
      newvalue
)
{
}

history::animations::ChangeTime::~ChangeTime()
{

}

void history::animations::ChangeTime::updateUI(core::Editor* e, const double& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->dsbAnimationTime,
            &QDoubleSpinBox::setValue,
            value
        )
    );
}
