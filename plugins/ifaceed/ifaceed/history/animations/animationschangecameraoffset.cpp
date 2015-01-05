#include "animationschangecameraoffset.h"


#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeCameraOffset::ChangeCameraOffset(
    sad::animations::Animation* d,
    const sad::Point2D& oldvalue,
    const sad::Point2D& newvalue

)
: history::animations::ChangeProperty<sad::Point2D>(
      d,
      "offset",
      oldvalue,
      newvalue
)
{
}

history::animations::ChangeCameraOffset::~ChangeCameraOffset()
{

}

void history::animations::ChangeCameraOffset::updateUI(core::Editor* e, const sad::Point2D& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->dsbCameraShakingOffsetX,
            &QDoubleSpinBox::setValue,
            value.x()
        )
    );
	 e->emitClosure( blocked_bind(
            e->panel()->UI()->dsbCameraShakingOffsetY,
            &QDoubleSpinBox::setValue,
            value.y()
        )
    );
}
