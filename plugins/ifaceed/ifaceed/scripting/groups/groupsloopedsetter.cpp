#include "groupsloopedsetter.h"

#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../history/groups/groupschangelooped.h"

Q_DECLARE_METATYPE(sad::animations::Group*); //-V566

scripting::groups::LoopedSetter::LoopedSetter(
        QScriptEngine* e
) : scripting::AbstractSetter<sad::animations::Group*, bool>(e, "looped")
{
    m_matched_property_names << "looped";
}

scripting::groups::LoopedSetter::~LoopedSetter()
{
        
}

void scripting::groups::LoopedSetter::setProperty(
    sad::animations::Group* obj, 
    const sad::String&, 
    bool oldvalue,  
    bool newvalue
)
{
    QScriptValue main = this->engine()->globalObject().property("---");
    scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
    MainPanel* panel = e->panel();
    core::Editor* editor =  panel->editor();

    history::Command* c = new history::groups::ChangeLooped(obj, oldvalue, newvalue);								
    editor->currentBatchCommand()->add(c);
    c->commit(editor);	
}
