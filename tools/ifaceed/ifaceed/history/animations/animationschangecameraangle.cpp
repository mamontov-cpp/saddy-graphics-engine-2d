#include "animationschangecameraangle.h"

#include <animations/animationsanimation.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QDoubleSpinBox>

#include "../../core/editor.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeCameraAngle::ChangeCameraAngle(
    sad::animations::Animation* d,
    const sad::String& property_name,
    QDoubleSpinBox* view,
    double old_value,
    double new_value

)
: history::animations::ChangeProperty<double>(
      d,
      property_name,
      old_value,
      new_value
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
