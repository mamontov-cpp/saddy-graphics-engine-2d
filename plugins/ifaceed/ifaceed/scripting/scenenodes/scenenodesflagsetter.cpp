#include "scenenodesflagsetter.h"

scripting::scenenodes::FlagSetter::FlagSetter(
        core::Editor* ed,
        QScriptEngine* e, 
        const QString& prop, 
        scripting::scenenodes::FlagSetter::Callback f
) : scripting::scenenodes::AbstractSetter<bool>(e), m_f(f), m_editor(ed)
{
    addMatched(prop);
}

scripting::scenenodes::FlagSetter::~FlagSetter()
{
    
}

history::Command* scripting::scenenodes::FlagSetter::command(sad::SceneNode* obj, const sad::String&, bool oldvalue,  bool newvalue)
{
    return m_f(m_editor, obj, oldvalue, newvalue);
}

