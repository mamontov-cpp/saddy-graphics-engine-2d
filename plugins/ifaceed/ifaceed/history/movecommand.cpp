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


ResizeCommand::ResizeCommand(AbstractScreenObject * object, const hRectF & or, const hRectF & nr, float a)
{
	m_object = object;
	m_old_rect = or;
	m_new_rect = nr;
	m_angle = a;
}

ResizeCommand::~ResizeCommand()
{

}

void ResizeCommand::commit(ActionContext *c, CommandChangeObserver * ob )
{
	m_object->setRotatedRectangle(m_new_rect, m_angle);
	ob->submitEvent("ResizeCommand::commit", sad::Variant(0));
}

void ResizeCommand::rollback(ActionContext *c, CommandChangeObserver * ob)
{
	m_object->setRotatedRectangle(m_old_rect, m_angle);
	ob->submitEvent("ResizeCommand::rollback", sad::Variant(0));
}
