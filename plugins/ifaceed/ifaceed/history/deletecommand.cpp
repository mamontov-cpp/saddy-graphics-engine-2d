#include "deletecommand.h"

DeleteCommand::DeleteCommand(ScreenTemplate * container, AbstractScreenObject * object)
{
	m_cmd = new NewCommand(container, object);
}

void DeleteCommand::commit(ActionContext *c)
{
	m_cmd->rollback(c);
}


void DeleteCommand::rollback(ActionContext *c)
{
	m_cmd->commit(c);
}

DeleteCommand::~DeleteCommand()
{
	delete m_cmd;
}
