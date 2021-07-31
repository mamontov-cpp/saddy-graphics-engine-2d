#include "instanceschangeobject.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationinstanceactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"

Q_DECLARE_METATYPE(sad::db::Object*) //-V566

history::instances::ChangeObject::ChangeObject(
    sad::animations::Instance* d,
    unsigned long long old_value,
    unsigned long long new_value

)
: history::instances::ChangeProperty<unsigned long long>(
      d,
      "object",
      old_value,
      new_value
)
{
}

history::instances::ChangeObject::~ChangeObject()
{

}

void history::instances::ChangeObject::updateItem(core::Editor* e, unsigned long long value)
{
    int pos = 0;
    if (value > 0)
    {
        pos = e->actions()->instanceActions()->findInComboBoxByMajorId<sad::db::Object*>(
            e->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject,
            value
        );
        if (pos < 0)
        {
            pos = 0;
        }
    }

    invoke_blocked(
        e->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject,
        &QComboBox::setCurrentIndex,
        pos
    );
}

void history::instances::ChangeObject::updateUI(core::Editor* e, const unsigned long long& value)
{
    e->emitClosure( bind(
            this,
            &history::instances::ChangeObject::updateItem,
            e,
            value
        )
    );
}
