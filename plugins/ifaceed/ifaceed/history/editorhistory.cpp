#include "editorhistory.h"
#include <3rdparty/format/format.h>
#include <log/log.h>

EditorHistory::EditorHistory(ActionContext * c)
{
	m_current = -1;
	m_c = c;
}

EditorHistory::~EditorHistory()
{
	this->clear();
}

void EditorHistory::clear()
{
	for (int i=0;i<m_commands.count();i++)
	{
		delete m_commands[i];
	}
	m_current = -1;
}

void EditorHistory::commit(CommandChangeObserver * ob)
{
	SL_SCOPE("EditorHistory::commit");
	SL_DEBUG(
		str(
			fmt::Format("At position {0} with {1} commands")
			<< m_current << m_commands.count()
		)
	);
	if (m_commands.count() && m_current!=m_commands.count()-1)
	{
		SL_DEBUG( 
			str(
				fmt::Format("Commiting command {0}")
				<< m_current + 1
			)
		);
		m_commands[m_current+1]->commit(m_c, ob);
		++m_current;
	}
	else
	{
		SL_DEBUG("Nothing to commit");
	}
}

void EditorHistory::rollback(CommandChangeObserver * ob)
{
	SL_SCOPE("EditorHistory::rollback");
	SL_DEBUG(
		str(
			fmt::Format("At position {0} with {1} commands")
			<< m_current << m_commands.count()
		)
	);
	if (m_commands.count() && m_current!=-1)
	{
		SL_DEBUG( 
			str(
				fmt::Format("Rolling back command {0}")
				<< m_current
			)
		);
		m_commands[m_current]->rollback(m_c, ob);
		--m_current;
	}
	else
	{
		SL_DEBUG("Nothing to rollback");
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
