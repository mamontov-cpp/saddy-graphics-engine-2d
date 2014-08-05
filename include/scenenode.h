/*! \file   scenenode.h
	\author HiddenSeeker

	Defines a node of a scene, which is basic for any renderable part
 */
#pragma once
#include "object.h"
#include "refcountable.h"

namespace sad
{
class Scene;
class Renderer;

/*! Defines an object, which is basic for any renderable part of scene
 */
class SceneNode: public sad::Object, public sad::RefCountable
{
SAD_OBJECT
public:
	/*! Defines a basic renderable part of scene
	 */
	SceneNode();
	/*! Implement this to make object render a part of scene
	 */
	virtual void render() = 0;
	/*! Called, when renderer is changed for a scene
	 */
	virtual void rendererChanged();
	/*! Must be implemented in order to get node destroyed
	 */
	virtual ~SceneNode();
	/*! A basic schema for object
		\return a schema 
	 */
	static sad::db::schema::Schema* basicSchema();
	/*! Returns schema for an object
		\return schema
	 */
	virtual sad::db::schema::Schema* schema() const;
	/*! Sets scene 
		\param[in] scene a scene, which will render a node
	 */
	virtual void setScene(sad::Scene * scene);
	/*! Returns a scene, which will render a node
		\return scene, which will render a node
	 */
	virtual sad::Scene * scene() const;
	/*! Returns a renderer, which scene is belongs to
		\returns a renderer
	 */
	sad::Renderer * renderer() const;
	/*! Sets scene activity flag, which determines, whether it should be rendered
		\param[in] active an activity flag
	 */
	inline void setActive(bool active)
	{
		m_active = active;
	}
	/*! Returns scene node activity flag, which determines, whether it should be rendered
		\return whether scene node is active
	 */
	inline bool active()
	{
		return m_active;
	}
	/*! Sets scene visibilty flag, which determines, whether it should be rendered
		\param[in] visible a visibility flag
	 */
	inline void setVisible(bool visible)
	{
		m_visible = visible;
	}
	/*! Returns scene node visibility flag, which determines, whether it should be rendered
		\return whether scene node is visible
	 */
	inline bool visible()
	{
		return m_visible;
	}
	/*! Sets cached layer value. If scene is set for node, forces node to set it's layer. 
		Note, that value will not be preserved, when a layer set after that.
		\param[in] layer a layer valye
	 */
	void setCachedLayer(unsigned int layer);
	/*! Returns cached value for scene node, if scene is not set, otherwise returns node's layer.
		\return cached layer value
	 */
	unsigned int cachedLayer() const;
protected:
	/*! Determines, whether scene node is active and should be rendered
	 */
	bool         m_active;
	/*! Determines, whether scene node is visible and should be rendered. It's same as m_active but can be used for different purposes,
		when object is active, but hidden by somewhere else in chain of responsibility of application.
	 */ 
	bool         m_visible;
	/*! A scene, which node is attached to
	 */
	sad::Scene * m_scene;
	/*! A special layer cache to be set for a scene node
	 */
	unsigned int m_cached_layer;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::SceneNode)
