#include "animationsaddtocomposite.h"

#include "../../gui/animationactions.h"

#include "../../mainpanel.h"
#include "../core/editor.h"

#include "../closuremethodcall.h"

history::animations::AddToComposite::AddToComposite(sad::animations::Composite* a, unsigned long long id)
: m_animation(a), m_id(id)
{
	m_animation->addRef();
}

history::animations::AddToComposite::~AddToComposite()
{
	m_animation->delRef();
}

void history::animations::AddToComposite::commit(core::Editor * ob)
{
	m_animation->add(m_id);
	if (ob)
	{
		if (ob->shared()->selectedAnimation() == m_animation)
		{
			ob->panel()->animationActions()->updateCompositeList();
		}
	}
}

void history::animations::AddToComposite::rollback(core::Editor * ob)
{
	m_animation->remove(m_animation->size());
	if (ob)
	{
		if (ob->shared()->selectedAnimation() == m_animation)
		{
			ob->panel()->animationActions()->updateCompositeList();
		}
	}
}

