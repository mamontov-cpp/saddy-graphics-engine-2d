#include "layercommands.h"
#include "../objects/abstractscreenobject.h"
#include "../objects/screentemplate.h"
#include "../core/ifaceeditor.h"

#ifndef UNUSED
#ifdef GCC
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif
#endif

LayerCommand::LayerCommand(AbstractScreenObject * object, unsigned int oldlayer, unsigned int newlayer)
{
	SL_SCOPE("LayerCommand::LayerCommand");
	m_old_layer = oldlayer;
	m_new_layer = newlayer;
	m_object = object;
}

void LayerCommand::commit(UNUSED ActionContext *c, CommandChangeObserver * ob )
{
	m_object->scene()->setLayer(m_object, m_new_layer);
	ob->submitEvent("LayerCommand::commit", sad::Variant(hPointF(m_old_layer, m_new_layer)));
}


void LayerCommand::rollback(UNUSED ActionContext *c, CommandChangeObserver * ob )
{
	m_object->scene()->setLayer(m_object, m_old_layer);
	ob->submitEvent("LayerCommand::rollback", sad::Variant(hPointF(m_old_layer, m_new_layer)));
}

LayerCommand::~LayerCommand()
{

}
