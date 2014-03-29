#include "scenenode.h"
#include "scene.h"

DECLARE_SOBJ(sad::SceneNode);

sad::SceneNode::SceneNode() : m_active(true), m_visible(true), m_cached_layer(0), m_scene(NULL)
{

}

sad::SceneNode::~SceneNode()
{

}

void sad::SceneNode::rendererChanged()
{
	
}

void sad::SceneNode::setScene(sad::Scene * scene)
{
	m_scene = scene;
}

sad::Scene * sad::SceneNode::scene() const
{
	return m_scene;
}

sad::Renderer * sad::SceneNode::renderer() const
{
	if (scene())
	{
		return scene()->renderer();
	}
	return NULL;
}

void sad::SceneNode::setCachedLayer(unsigned int layer)
{
	m_cached_layer = layer;
	if (scene())
	{
		scene()->setLayer(this, layer);
	}
}

unsigned int sad::SceneNode::cachedLayer() const
{
	if (scene())
	{
		sad::SceneNode * me = const_cast<sad::SceneNode *>(this);
		me->m_cached_layer = (unsigned int)(scene()->findLayer(me));
	}
	return m_cached_layer;
}