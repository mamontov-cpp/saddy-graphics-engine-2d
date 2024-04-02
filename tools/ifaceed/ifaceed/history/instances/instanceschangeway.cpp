#include "instanceschangeway.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include <way.h>

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationinstanceactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"

Q_DECLARE_METATYPE(sad::Way*) //-V566

history::instances::ChangeWay::ChangeWay(
    sad::animations::Instance* d,
    unsigned long long old_value,
    unsigned long long new_value

)
: history::instances::ChangeProperty<unsigned long long>(
      d,
      "way",
      old_value,
      new_value
)
{
}

history::instances::ChangeWay::~ChangeWay()
{

}

void history::instances::ChangeWay::updateItem(core::Editor* e, unsigned long long value)
{
    int pos = 0;
    if (value > 0)
    {
        pos = e->actions()->instanceActions()->findInComboBoxByMajorId<sad::Way*>(
            e->uiBlocks()->uiAnimationInstanceBlock()->cmbWayAnimationInstanceWay,
            value
        );
        if (pos < 0)
        {
            pos = 0;
        }
    }

    invoke_blocked(
        e->uiBlocks()->uiAnimationInstanceBlock()->cmbWayAnimationInstanceWay,
        &QComboBox::setCurrentIndex,
        pos
    );
}

void history::instances::ChangeWay::updateUI(core::Editor* e, const unsigned long long& value)
{
    e->emitClosure( bind(
            this,
            &history::instances::ChangeWay::updateItem,
            e,
            value
        )
    );
}
