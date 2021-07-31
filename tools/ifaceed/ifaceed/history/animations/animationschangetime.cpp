#include "animationschangetime.h"

#include "../../core/editor.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"

#include "../../blockedclosuremethodcall.h"

#include <QDoubleSpinBox>

history::animations::ChangeTime::ChangeTime(
    sad::animations::Animation* d,
    double old_value,
    double new_value

)
: history::animations::ChangeProperty<double>(
      d,
      "time",
      old_value,
      new_value
)
{
}

history::animations::ChangeTime::~ChangeTime()
{

}

void history::animations::ChangeTime::updateUI(core::Editor* e, const double& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiAnimationBlock()->dsbAnimationTime,
            &QDoubleSpinBox::setValue,
            value
        )
    );
}
