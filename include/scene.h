/*! \file   scene.h
    \author HiddenSeeker
    \brief  Here placed a scene files
*/
#include "templates/hlvector.hpp"
#include "templates/hhash.hpp"
#include "os/mutex.h"
#include "typeindex.h"
#include <assert.h>
#pragma once

namespace sad
{

class Renderer;
/*! Basic objects of a scene
*/
class BasicNode
{
 SAD_NODE
protected:
	BasicNode();
public:
	/*! Render procedure
	*/
	virtual void render()=0;
	/*! Destructor
	*/
	virtual ~BasicNode();
};

/*! Determines, whether it's kind of node or not
    \param[in] obj object passed object
*/
template<typename _Type>
bool isKindOf(const sad::BasicNode * obj);
/*! Camera class
*/
class Camera
{
 private:
	     float m_curX;
		 float m_curY;
		 float m_curZ;

		 float m_rotX;
		 float m_rotY;
		 float m_rotZ;
		 
		 float m_angle;
 public:
	     /*! Default constructed camera
		 */
	     Camera();
		 /*! Custom camera
		     \param x    x coord
			 \param y    y coord
			 \param z    zoom
			 \param angle angle
			 \param rx    rotation of x
			 \param ry    rotation of y
			 \param rz    rotation of z
		 */
         Camera(float x, float y, float z,float angle=0, float rx=0, float ry=0, float rz=0);

		 /*! Applies a camera
		 */
		 virtual void apply();
		 
		 inline float & x() { return m_curX;}
         inline float & y() { return m_curY;}
         inline float & z() { return m_curZ;}

		 inline float & rotationX() { return m_rotX;}
         inline float & rotationY() { return m_rotY;}
         inline float & rotationZ() { return m_rotZ;}
         inline float & angle() { return m_angle;}

		 /*! Destructor
		 */
		 virtual ~Camera();
};
/*! Scene class
*/
class Scene
{
private:
	bool                       m_clear;                 //!< Scene cleanup flag
	hst::vector<BasicNode *>   m_layers;                //!< Layers
	hst::hash<hst::string,unsigned long>  m_nodehash;   //!< Hash by an index
	hst::vector<BasicNode *>   m_marked;                //!< Marked for deletion nodes
	hst::vector< hst::triplet<BasicNode *,hst::string,unsigned long > >   m_toadd;    //!< Marked for addition nodes             //!< Помеченные для добавления вершины 
	sad::Camera      *        m_camera;                  //!< Current camera
	os::mutex                 m_add;                     //!< Add mutex
	os::mutex                 m_rem;                     //!< Remove mutex
	sad::Renderer    *        m_renderer;                //!< Renderer pointer, only set when rendering
	/*! Adds object from adding to main queue 
	 */
	void fireNodeAdding();
	/*! Removes objects from queue, removing it from scene
	 */
	void fireNodeRemoving();
protected:
	/*! Defines a behaviour on node removal. Default is destructing some node, freeing memory from it
		\param[in] node node to be removed
	 */
	virtual void onNodeRemoval(sad::BasicNode * node);
public:
	sad::Camera   & camera();  //!< Returns a current camera

	Scene();                   //!< Creates an empty scene
	virtual ~Scene();                  //!< Destructor
	/*! Sets a renderer
		\param[in] renderer renderer part
	 */
	inline void setRenderer(sad::Renderer * renderer)
	{
		m_renderer = renderer;
	}
	/*! Sets a camera in scene
		\param[in] camera  new camera
	*/
	inline void setCamera(Camera * camera) { delete m_camera;  m_camera=camera; }
	/** Finds a layers for node
		\param[in] node this node
		\return -1 if not found, index otherwise
     */
	int findLayer(sad::BasicNode * node);
	/** Sets a layer for node. If no node found in scene - nothing happens, if layer not found - it 
		pushes to end
		\param[in] node node data
		\param[in] layer layer number
	 */
	void setLayer(sad::BasicNode * node,unsigned int layer);
	/** Swaps to layers for nodes
		\param[in] node1 first node
		\param[in] node2 second node
	 */
	void swapLayers(sad::BasicNode * node1, sad::BasicNode * node2);
	/*! Removes all from scene.
	    DEPRECATED: use ::clear() instead, because it can be called
		            only before renderer was started.
	*/
	void clearNow();
	/*! Forces a scene to delete an object from scene
	    \param[in] what object to be deleted
	*/
	void remove(BasicNode * what);
	/*! Forces a scene to add an object to scene.
	    \param[in] node  object
      	\param[in] name  name. Creates anonymous object if empty
	    \param[in] lay   layer. -1 for the first layer
	*/
	void add(
	         BasicNode * node, 
		     const hst::string & name=hst::string(),
		     unsigned long lay=(unsigned long)-1
			);

	/*! Adds an object to scene.
	    DEPRECATED: It can be called if rendere is not started.
		Use ::add() instead
	    \param[in] node  object 
	    \param[in] name  name. Creates anonymous object, if empty
	    \param[in] lay   layer. -1 for the first layer
	*/
	void addNow(
		     BasicNode * node, 
		     const hst::string & name=hst::string(),
		     unsigned long lay=(unsigned long)-1
		    );
	/*! Forces scene to make cleanup, after rendering cycle 
	*/
	void clear();
	/*! Removes an object from scene.
	    DEPRECATED: It can be called only if renderer is not started, or not in scene.
		Use ::remove() instead.
	    \param[in] name name of object
	*/
	void removeNow(const hst::string & name);
	/*! Renders a scene
	*/
	virtual void render();
	/*! Returns an instance of object by name. 
	    \param[in] name name of object
	    \return an instance of object. Returns NULL if can't be found
	*/
	template<typename T> T * get(const hst::string & name); 
	/*! Returns amount of scene objects
		\return objects amount
	*/
	inline unsigned long objectCount() { return m_layers.count(); }
};

}

typedef sad::BasicNode BasicNode;
typedef sad::Scene     Scene;

//=================Code goes here====================

namespace sad
{
template<typename _Type>
bool isKindOf(const sad::BasicNode * obj)
{
	if (obj->type()==_Type::ID) return true;
	const std::vector<int> & from=obj->inheritedFrom();
	for (unsigned int i=0;i<from.size();i++)
		if (_Type::ID==from[i])
			return true;
	return false;
}
}
template<typename T> T * sad::Scene::get(const hst::string & name)
{
	if (!m_nodehash.contains(name)) return NULL;

	BasicNode *  ind=m_layers[m_nodehash[name]];
	assert( sad::isKindOf<T>(ind) && "Inconsistent types!");
	return static_cast<T *>(ind);
}
