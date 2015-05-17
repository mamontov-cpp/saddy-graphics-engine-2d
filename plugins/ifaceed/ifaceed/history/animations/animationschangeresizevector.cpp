#include "animationschangeresizevector.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeResizeVector::ChangeResizeVector(
    sad::animations::Animation* d,
    const sad::Point2D& oldvalue,
    const sad::Point2D& newvalue

)
: history::animations::ChangeProperty<sad::Point2D>(
      d,
      "vector",
      oldvalue,
      newvalue
)
{
}

history::animations::ChangeResizeVector::~ChangeResizeVector()
{

}

void history::animations::ChangeResizeVector::updateUI(core::Editor* e, const sad::Point2D& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->dabResizeVectorX,
            &QDoubleSpinBox::setValue,
            value.x()
        )
    );
	e->emitClosure( blocked_bind(
            e->panel()->UI()->dabResizeVectorY,
            &QDoubleSpinBox::setValue,
            value.y()
        )
    );
}
