#include "scenenodesflagsetter.h"

scripting::scenenodes::FlagSetter::FlagSetter(
		QScriptEngine* e, 
		const QString& prop, 
		scripting::scenenodes::FlagSetter::Callback f
) : scripting::scenenodes::AbstractSetter<bool>(e), m_f(f)
{
	addMatched(prop);
}

scripting::scenenodes::FlagSetter::~FlagSetter()
{
	
}

history::Command* scripting::scenenodes::FlagSetter::command(sad::SceneNode* obj, const sad::String&, bool oldvalue,  bool newvalue)
{
	return m_f(obj, oldvalue, newvalue);
}

