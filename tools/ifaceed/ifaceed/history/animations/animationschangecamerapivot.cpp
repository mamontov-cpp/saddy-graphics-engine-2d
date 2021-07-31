#include "animationschangecamerapivot.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QDoubleSpinBox>

#include "../../core/editor.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeCameraPivot::ChangeCameraPivot(
    sad::animations::Animation* d,
    const sad::Point3D& old_value,
    const sad::Point3D& new_value

)
: history::animations::ChangeProperty<sad::Point3D>(
      d,
      "pivot",
      old_value,
      new_value
)
{
}

history::animations::ChangeCameraPivot::~ChangeCameraPivot()
{

}

void history::animations::ChangeCameraPivot::updateUI(core::Editor* e, const sad::Point3D& value)
{
    gui::uiblocks::UIAnimationBlock* blk = e->uiBlocks()->uiAnimationBlock();
    
    e->emitClosure( blocked_bind(
            blk->dsbCameraRotationPivotX,
            &QDoubleSpinBox::setValue,
            value.x()
        )
    );
     e->emitClosure( blocked_bind(
            blk->dsbCameraRotationPivotY,
            &QDoubleSpinBox::setValue,
            value.y()
        )
    );
}
