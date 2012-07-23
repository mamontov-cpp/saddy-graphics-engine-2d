/*! \file interlockedcene.h
	\author HiddenSeeker

	Contains implementation of interlocked scene, that can be safe in interaction
 */
#include <scene.h>
#include <templates/hhash.hpp>
#include "rendermutex.h"
#pragma once

/*! \class SceneObjectContainer
	
	Default object container for interlocked scene
 */
class ISceneObjectContainer
{
 public:
		  /*! Default constructor
		   */
	      inline ISceneObjectContainer() {}
		  /*! Destructor, which calls clear 
		   */
		  virtual ~ISceneObjectContainer();
		  /*! Clears a container
		   */
		  virtual void clear()=0;
};

/*! \class SceneObjectContainer
	
	Non-default object container
 */
template<typename T>
class SceneObjectContainer: public ISceneObjectContainer
{
 private:
			hst::hash<T,T> m_container;
 public:
			/*! Default constructor
			 */
		    inline SceneObjectContainer() {}
			/*! Default Destructor
			 */
			virtual ~SceneObjectContainer() { clear(); }
			/*! Clears a container
			 */
			virtual void clear()
			{
				for (typename hst::hash<T,T>::iterator it=m_container.begin();it!=m_container.end();it++)
					delete it.value();
			}
			/*! Adds an object to container
				\param[in] object object
			 */
			void add(T object) 
			{
				m_container.insert(object,object);
			}
			/*! Removes an objects from container
			 */
			void remove(T object)
			{
				m_container.remove(object);
			}
};

/*! \class InterlockedScene

	A safe scene for implementation of work
 */
class InterlockedScene: public sad::Scene
{
 private:
		hst::hash<hst::string,ISceneObjectContainer *> m_containers; //!< Typed object containers
 public:
		InterlockedScene();  //!< Default constructor
		/*! Interlocked render function
		 */
		virtual void render();
		~InterlockedScene(); //!< Default destructor
		/*! Adds a new container to scene
			\param[in] string name of container
		 */
		template<typename T>
		inline void add(const hst::string & string = "default")
		{
			if (m_containers.contains(string)==false)
			{
			 m_containers.insert(string, new  SceneObjectContainer<T>() );
			}
		}
		/*! Clears a container
		 */
		void clearContainers();
		/*! Returns a container from scene
			\param[in] string name of container
		 */
		template<typename T>
		inline SceneObjectContainer<T> * get(const hst::string & string = "default")
		{
			return static_cast<  SceneObjectContainer<T> * >(m_containers[string]);
		}
};
