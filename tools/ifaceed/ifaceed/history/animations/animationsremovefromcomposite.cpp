#include "animationsremovefromcomposite.h"

#include "../core/editor.h"

#include "../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationactions.h"

history::animations::RemoveFromComposite::RemoveFromComposite(
    sad::animations::Composite* a, 
    unsigned long long id,
    int position
)
: m_animation(a), m_id(id), m_position(position)
{
    m_animation->addRef();
}

history::animations::RemoveFromComposite::~RemoveFromComposite()
{
    m_animation->delRef();
}

void history::animations::RemoveFromComposite::commit(core::Editor * ob)
{
    m_animation->remove(m_position);
    if (ob)
    {
        if (ob->shared()->selectedAnimation() == m_animation)
        {
            ob->actions()->animationActions()->updateCompositeList();
        }
    }
}

void history::animations::RemoveFromComposite::rollback(core::Editor * ob)
{
    m_animation->insert(m_id, m_position);
    if (ob)
    {
        if (ob->shared()->selectedAnimation() == m_animation)
        {
            ob->actions()->animationActions()->updateCompositeList();
        }
    }
}

