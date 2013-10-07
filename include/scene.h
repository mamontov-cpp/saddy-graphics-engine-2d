/*! \file   scene.h
    \author HiddenSeeker
    \brief  Here placed a scene files
*/
#include "sadvector.h"
#include "sadhash.h"
#include "temporarilyimmutablecontainer.h"
#include "os/mutex.h"
#include "primitives/object.h"
#include <assert.h>
#pragma once

namespace sad
{

class Renderer;
/*! Basic objects of a scene
*/
class BasicNode: public sad::Object
{
 SAD_OBJECT
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
class Scene: public sad::TemporarilyImmutableContainer<sad::BasicNode>
{
protected:
	sad::Vector<BasicNode *>   m_layers;                 //!< Layers
	sad::Camera      *        m_camera;                  //!< Current camera
	sad::Renderer    *        m_renderer;                //!< Renderer pointer, only set when rendering
	/*! Defines a behaviour on node removal. Default is destructing some node, freeing memory from it
		\param[in] node node to be removed
	 */
	virtual void onNodeRemoval(sad::BasicNode * node);
	/*! Adds an object to scene
		\param[in] node 
	 */
	virtual void addNow(sad::BasicNode * node);
	/*! Removes object from scene
		\param[in] node
	 */
	virtual void removeNow(sad::BasicNode * node);
	/*! Clears a scene
	 */
	virtual void clearNow();
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
	/*! Renders a scene
	*/
	virtual void render();
	/*! Returns amount of scene objects
		\return objects amount
	*/
	inline unsigned long objectCount() { return m_layers.count(); }
};

}
