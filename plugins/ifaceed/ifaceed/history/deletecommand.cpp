#include "deletecommand.h"
#include <log/log.h>

DeleteCommand::DeleteCommand(ScreenTemplate * container, AbstractScreenObject * object)
{
	SL_SCOPE("DeleteCommand::DeleteCommand");
	m_cmd = new NewCommand(container, object);
}

void DeleteCommand::commit(CommandChangeObserver * ob)
{
	SL_SCOPE("DeleteCommand::commit");
	m_cmd->rollback(ob);
}


void DeleteCommand::rollback(CommandChangeObserver * ob)
{
	SL_SCOPE("DeleteCommand::rollback");
	m_cmd->commit(ob);
}

DeleteCommand::~DeleteCommand()
{
	delete m_cmd;
}
