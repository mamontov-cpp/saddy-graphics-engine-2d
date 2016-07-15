#include "scenesnamesetter.h"

#include "../scripting.h"

#include "../../core/editor.h"

#include "../../history/scenes/sceneschangename.h"

scripting::scenes::NameSetter::NameSetter(
        QScriptEngine* e
) : scripting::AbstractSetter<sad::Scene*, sad::String>(e, "name")
{
    m_matched_property_names << "name";
}

scripting::scenes::NameSetter::~NameSetter()
{
        
}

void scripting::scenes::NameSetter::setProperty(
    sad::Scene* obj, 
    const sad::String&, 
    sad::String oldvalue,  
    sad::String newvalue
)
{
    QScriptValue main = this->engine()->globalObject().property("---");
    scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
    core::Editor* editor =  e->editor();

    history::Command* c = new history::scenes::ChangeName(obj, oldvalue, newvalue);								
    editor->currentBatchCommand()->add(c);
    c->commit(editor);
}
