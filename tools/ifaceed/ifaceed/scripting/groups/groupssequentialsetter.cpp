/*#include "groupssequentialsetter.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../scripting.h"

#include "../../core/editor.h"

#include "../../history/groups/groupschangesequential.h"

Q_DECLARE_METATYPE(sad::animations::Group*); //-V566

scripting::groups::SequentialSetter::SequentialSetter(
        QScriptEngine* e
) : scripting::AbstractSetter<sad::animations::Group*, bool>(e, "sequential")
{
    m_matched_property_names << "sequential";
}

scripting::groups::SequentialSetter::~SequentialSetter()
{
        
}

void scripting::groups::SequentialSetter::setProperty(
    sad::animations::Group* obj, 
    const sad::String&, 
    bool oldvalue,  
    bool newvalue
)
{
    QScriptValue main = this->engine()->globalObject().property("---");
    scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
    core::Editor* editor =  e->editor();

    history::Command* c = new history::groups::ChangeSequential(obj, oldvalue, newvalue);								
    editor->currentBatchCommand()->add(c);
    c->commit(editor);	
}*/
