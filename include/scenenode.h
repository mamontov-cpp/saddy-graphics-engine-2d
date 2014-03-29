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
	/*! Returns scene activity flag, which determines, whether it should be rendered
		\return whether scene is active
	 */
	inline bool active()
	{
		return m_active;
	}
	/*! Sets cached layer value. If scene is set for node, forces node to set it's layer. 
		Note, that value will not be preserved, when a layer set after that.
		\param[in] layer a layer valye
	 */
	void setCachedLayer(unsigned int layer);
	/*! Returns cached value for scene node, if scene is not set, otherwise returns node.
		\return cached layer value
	 */
	unsigned int cachedLayer() const;
protected:
	/*! Determines, whether scene is active and should be rendered
	 */
	bool         m_active;
	/*! A scene, which node is attached to
	 */
	sad::Scene * m_scene;
	/*! A special layer cache to be set for a scene node
	 */
	unsigned int m_cached_layer;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::SceneNode)
