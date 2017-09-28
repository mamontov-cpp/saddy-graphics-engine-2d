#include "scenenodesflagsetter.h"

scripting::scenenodes::FlagSetter::FlagSetter(
    scripting::Scripting* scripting,
    const sad::String& property_name,
    scripting::scenenodes::FlagSetter::Callback f
) : scripting::scenenodes::AbstractSetter<bool>(scripting, property_name), m_f(f)
{

}

dukpp03::qt::Callable* scripting::scenenodes::FlagSetter::clone()
{
    return new scripting::scenenodes::FlagSetter(*this);
}

scripting::scenenodes::FlagSetter::~FlagSetter()
{
    
}

history::Command* scripting::scenenodes::FlagSetter::command(sad::SceneNode* obj, const sad::String&, bool oldvalue,  bool newvalue)
{
    return m_f(m_scripting->editor(), obj, oldvalue, newvalue);
}

