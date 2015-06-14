#include "animationschangeresizestartingsize.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeResizeStartingSize::ChangeResizeStartingSize(
    sad::animations::Animation* d,
    const sad::Point2D& oldvalue,
    const sad::Point2D& newvalue

)
: history::animations::ChangeProperty<sad::Point2D>(
      d,
      "start_size",
      oldvalue,
      newvalue
)
{
}

history::animations::ChangeResizeStartingSize::~ChangeResizeStartingSize()
{

}

void history::animations::ChangeResizeStartingSize::updateUI(core::Editor* e, const sad::Point2D& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->dabResizeStartingSizeX,

            &QDoubleSpinBox::setValue,
            value.x()
        )
    );
	e->emitClosure( blocked_bind(
            e->panel()->UI()->dabResizeStartingSizeY,

            &QDoubleSpinBox::setValue,
            value.y()
        )
    );
}
