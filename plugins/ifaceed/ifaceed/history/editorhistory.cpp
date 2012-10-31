#include "editorhistory.h"

EditorHistory::EditorHistory(ActionContext * c)
{
	m_current = -1;
	m_c = c;
}

EditorHistory::~EditorHistory()
{
	for (int i=0;i<m_commands.count();i++)
	{
		delete m_commands[i];
	}
}

void EditorHistory::commit()
{
	if (m_commands.count() && m_current!=m_commands.count()-1)
	{
		m_commands[m_current+1]->commit(m_c);
		++m_current;
	}
}

void EditorHistory::rollback()
{
	if (m_commands.count() && m_current!=-1)
	{
		m_commands[m_current]->rollback(m_c);
		--m_current;
	}
}

void EditorHistory::add(AbstractCommand * c)
{
	int count = m_commands.count();
	for(int i=m_current+1;i<count;i++)
	{
		delete m_commands[m_current+1];
		m_commands.removeAt(m_current+1);
	}
	m_commands << c;
	++m_current;
}
