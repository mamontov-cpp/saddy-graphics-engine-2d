#include "deletecommand.h"

DeleteCommand::DeleteCommand(ScreenTemplate * container, AbstractScreenObject * object)
{
	m_cmd = new NewCommand(container, object);
}

void DeleteCommand::commit(ActionContext *c, CommandChangeObserver * ob)
{
	m_cmd->rollback(c, ob);
}


void DeleteCommand::rollback(ActionContext *c, CommandChangeObserver * ob)
{
	m_cmd->commit(c, ob);
}

DeleteCommand::~DeleteCommand()
{
	delete m_cmd;
}
