#include "groupsnamesetter.h"

#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../history/groups/groupschangename.h"

Q_DECLARE_METATYPE(sad::animations::Group*); //-V566

scripting::groups::NameSetter::NameSetter(
        QScriptEngine* e
) : scripting::AbstractSetter<sad::animations::Group*, sad::String>(e, "name")
{
    m_matched_property_names << "name";
}

scripting::groups::NameSetter::~NameSetter()
{
        
}

void scripting::groups::NameSetter::setProperty(
    sad::animations::Group* obj, 
    const sad::String&, 
    sad::String oldvalue,  
    sad::String newvalue
)
{
    QScriptValue main = this->engine()->globalObject().property("---");
    scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
    MainPanel* panel = e->panel();
    core::Editor* editor =  panel->editor();

    int row = panel->findInList<sad::animations::Group*>(panel->UI()->lstAnimationsGroup, obj);

    if (row > - 1)
    {
        history::Command* c = new history::groups::ChangeName(obj, row, oldvalue, newvalue);								
        editor->currentBatchCommand()->add(c);
        c->commit(editor);
    }
}
