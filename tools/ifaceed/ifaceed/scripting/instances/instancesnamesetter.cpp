#include "instancesnamesetter.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <db/dbdatabase.h>

#include "../scripting.h"

#include "../../core/editor.h"

#include "../../history/instances/instanceschangename.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationinstanceactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"

Q_DECLARE_METATYPE(sad::animations::Instance*); //-V566

scripting::instances::NameSetter::NameSetter(
        QScriptEngine* e
) : scripting::AbstractSetter<sad::animations::Instance*, sad::String>(e, "name")
{
    m_matched_property_names << "name";
}

scripting::instances::NameSetter::~NameSetter()
{
        
}

void scripting::instances::NameSetter::setProperty(
    sad::animations::Instance* obj, 
    const sad::String&, 
    sad::String oldvalue,  
    sad::String newvalue
)
{
    QScriptValue main = this->engine()->globalObject().property("---");
    scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
    core::Editor* editor =  e->editor();

    gui::actions::AnimationInstanceActions* ai_actions = editor->actions()->instanceActions();
    gui::uiblocks::UIAnimationInstanceBlock* ai_blk = editor->uiBlocks()->uiAnimationInstanceBlock();	
    
    
    int row = ai_actions->findInList<sad::animations::Instance*>(ai_blk->lstAnimationInstances, obj);

    if (row > - 1)
    {
        history::Command* c = new history::instances::ChangeName(obj, row, oldvalue, newvalue);								
        editor->currentBatchCommand()->add(c);
        c->commit(editor);
    }
}
