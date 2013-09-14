#include "movecommand.h"
#include "../objects/abstractscreenobject.h"
#include "../objects/screentemplate.h"
#include "../editorcore/editor.h"

MoveCommand::MoveCommand(AbstractScreenObject * object, const hPointF & oldp, const hPointF & newp)
{
	SL_SCOPE("MoveCommand::MoveCommand");
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
	ob->submitEvent("MoveCommand::commit", sad::Variant(0));
}

void MoveCommand::rollback(ActionContext *c, CommandChangeObserver * ob)
{
	m_object->moveCenterTo(m_old_point);
	ob->submitEvent("MoveCommand::rollback", sad::Variant(0));
}


ResizeCommand::ResizeCommand(AbstractScreenObject * object, const hRectF & or, const hRectF & nr, float a)
{
	SL_SCOPE("ResizeCommand::ResizeCommand");
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


MakeBackgroundCommand::MakeBackgroundCommand(AbstractScreenObject * object)
{
	SL_SCOPE("MakeBackgroundCommand::MakeBackgroundCommand");
	m_o = object;
	m_layer = m_o->prop<unsigned int>("layer", sad::Log::ref());
	m_rect = m_o->prop<hRectF>("rect", sad::Log::ref());
	m_angle =  m_o->prop<float>("angle", sad::Log::ref());
}

void MakeBackgroundCommand::commit(ActionContext *c, CommandChangeObserver * ob )
{
	m_o->setProp<unsigned int>("layer", 0, sad::Log::ref());
	m_o->setProp<float>("angle", 0.0f, sad::Log::ref());
	m_o->setProp<hRectF>("rect", hRectF(hPointF(0,0), hPointF(WINDOW_WIDTH, WINDOW_HEIGHT)), sad::Log::ref());	
	ob->submitEvent("MakeBackgroundCommand::commit", sad::Variant(0));
}

void MakeBackgroundCommand::rollback(ActionContext *c, CommandChangeObserver * ob )
{
	m_o->setProp<hRectF>("rect", m_rect, sad::Log::ref());	
	m_o->setProp<float>("angle", m_angle, sad::Log::ref());	
	m_o->setProp<unsigned int>("layer", m_layer, sad::Log::ref());	
	
	ob->submitEvent("MakeBackgroundCommand::rollback", sad::Variant(0));
}


MakeBackgroundCommand::~MakeBackgroundCommand()
{

}


