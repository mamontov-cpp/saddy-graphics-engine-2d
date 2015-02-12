#include "instancesnamesetter.h"

#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../history/instances/instanceschangename.h"

Q_DECLARE_METATYPE(sad::animations::Instance*);

scripting::instances::NameSetter::NameSetter(
		QScriptEngine* e
) : scripting::AbstractSetter<sad::animations::Instance*, sad::String>(e, "name")
{
	m_matched_property_names << "name";
}

scripting::instances::NameSetter::~NameSetter()
{
		
}

void scripting::instances::NameSetter::setProperty(
	sad::animations::Instance* obj, 
	const sad::String&, 
	sad::String oldvalue,  
	sad::String newvalue
)
{
	QScriptValue main = this->engine()->globalObject().property("---");
	scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
	MainPanel* panel = e->panel();
	core::Editor* editor =  panel->editor();

	int row = panel->findInList<sad::animations::Instance*>(panel->UI()->lstAnimationInstances, obj);

	if (row > - 1)
	{
		history::Command* c = new history::instances::ChangeName(obj, row, oldvalue, newvalue);								
		editor->currentBatchCommand()->add(c);
		c->commit(editor);
	}
}
