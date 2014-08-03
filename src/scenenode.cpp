#include "scenenode.h"
#include "scene.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"

DECLARE_SOBJ(sad::SceneNode);

sad::SceneNode::SceneNode() : m_active(true), m_visible(true), m_cached_layer(0), m_scene(NULL)
{

}

sad::SceneNode::~SceneNode()
{

}

static sad::db::schema::Schema* SceneNodeBasicSchema = NULL;

sad::db::schema::Schema* sad::SceneNode::basicSchema()
{
	if (SceneNodeBasicSchema == NULL)
	{
		SceneNodeBasicSchema = new sad::db::schema::Schema();
		SceneNodeBasicSchema->addParent(sad::db::Object::basicSchema());
		SceneNodeBasicSchema->add(
			"visible", 
			new sad::db::MethodPair<sad::SceneNode, bool>(
				&sad::SceneNode::visible,
				&sad::SceneNode::setVisible
			)
		);
		SceneNodeBasicSchema->add(
			"active", 
			new sad::db::MethodPair<sad::SceneNode, bool>(
				&sad::SceneNode::active,
				&sad::SceneNode::setActive
			)
		);		
		SceneNodeBasicSchema->add(
			"layer", 
			new sad::db::MethodPair<sad::SceneNode, unsigned int>(
				&sad::SceneNode::cachedLayer,
				&sad::SceneNode::setCachedLayer
			)
		);
	}
	return SceneNodeBasicSchema;
}

sad::db::schema::Schema* sad::SceneNode::schema() const
{
	return sad::SceneNode::basicSchema();
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