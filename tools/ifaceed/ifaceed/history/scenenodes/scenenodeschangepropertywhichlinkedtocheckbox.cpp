#include "scenenodeschangepropertywhichlinkedtocheckbox.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../blockedclosuremethodcall.h"

history::scenenodes::ChangePropertyWhichLinkedToCheckbox::ChangePropertyWhichLinkedToCheckbox(
    QCheckBox* cb,
    sad::SceneNode* d,
    const sad::String& property,
    bool old_value,
    bool new_value
) : history::scenenodes::ChangeProperty<bool>(d, property, old_value, new_value), m_cb(cb)
{
    
}

history::scenenodes::ChangePropertyWhichLinkedToCheckbox::~ChangePropertyWhichLinkedToCheckbox()
{
    
}

void history::scenenodes::ChangePropertyWhichLinkedToCheckbox::updateUI(core::Editor* e, const bool& value)
{
    Qt::CheckState state = Qt::Unchecked;
    if (value)
    {
        state = Qt::Checked;
    }
  
    e->emitClosure( blocked_bind(m_cb, &QCheckBox::setCheckState, state) );
}
