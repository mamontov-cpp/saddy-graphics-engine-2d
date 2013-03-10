#include "movecommand.h"
#include "../objects/abstractscreenobject.h"
#include "../objects/screentemplate.h"

MoveCommand::MoveCommand(AbstractScreenObject * object, const hPointF & oldp, const hPointF & newp)
{
	m_object = object;
	m_old_point = oldp;
	m_new_point = newp;
}

MoveCommand::~MoveCommand()
{

}

void MoveCommand::commit(ActionContext *c)
{
	m_object->moveCenterTo(m_new_point);
}

void MoveCommand::rollback(ActionContext *c)
{
	m_object->moveCenterTo(m_old_point);
}
