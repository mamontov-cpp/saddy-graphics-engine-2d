#include "animationschangewaymovingway.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../closuremethodcall.h"
#include "../../blockedclosuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"

Q_DECLARE_METATYPE(sad::p2d::app::Way*) //-V566

history::animations::ChangeWayMovingWay::ChangeWayMovingWay(
    sad::animations::Animation* d,
    unsigned long long old_value,
    unsigned long long new_value

)
: history::animations::ChangeProperty<unsigned long long>(
      d,
      "way",
      old_value,
      new_value
)
{
}

history::animations::ChangeWayMovingWay::~ChangeWayMovingWay()
{

}

void history::animations::ChangeWayMovingWay::performUpdateUI(core::Editor* e, unsigned long long value)
{
    QComboBox* list = e->uiBlocks()->uiAnimationBlock()->cmbWayAnimationWay;
    gui::actions::AnimationActions* a_actions  = e->actions()->animationActions();
    invoke_blocked(list, &QComboBox::setCurrentIndex,  a_actions->findInComboBoxByMajorId<sad::p2d::app::Way*>(list,value));
}

void history::animations::ChangeWayMovingWay::updateUI(core::Editor* e, const unsigned long long& value)
{
    e->emitClosure( bind(
            this,
            &history::animations::ChangeWayMovingWay::performUpdateUI,
            e,
            value
        )
    );
}
