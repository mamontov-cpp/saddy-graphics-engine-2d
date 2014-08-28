#include "history.h"

#include <3rdparty/format/format.h>

#include <db/load.h>
#include <db/save.h>

#include <log/log.h>

history::History::History()
{
	m_current = -1;
}

history::History::~History()
{
	this->clear();
}

void history::History::clear()
{
	for (int i=0;i<m_commands.count();i++)
	{
		delete m_commands[i];
	}
	m_commands.clear();
	m_current = -1;
}

void history::History::commit(core::Editor * ob)
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
		m_commands[m_current+1]->commit(ob);
		++m_current;
	}
	else
	{
		SL_DEBUG("Nothing to commit");
	}
}

void history::History::rollback(core::Editor * ob)
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
		m_commands[m_current]->rollback(ob);
		--m_current;
	}
	else
	{
		SL_DEBUG("Nothing to rollback");
	}
}

void history::History::add(history::Command * c)
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
