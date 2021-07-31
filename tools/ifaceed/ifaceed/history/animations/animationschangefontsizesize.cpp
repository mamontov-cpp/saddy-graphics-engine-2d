#include "animationschangefontsizesize.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QSpinBox>

#include "animations/animationsanimation.h"

#include "../../core/editor.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeFontSizeSize::ChangeFontSizeSize(
    sad::animations::Animation* d,
    const sad::String& property_name,
    QSpinBox* view,
    unsigned int old_value,
    unsigned int new_value

)
: history::animations::ChangeProperty<unsigned int>(
      d,
      property_name,
      old_value,
      new_value
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
