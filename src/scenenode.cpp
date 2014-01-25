#include "scenenode.h"
#include "scene.h"

DECLARE_SOBJ(sad::SceneNode);

sad::SceneNode::SceneNode() : m_active(true), m_scene(NULL)
{

}

sad::SceneNode::~SceneNode()
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
