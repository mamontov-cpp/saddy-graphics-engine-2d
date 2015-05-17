#include "animationschangecamerapivot.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeCameraPivot::ChangeCameraPivot(
    sad::animations::Animation* d,
    const sad::Point3D& oldvalue,
    const sad::Point3D& newvalue

)
: history::animations::ChangeProperty<sad::Point3D>(
      d,
      "pivot",
      oldvalue,
      newvalue
)
{
}

history::animations::ChangeCameraPivot::~ChangeCameraPivot()
{

}

void history::animations::ChangeCameraPivot::updateUI(core::Editor* e, const sad::Point3D& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->dsbCameraRotationPivotX,
            &QDoubleSpinBox::setValue,
            value.x()
        )
    );
	 e->emitClosure( blocked_bind(
            e->panel()->UI()->dsbCameraRotationPivotY,
            &QDoubleSpinBox::setValue,
            value.y()
        )
    );
}
