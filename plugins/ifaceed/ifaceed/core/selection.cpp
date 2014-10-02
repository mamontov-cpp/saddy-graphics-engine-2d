#include "selection.h"

core::Selection::Selection() : m_editor(NULL)
{
	
}

core::Selection::~Selection()
{
	
}

void core::Selection::setEditor(core::Editor* e)
{
	m_editor = e;
}

core::Editor* core::Selection::editor() const
{
	return m_editor;
}
