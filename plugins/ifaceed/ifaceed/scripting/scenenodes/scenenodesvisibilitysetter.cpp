#include "scenenodesvisibilitysetter.h"

#include "../../history/scenenodes/scenenodeschangevisibility.h"

scripting::scenenodes::VisibilitySetter::VisibilitySetter(QScriptEngine* e) : scripting::scenenodes::AbstractSetter<bool>(e)
{
	addMatched("visible");
}

scripting::scenenodes::VisibilitySetter::~VisibilitySetter()
{
	
}

history::Command* scripting::scenenodes::VisibilitySetter::command(sad::SceneNode* obj, const sad::String&, bool oldvalue,  bool newvalue)
{
	return history::scenenodes::changeVisibility(obj, oldvalue, newvalue);
}

