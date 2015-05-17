#include "animationschangecolorcolor.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../core/typeconverters/sadacolortoqcolor.h"

history::animations::ChangeColorColor::ChangeColorColor(
    sad::animations::Animation* d,
	const sad::String& propertyname,
	gui::colorview::ColorView* view,
    const sad::AColor& oldvalue,
    const sad::AColor& newvalue

)
: history::animations::ChangeProperty<sad::AColor>(
      d,
      propertyname,
      oldvalue,
      newvalue
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
