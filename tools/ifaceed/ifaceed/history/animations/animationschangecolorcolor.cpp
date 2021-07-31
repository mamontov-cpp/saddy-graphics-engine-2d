#include "animationschangecolorcolor.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <sadstring.h>

#include "../../core/editor.h"

#include "../../gui/colorview/colorview.h"


#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../core/typeconverters/sadacolortoqcolor.h"

history::animations::ChangeColorColor::ChangeColorColor(
    sad::animations::Animation* d,
    const sad::String& property_name,
    gui::colorview::ColorView* view,
    const sad::AColor& old_value,
    const sad::AColor& new_value

)
: history::animations::ChangeProperty<sad::AColor>(
      d,
      property_name,
      old_value,
      new_value
), m_view(view)
{
}

history::animations::ChangeColorColor::~ChangeColorColor()
{

}

void history::animations::ChangeColorColor::updateUI(core::Editor* e, const sad::AColor& value)
{
    QColor color;

    core::typeconverters::SadAColorToQColor::convert(value, color);
    
    e->emitClosure( blocked_bind(
            m_view,
            &gui::colorview::ColorView::setBackgroundColor,
            color
        )
    );

    void (QWidget::*f)() = &QWidget::update;

    e->emitClosure( bind(
            m_view,
            f
        )
    );
}
