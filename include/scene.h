/*! \file   scene.h
    \author HiddenSeeker
    \brief  Here placed a scene files
*/
#include "templates/hlvector.hpp"
#include "templates/hhash.hpp"
#include "os/mutex.h"
#include <assert.h>
#pragma once

namespace sad
{
/*! Basic objects of a scene
*/
class BasicNode
{
private:
	float m_x;    //!< X position on a space
	float m_y;    //!< Y position on a space
protected:
	int   m_type; //!< Type of an objects (dynamic)
    
	BasicNode();
public:
	static int Type; //!< Static type
	/*! Returns a type of object
	*/
	int type() const;
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
class Scene
{
private:
	bool                       m_clear;                 //!< Scene cleanup flag
	hst::vector<BasicNode *>   m_layers;                //!< Layers
	hst::hash<hst::string,unsigned long>  m_nodehash;   //!< Hash by an index
	hst::vector<BasicNode *>   m_marked;                //!< Marked for deletion nodes
	hst::vector< hst::triplet<BasicNode *,hst::string,unsigned long > >   m_toadd;    //!< Marked for addition nodes             //!< Помеченные для добавления вершины 
	sad::Camera      *        m_camera;                  //!< Current camera
    
	os::mutex                m_add;                     //!< Add mutex
	os::mutex                m_rem;                     //!< Remove mutex
public:
	sad::Camera   & camera();  //!< Returns a current camera

	Scene();                   //!< Creates an empty scene
	~Scene();                  //!< Destructor
	/*! Sets a camera in scene
		\param[in] camera  new camera
	*/
	inline void setCamera(Camera * camera) { delete m_camera;  m_camera=camera; }
	/*! Removes all from scene.
	    DEPRECATED: use ::performCleanup() instead, because it can be called
		            only before renderer was started.
	*/
	void clear();
	/*! Forces a scene to delete an object from scene
	    \param[in] what object to be deleted
	*/
	void markForDeletion(BasicNode * what);
	/*! Forces a scene to add an object to scene.
	    \param[in] node  object
      	\param[in] name  name. Creates anonymous object if empty
	    \param[in] lay   layer. -1 for the first layer
	*/
	void markForAddition(
	                     BasicNode * node, 
		                 const hst::string & name=hst::string(),
		                 unsigned long lay=(unsigned long)-1
						);

	/*! Adds an object to scene.
	    DEPRECATED: It can be called if rendere is not started.
		Use ::markForAddition() instead
	    \param[in] node  object 
	    \param[in] name  name. Creates anonymous object, if empty
	    \param[in] lay   layer. -1 for the first layer
	*/
	void add(
		     BasicNode * node, 
		     const hst::string & name=hst::string(),
		     unsigned long lay=(unsigned long)-1
		    );
	/*! Forces scene to make cleanup. 
	*/
	void performCleanup();
	/*! Removes an object from scene.
	    DEPRECATED: It can be called only if renderer is not started.
		Use ::markForDeletion() instead.
	    \param[in] name name of object
	*/
	void remove(const hst::string & name);
	/*! Renders a scene
	*/
	void render();
	/*! Returns an instance of object by name. 
	    \param[in] name name of object
	    \return an instance of object. Returns NULL if can't be found
	*/
	template<typename T> T * get(const hst::string & name); 
};

}

typedef sad::BasicNode BasicNode;
typedef sad::Scene     Scene;

//=================Code goes here====================
template<typename T> T * sad::Scene::get(const hst::string & name)
{
	if (!m_nodehash.contains(name)) return NULL;

	BasicNode *  ind=m_layers[m_nodehash[name]];
	assert(ind->type()==T::Type && "Inconsistent types!");
	return static_cast<T *>(ind);
}
