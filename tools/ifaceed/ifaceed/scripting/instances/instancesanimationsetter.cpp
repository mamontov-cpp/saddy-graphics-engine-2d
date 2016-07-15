#include "instancesanimationsetter.h"

#include <QRadioButton>

#include <db/dbdatabase.h>

#include "../scripting.h"

#include "../../core/editor.h"

#include "../../history/instances/instanceschangeanimation.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationinstanceactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"

Q_DECLARE_METATYPE(sad::animations::Instance*); //-V566

// ===================== scripting::instances::AnimationSetter::IsAnimationResource =====================

scripting::instances::AnimationSetter::IsAnimationResource::IsAnimationResource()
{
    
}

scripting::instances::AnimationSetter::IsAnimationResource::~IsAnimationResource()
{
    
}

sad::Maybe<QString> scripting::instances::AnimationSetter::IsAnimationResource::check(const sad::String& a)
{
    sad::Maybe<QString> result;
    if (a.length())
    {
        bool valid = false;
        sad::resource::Resource* r = sad::Renderer::ref()->tree("")->root()->resource(a);
        if (r)
        {
            valid = r->metaData()->canBeCastedTo("sad::animations::Animation");
        }
        if (!valid)
        {
            QString name = a.c_str();
            result.setValue(name + " : is not a reference to an animation from resources");
        }
    }
    return result;
}

// ===================== scripting::instances::AnimationSetter =====================


scripting::instances::AnimationSetter::AnimationSetter(
        QScriptEngine* e
) : scripting::AbstractSetter<sad::animations::Instance*, sad::String>(e, "animation")
{
    m_matched_property_names << "animation";
    this->addCondition(new scripting::instances::AnimationSetter::IsAnimationResource());
}

scripting::instances::AnimationSetter::~AnimationSetter()
{
        
}

void scripting::instances::AnimationSetter::setProperty(
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
    unsigned long long majorid = obj->getProperty<unsigned long long>("animationmajorid").value();
    if (row > - 1)
    {
        history::Command* c  = NULL;
        QRadioButton* treebutton = ai_blk->rbAnimationInstanceFromTree;
        QRadioButton* dbbutton = ai_blk->rbAnimationInstanceFromDatabase;
        if (newvalue.length() == 0)
        {
            c = new history::instances::ChangeAnimation(
                obj,
                dbbutton,
                treebutton,
                oldvalue,
                majorid,
                newvalue,
                majorid
            );
            
        }
        else
        {
            if (majorid > 0)
            {
                c = new history::instances::ChangeAnimation(
                    obj,
                    treebutton,
                    dbbutton,
                    oldvalue,
                    majorid,
                    newvalue,
                    majorid
                );
            }
            else
            {
                c = new history::instances::ChangeAnimation(
                    obj,
                    oldvalue,
                    newvalue
                );
            }
        }
        editor->currentBatchCommand()->add(c);
        c->commit(editor);
    }
}
