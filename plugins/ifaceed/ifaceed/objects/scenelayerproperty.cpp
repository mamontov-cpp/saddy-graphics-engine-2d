#include "scenelayerproperty.h"
#include "abstractscreenobject.h"
#include "../editorcore/editor.h"
#include "marshal/saveloadcallbacks.h"

SceneLayerProperty::SceneLayerProperty()
{
	m_variant = NULL;
}

SceneLayerProperty::~SceneLayerProperty()
{
	delete m_variant;
}

void SceneLayerProperty::set(const sad::Variant & value,ActionContext * context)
{
	if (this->getParent())
	{
		context->pushAction("Entering SceneLayerProperty::set");
		AbstractScreenObject * object = static_cast<AbstractScreenObject *>(this->getParent());
		if (object->scene())
		{
			object->scene()->setLayer(object,value.get<unsigned int>(context));
		}
		else
		{
			m_layer_deferred = value.get<unsigned int>(context);
		}
		context->popAction();
	}
}


sad::Variant * SceneLayerProperty::get(ActionContext * context) const
{
	if (this->getParent())
	{
		delete m_variant;
		context->pushAction("Entering SceneLayerProperty::get");
		AbstractScreenObject * object = static_cast<AbstractScreenObject *>(this->getParent());
		unsigned int layer = m_layer_deferred;
		if (object->scene()) 
		{
			layer = object->scene()->findLayer(object);
		}
		const_cast<SceneLayerProperty*>(this)->m_variant = new sad::Variant(layer);
		context->popAction();
		return m_variant;
	} 
	return m_variant;
}

hst::string SceneLayerProperty::save(ActionContext * context) const
{
	return  SaveLoadCallback<unsigned int>::save( get(context)->get<unsigned int> (context));
}

void SceneLayerProperty::load(const hst::string & str, ActionContext * context)
{
	set(sad::Variant(SaveLoadCallback<unsigned int>::load(context,str)),context);
}



bool SceneLayerProperty::saveable() const
{
	return true;
}

