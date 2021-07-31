#include "animationschangerotateangle.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../blockedclosuremethodcall.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QDoubleSpinBox>

history::animations::ChangeRotateAngle::ChangeRotateAngle(
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

history::animations::ChangeRotateAngle::~ChangeRotateAngle()
{

}

void history::animations::ChangeRotateAngle::updateUI(core::Editor* e, const double& value)
{    
    e->emitClosure( blocked_bind(
            m_view,
            &QDoubleSpinBox::setValue,
            value / M_PI * 180.0
        )
    );
}
