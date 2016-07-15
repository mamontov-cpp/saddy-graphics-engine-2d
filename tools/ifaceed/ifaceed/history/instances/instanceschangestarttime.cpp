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
            e->uiBlocks()->uiAnimationInstanceBlock()->dsbAnimationInstanceStartTime,
            &QDoubleSpinBox::setValue,
            value
        )
    );
}
