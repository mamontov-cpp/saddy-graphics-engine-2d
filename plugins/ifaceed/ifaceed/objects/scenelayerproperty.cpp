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

void SceneLayerProperty::set(const sad::Variant & value)
{
	if (this->getParent())
	{
		AbstractScreenObject * object = static_cast<AbstractScreenObject *>(this->getParent());
		if (object->scene())
		{
			object->scene()->setLayer(object,value.get<unsigned int>());
		}
		else
		{
			m_layer_deferred = value.get<unsigned int>();
		}
	}
}


sad::Variant * SceneLayerProperty::get() const
{
	if (this->getParent())
	{
		delete m_variant;
		AbstractScreenObject * object = static_cast<AbstractScreenObject *>(this->getParent());
		unsigned int layer = m_layer_deferred;
		if (object->scene()) 
		{
			layer = object->scene()->findLayer(object);
		}
		const_cast<SceneLayerProperty*>(this)->m_variant = new sad::Variant(layer);
		return m_variant;
	} 
	return m_variant;
}

sad::String SceneLayerProperty::save() const
{
	return  SaveLoadCallback<unsigned int>::save( get()->get<unsigned int> ());
}

void SceneLayerProperty::load(const sad::String & str)
{
	set(sad::Variant(SaveLoadCallback<unsigned int>::load(str)));
}



bool SceneLayerProperty::saveable() const
{
	return true;
}

