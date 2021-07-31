#include "animationsnew.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationactions.h"

history::animations::New::New(sad::animations::Animation* a) : m_animation(a)
{
    m_animation->addRef();
}

history::animations::New::~New()
{
    m_animation->delRef();
}


void history::animations::New::commit(core::Editor* ob)
{
    m_animation->Active = true;
    if (ob)
    {
        if (ob->panel())
        {
            gui::actions::AnimationActions* a_actions  = ob->actions()->animationActions();						
            ob->emitClosure( bind(a_actions, &gui::actions::AnimationActions::addAnimationToViewingLists, m_animation) );
        }
    }
}

void history::animations::New::rollback(core::Editor* ob)
{
    m_animation->Active = false;
    if (ob)
    {
        if (ob->panel())
        {
            gui::actions::AnimationActions* a_actions  = ob->actions()->animationActions();						            			
            ob->emitClosure( bind(a_actions, &gui::actions::AnimationActions::removeAnimationFromViewingLists, m_animation) );
        }
        if (ob->shared()->selectedAnimation() == m_animation)
        {
            ob->shared()->setSelectedAnimation(nullptr);
        }
    }
}
