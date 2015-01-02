#include "animationschangelooped.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeLooped::ChangeLooped(
    sad::animations::Animation* d,
    bool oldvalue,
    bool newvalue

)
: history::animations::ChangeProperty<bool>(
      d,
      "looped",
      oldvalue,
      newvalue
)
{
}

history::animations::ChangeLooped::~ChangeLooped()
{

}

void history::animations::ChangeLooped::updateUI(core::Editor* e, const bool& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->cbAnimationLooped,
            &QCheckBox::setCheckState,
            (value ? Qt::Checked : Qt::Unchecked)
        )
    );
}
