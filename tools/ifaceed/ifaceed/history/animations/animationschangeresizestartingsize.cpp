#include "animationschangeresizestartingsize.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../blockedclosuremethodcall.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"

#include <QDoubleSpinBox>

history::animations::ChangeResizeStartingSize::ChangeResizeStartingSize(
    sad::animations::Animation* d,
    const sad::Point2D& old_value,
    const sad::Point2D& new_value

)
: history::animations::ChangeProperty<sad::Point2D>(
      d,
      "start_size",
      old_value,
      new_value
)
{
}

history::animations::ChangeResizeStartingSize::~ChangeResizeStartingSize()
{

}

void history::animations::ChangeResizeStartingSize::updateUI(core::Editor* e, const sad::Point2D& value)
{
    gui::uiblocks::UIAnimationBlock* blk = e->uiBlocks()->uiAnimationBlock();

    e->emitClosure( blocked_bind(
            blk->dabResizeStartingSizeX,

            &QDoubleSpinBox::setValue,
            value.x()
        )
    );
    e->emitClosure( blocked_bind(
            blk->dabResizeStartingSizeY,

            &QDoubleSpinBox::setValue,
            value.y()
        )
    );
}
