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

void MoveCommand::commit(ActionContext *c, CommandChangeObserver * ob )
{
	m_object->moveCenterTo(m_new_point);
	ob->submitEvent("move", sad::Variant(0));
}

void MoveCommand::rollback(ActionContext *c, CommandChangeObserver * ob)
{
	m_object->moveCenterTo(m_old_point);
	ob->submitEvent("move", sad::Variant(0));
}
