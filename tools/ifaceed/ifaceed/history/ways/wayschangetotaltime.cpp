#include "wayschangetotaltime.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiwayblock.h"

#include "../../blockedclosuremethodcall.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QDoubleSpinBox>

history::ways::ChangeTotalTime::ChangeTotalTime(
    sad::p2d::app::Way* d,
    double old_value,
    double new_value

)
: history::ways::ChangeProperty<double>(
      d,
      "totaltime",
      old_value,
      new_value
)
{
}

history::ways::ChangeTotalTime::~ChangeTotalTime()
{

}

void history::ways::ChangeTotalTime::updateUI(core::Editor* e, const double& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiWayBlock()->dsbWayTotalTime,
            &QDoubleSpinBox::setValue,
            value
        )
    );
}

