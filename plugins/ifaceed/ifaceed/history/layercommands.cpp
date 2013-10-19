#include "layercommands.h"
#include "../objects/abstractscreenobject.h"
#include "../objects/screentemplate.h"
#include "../core/ifaceeditor.h"
#include <unused.h>

LayerCommand::LayerCommand(AbstractScreenObject * object, unsigned int oldlayer, unsigned int newlayer)
{
	SL_SCOPE("LayerCommand::LayerCommand");
	m_old_layer = oldlayer;
	m_new_layer = newlayer;
	m_object = object;
}

void LayerCommand::commit(CommandChangeObserver * ob )
{
	SL_SCOPE("LayerCommand::commit");
	m_object->scene()->setLayer(m_object, m_new_layer);
	ob->submitEvent("LayerCommand::commit", sad::Variant(sad::Point2D(m_old_layer, m_new_layer)));
}


void LayerCommand::rollback(CommandChangeObserver * ob )
{
	SL_SCOPE("LayerCommand::rollback");
	m_object->scene()->setLayer(m_object, m_old_layer);
	ob->submitEvent("LayerCommand::rollback", sad::Variant(sad::Point2D(m_old_layer, m_new_layer)));
}

LayerCommand::~LayerCommand()
{

}
