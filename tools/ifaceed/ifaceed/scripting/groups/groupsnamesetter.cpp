/*#include "groupsnamesetter.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../scripting.h"

#include "../../core/editor.h"

#include "../../history/groups/groupschangename.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationgroupactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationsgroupblock.h"

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
    core::Editor* editor =  e->editor();
    
    gui::actions::AnimationGroupActions* ag_actions = editor->actions()->groupActions();
    gui::uiblocks::UIAnimationsGroupBlock* blk = editor->uiBlocks()->uiAnimationsGroupBlock();
    
    int row = ag_actions->findInList<sad::animations::Group*>(blk->lstAnimationsGroup, obj);

    if (row > - 1)
    {
        history::Command* c = new history::groups::ChangeName(obj, row, oldvalue, newvalue);								
        editor->currentBatchCommand()->add(c);
        c->commit(editor);
    }
}*/
