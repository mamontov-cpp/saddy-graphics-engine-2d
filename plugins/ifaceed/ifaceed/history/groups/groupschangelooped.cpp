#include "groupschangelooped.h"

#include "../../core/editor.h"
#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"


history::groups::ChangeLooped::ChangeLooped(
    sad::animations::Group* d,
    bool oldvalue,
    bool newvalue
)
: history::groups::ChangeProperty<bool>(
      d,
      "looped",
      oldvalue,
      newvalue
)
{
}

history::groups::ChangeLooped::~ChangeLooped()
{

}

void history::groups::ChangeLooped::updateUI(core::Editor* e, const bool& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->cbAnimationsGroupLooped,
            &QCheckBox::setCheckState,
            ( value ? Qt::Checked : Qt::Unchecked)
        )
    );
}
