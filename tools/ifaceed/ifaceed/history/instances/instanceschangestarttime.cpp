#include "instanceschangestarttime.h"

#include "../../core/editor.h"

#include "../../closuremethodcall.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QDoubleSpinBox>

history::instances::ChangeStartTime::ChangeStartTime(
    sad::animations::Instance* d,
    double old_value,
    double new_value

)
: history::instances::ChangeProperty<double>(
      d,
      "starttime",
      old_value,
      new_value
)
{
}

history::instances::ChangeStartTime::~ChangeStartTime()
{

}


void history::instances::ChangeStartTime::updateUI(core::Editor* e, const double& value)
{
    e->emitClosure( bind(
            e->uiBlocks()->uiAnimationInstanceBlock()->dsbAnimationInstanceStartTime,
            &QDoubleSpinBox::setValue,
            value
        )
    );
}
