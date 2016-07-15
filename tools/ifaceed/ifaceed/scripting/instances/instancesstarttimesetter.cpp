#include "instancesstarttimesetter.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../scripting.h"

#include "../../core/editor.h"

#include "../../history/instances/instanceschangestarttime.h"

Q_DECLARE_METATYPE(sad::animations::Instance*); //-V566

scripting::instances::StartTimeSetter::StartTimeSetter(
        QScriptEngine* e
) : scripting::AbstractSetter<sad::animations::Instance*, double>(e, "starttime")
{
    m_matched_property_names << "starttime";
}

scripting::instances::StartTimeSetter::~StartTimeSetter()
{
        
}

void scripting::instances::StartTimeSetter::setProperty(
    sad::animations::Instance* obj, 
    const sad::String&, 
    double oldvalue,  
    double newvalue
)
{
    QScriptValue main = this->engine()->globalObject().property("---");
    scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
    core::Editor* editor =  e->editor();

    history::Command* c = new history::instances::ChangeStartTime(obj, oldvalue, newvalue);								
    editor->currentBatchCommand()->add(c);
    c->commit(editor);
}
