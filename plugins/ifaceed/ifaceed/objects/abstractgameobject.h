/*! \file abstractgameobject.h
	\author HiddenSeeker

	Describes an abstract game object
 */
#pragma once

/*! \class AbstractGameObject

	an abstract game object data
 */
class AbstractGameObject
{
 protected:
		// InterlockedScene * m_scene; //!< A scene of object
 public:
	  inline AbstractGameObject() { /*m_scene=NULL;*/ }	
	  /*! Adds an object to scene. Adds it to container to scene.
		  \param[in] scene scene
	   */
	  //virtual void addToScene(InterlockedScene * scene);
	  /*! Removes an object from scene
	   */
	  //virtual void removeFromScene();
	  /*! Destructor
	   */
	  virtual ~AbstractGameObject();
};

