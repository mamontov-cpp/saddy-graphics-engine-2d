#include "animationschangerect.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../blockedclosuremethodcall.h"

#include "../../core/typeconverters/sadrect2dtoqrectf.h"

// ReSharper disable once CppUnusedIncludeDirective
#include "../../gui/rectwidget/rectwidget.h"

history::animations::ChangeRect::ChangeRect(
    sad::animations::Animation* d,
    const sad::String& property_name,
    gui::rectwidget::RectWidget* view,
    const sad::Rect2D& old_value,
    const sad::Rect2D& new_value

)
: history::animations::ChangeProperty<sad::Rect2D>(
      d,
      property_name,
      old_value,
      new_value
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
