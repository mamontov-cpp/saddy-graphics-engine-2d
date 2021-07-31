#ifdef _MSC_VER
    #pragma warning(disable: 4506)
#endif

#include "animationschangepropertyaspoint2displayedintwospinboxes.h"
#include <blockedclosuremethodcall.h>

history::animations
::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes
::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes(
        sad::animations::Animation* d,
        const sad::String& property,
        const sad::Point2D& old_value,
        const sad::Point2D& new_value,
        QDoubleSpinBox* x_widget,
        QDoubleSpinBox* y_widget
) : history::animations::ChangeProperty<sad::Point2D>(d, property, old_value, new_value),
m_x_widget(x_widget),
m_y_widget(y_widget)
{
    
}

history::animations
::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes
::~ChangePropertyAsPoint2DDisplayedInTwoSpinboxes()
{
    
}

void 
history::animations
::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes
::updateUI(core::Editor* e, const sad::Point2D& value)
{
    e->emitClosure( blocked_bind(
            m_x_widget,
            &QDoubleSpinBox::setValue,
            value.x()
        )
    );
     e->emitClosure( blocked_bind(
            m_y_widget,
            &QDoubleSpinBox::setValue,
            value.y()
        )
    );
}
