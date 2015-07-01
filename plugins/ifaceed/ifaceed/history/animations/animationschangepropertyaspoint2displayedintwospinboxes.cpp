#include "animationschangepropertyaspoint2displayedintwospinboxes.h"
#include <blockedclosuremethodcall.h>

history::animations
::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes
::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes(
        sad::animations::Animation* d,
        const sad::String& property,
        const sad::Point2D& oldvalue,
        const sad::Point2D& newvalue,
        QDoubleSpinBox* xwidget,
        QDoubleSpinBox* ywidget
) : history::animations::ChangeProperty<sad::Point2D>(d, property, oldvalue, newvalue),
m_xwidget(xwidget),
m_ywidget(ywidget)
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
            m_xwidget,
            &QDoubleSpinBox::setValue,
            value.x()
        )
    );
     e->emitClosure( blocked_bind(
            m_ywidget,
            &QDoubleSpinBox::setValue,
            value.y()
        )
    );
}
