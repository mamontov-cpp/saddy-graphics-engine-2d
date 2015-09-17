#include "animationsnew.h"

#include "../../mainpanel.h"
#include "../../core/editor.h"

#include "../../closuremethodcall.h"


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
            ob->emitClosure( bind(ob->panel(), &MainPanel::addAnimationToViewingLists, m_animation) );
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
            ob->emitClosure( bind(ob->panel(), &MainPanel::removeAnimationFromViewingLists, m_animation) );
        }
        if (ob->shared()->selectedAnimation() == m_animation)
        {
            ob->shared()->setSelectedAnimation(NULL);
        }
    }
}
