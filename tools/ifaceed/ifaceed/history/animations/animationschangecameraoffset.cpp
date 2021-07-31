#include "animationschangecameraoffset.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QDoubleSpinBox>

#include "../../core/editor.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeCameraOffset::ChangeCameraOffset(
    sad::animations::Animation* d,
    const sad::Point2D& old_value,
    const sad::Point2D& new_value

)
: history::animations::ChangeProperty<sad::Point2D>(
      d,
      "offset",
      old_value,
      new_value
)
{
}

history::animations::ChangeCameraOffset::~ChangeCameraOffset()
{

}

void history::animations::ChangeCameraOffset::updateUI(core::Editor* e, const sad::Point2D& value)
{
    gui::uiblocks::UIAnimationBlock* blk = e->uiBlocks()->uiAnimationBlock();
    e->emitClosure( blocked_bind(
            blk->dsbCameraShakingOffsetX,
            &QDoubleSpinBox::setValue,
            value.x()
        )
    );
     e->emitClosure( blocked_bind(
            blk->dsbCameraShakingOffsetY,
            &QDoubleSpinBox::setValue,
            value.y()
        )
    );
}
