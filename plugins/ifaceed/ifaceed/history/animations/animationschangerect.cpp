#include "animationschangerect.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"

#include "../../core/typeconverters/sadrect2dtoqrectf.h"

history::animations::ChangeRect::ChangeRect(
    sad::animations::Animation* d,
	const sad::String& propertyname,
	gui::rectwidget::RectWidget* view,
    const sad::Rect2D& oldvalue,
    const sad::Rect2D& newvalue

)
: history::animations::ChangeProperty<sad::Rect2D>(
      d,
      propertyname,
      oldvalue,
      newvalue
), m_view(view)
{
}

history::animations::ChangeRect::~ChangeRect()
{

}

void history::animations::ChangeRect::updateUI(core::Editor* e, const sad::Rect2D& value)
{
	QRectF rect;
	core::typeconverters::SadRect2DToQRectF::convert(value, rect);

    e->emitClosure( blocked_bind(
            m_view,
            &gui::rectwidget::RectWidget::setValue,
            rect
        )
    );
}
