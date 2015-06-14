#include "animationschangeresizeendingsize.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeResizeEndingSize::ChangeResizeEndingSize(
    sad::animations::Animation* d,
    const sad::Point2D& oldvalue,
    const sad::Point2D& newvalue

)
: history::animations::ChangeProperty<sad::Point2D>(
      d,
      "end_size",
      oldvalue,
      newvalue
)
{
}

history::animations::ChangeResizeEndingSize::~ChangeResizeEndingSize()
{

}

void history::animations::ChangeResizeEndingSize::updateUI(core::Editor* e, const sad::Point2D& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->dabResizeEndingSizeX,

            &QDoubleSpinBox::setValue,
            value.x()
        )
    );
	e->emitClosure( blocked_bind(
            e->panel()->UI()->dabResizeEndingSizeY,

            &QDoubleSpinBox::setValue,
            value.y()
        )
    );
}
