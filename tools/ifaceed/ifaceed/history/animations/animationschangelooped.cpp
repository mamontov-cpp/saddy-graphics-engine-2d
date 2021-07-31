#include "animationschangelooped.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include <QCheckBox>

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeLooped::ChangeLooped(
    sad::animations::Animation* d,
    bool old_value,
    bool new_value

)
: history::animations::ChangeProperty<bool>(
      d,
      "looped",
      old_value,
      new_value
)
{
}

history::animations::ChangeLooped::~ChangeLooped()
{

}

void history::animations::ChangeLooped::updateUI(core::Editor* e, const bool& value)
{
    e->emitClosure( blocked_bind(
            e->panelAsWidget()->findChild<QCheckBox*>("cbAnimationLooped"),
            &QCheckBox::setCheckState,
            (value ? Qt::Checked : Qt::Unchecked)
        )
    );
}
