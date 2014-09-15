#include "changefontname.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

history::label::ChangeFontName::ChangeFontName(
	sad::SceneNode* d,
	const sad::String& oldvalue,
	const sad::String& newvalue
	
) : m_node(d), m_oldvalue(oldvalue), m_newvalue(newvalue)
{
	m_node->addRef();
}

history::label::ChangeFontName::~ChangeFontName()
{
	m_node->delRef();
}

 void history::label::ChangeFontName::commit(core::Editor * ob)
{
	m_node->setProperty<sad::String>("font", m_newvalue);
	tryUpdateUI(ob, m_newvalue);
}

 void history::label::ChangeFontName::rollback(core::Editor * ob)
{
	m_node->setProperty<sad::String>("font", m_oldvalue);
	tryUpdateUI(ob, m_oldvalue);
}


void history::label::ChangeFontName::tryUpdateUI(core::Editor* e, const sad::String& value)
{
	if (m_node  == e->shared()->selectedObject() && e->machine()->isInState("selected"))
	{
		CLOSURE
		CLOSURE_DATA(sad::String v; gui::resourcetreewidget::ResourceTreeWidget* w;)
		CLOSURE_CODE(  
			w->setSelectedResourceName(v);
		);
		INITCLOSURE( CLSET(v, value); CLSET(w, e->panel()->UI()->rtwLabelFont);  )
		SUBMITCLOSURE(	e->emitClosure )
	}
}
