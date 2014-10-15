
#include "wayschangeclosed.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::ways::ChangeClosed::ChangeClosed(
    sad::p2d::app::Way* d,
    bool oldvalue,
    bool newvalue

)
: history::ways::ChangeProperty<bool>(
      d,
      "closed",
      oldvalue,
      newvalue
)
{
}

history::ways::ChangeClosed::~ChangeClosed()
{

}

void history::ways::ChangeClosed::updateUI(core::Editor* e, const bool& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->cbWayClosed,
            &QCheckBox::setCheckState,
            (value) ? Qt::Checked : Qt::Unchecked
        )
    );
}

