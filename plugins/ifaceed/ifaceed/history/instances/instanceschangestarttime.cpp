#include "instanceschangestarttime.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::instances::ChangeStartTime::ChangeStartTime(
    sad::animations::Instance* d,
    double oldvalue,
    double newvalue

)
: history::instances::ChangeProperty<double>(
      d,
      "starttime",
      oldvalue,
      newvalue
)
{
}

history::instances::ChangeStartTime::~ChangeStartTime()
{

}


void history::instances::ChangeStartTime::updateUI(core::Editor* e, const double& value)
{
    e->emitClosure( bind(
            e->panel()->UI()->dsbAnimationInstanceStartTime,
            &QDoubleSpinBox::setValue,
            value
        )
    );
}
