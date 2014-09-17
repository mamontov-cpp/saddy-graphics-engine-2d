#include "scenenodescheckboxreferencedpropertychange.h"

#include "../../core/editor.h"

#include "../../blockedclosuremethodcall.h"

history::scenenodes::CheckboxReferencedProperyChange::CheckboxReferencedProperyChange(
	QCheckBox* (MainPanel::*method)() const,
    sad::SceneNode* d,
    const sad::String& property,
    bool oldvalue,
    bool newvalue
) : history::scenenodes::PropertyChanged<bool>(d, property, oldvalue, newvalue), m_method(method)
{
	
}

history::scenenodes::CheckboxReferencedProperyChange::~CheckboxReferencedProperyChange()
{
	
}

void history::scenenodes::CheckboxReferencedProperyChange::updateUI(core::Editor* e, const bool& value)
{
	Qt::CheckState state = Qt::Unchecked;
	if (value)
	{
		state = Qt::Checked;
	}

	QCheckBox* checkbox = (e->panel()->*m_method)();

	e->emitClosure( blocked_bind(checkbox, &QCheckBox::setCheckState, state) );
}
