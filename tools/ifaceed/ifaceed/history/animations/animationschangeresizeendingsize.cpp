#include "animationschangeresizeendingsize.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../blockedclosuremethodcall.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"

#include <QDoubleSpinBox>

history::animations::ChangeResizeEndingSize::ChangeResizeEndingSize(
    sad::animations::Animation* d,
    const sad::Point2D& old_value,
    const sad::Point2D& new_value

)
: history::animations::ChangeProperty<sad::Point2D>(
      d,
      "end_size",
      old_value,
      new_value
)
{
}

history::animations::ChangeResizeEndingSize::~ChangeResizeEndingSize()
{

}

void history::animations::ChangeResizeEndingSize::updateUI(core::Editor* e, const sad::Point2D& value)
{
    gui::uiblocks::UIAnimationBlock* blk = e->uiBlocks()->uiAnimationBlock();
    e->emitClosure( blocked_bind(
            blk->dabResizeEndingSizeX,
            &QDoubleSpinBox::setValue,
            value.x()
        )
    );
    e->emitClosure( blocked_bind(
            blk->dabResizeEndingSizeY,
            &QDoubleSpinBox::setValue,
            value.y()
        )
    );
}
