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
protected:
	/*! A scene, which node is attached to
	 */
	sad::Scene * m_scene;
};

}
