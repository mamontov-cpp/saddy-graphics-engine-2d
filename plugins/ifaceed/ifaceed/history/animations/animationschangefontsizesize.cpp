#include "animationschangefontsizesize.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeFontSizeSize::ChangeFontSizeSize(
    sad::animations::Animation* d,
	const sad::String& propertyname,
	QSpinBox* view,
    unsigned int oldvalue,
    unsigned int newvalue

)
: history::animations::ChangeProperty<unsigned int>(
      d,
      propertyname,
      oldvalue,
      newvalue
), m_view(view)
{
}

history::animations::ChangeFontSizeSize::~ChangeFontSizeSize()
{

}

void history::animations::ChangeFontSizeSize::updateUI(core::Editor* e, const unsigned int& value)
{    
	e->emitClosure( blocked_bind(
            m_view,
            &QSpinBox::setValue,
            value
        )
    );
}
