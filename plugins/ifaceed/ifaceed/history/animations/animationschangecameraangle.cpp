#include "animationschangecameraangle.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeCameraAngle::ChangeCameraAngle(
    sad::animations::Animation* d,
	const sad::String& propertyname,
	QDoubleSpinBox* view,
    double oldvalue,
    double newvalue

)
: history::animations::ChangeProperty<double>(
      d,
      propertyname,
      oldvalue,
      newvalue
), m_view(view)
{
}

history::animations::ChangeCameraAngle::~ChangeCameraAngle()
{

}

void history::animations::ChangeCameraAngle::updateUI(core::Editor* e, const double& value)
{    
	e->emitClosure( blocked_bind(
            m_view,
            &QDoubleSpinBox::setValue,
            value
        )
    );
}
