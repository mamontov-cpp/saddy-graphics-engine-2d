#include "wayschangetotaltime.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::ways::ChangeTotalTime::ChangeTotalTime(
    sad::p2d::app::Way* d,
    double oldvalue,
    double newvalue

)
: history::ways::ChangeProperty<double>(
      d,
      "totaltime",
      oldvalue,
      newvalue
)
{
}

history::ways::ChangeTotalTime::~ChangeTotalTime()
{

}

void history::ways::ChangeTotalTime::updateUI(core::Editor* e, const double& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->dsbWayTotalTime,
            &QDoubleSpinBox::setValue,
            value
        )
    );
}

