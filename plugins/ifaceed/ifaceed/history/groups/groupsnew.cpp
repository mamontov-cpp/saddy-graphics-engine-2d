#include "groupsnew.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../closuremethodcall.h"

Q_DECLARE_METATYPE(sad::animations::Group*)


history::groups::New::New(sad::animations::Group* d) : m_node(d)
{
	m_node->addRef();
}

history::groups::New::~New()
{
	m_node->delRef();
}


void history::groups::New::commit(core::Editor * ob)
{
	m_node->Active = true;

	if(ob)
	{
		ob->emitClosure( bind(
			ob->panel(),
			&MainPanel::addGroupToList,
			m_node
		));
	}
}

void history::groups::New::rollback(core::Editor * ob)
{
	m_node->Active = false;

	if (ob)
	{
		ob->emitClosure( bind(
			ob->panel(),
			&MainPanel::removeLastGroupFromList
		));
	}
}


