/*! \file serializablecontainer.h
	\author HiddenSeeker

	SerializableContainer - is container, used to serialize some objects
 */
#include "serializableobject.h"
#pragma once

/*! \class SerializableContainer
	
	A container, that can be used for serialization
 */
class SerializableContainer
{
 private:

 public:

		 /*! Adds an object to container
			 \param[in] obj object
		  */
		 virtual void add(SerializableObject * obj);
		 /*! Removes an object to container
			 \param[in] obj object
		  */
		 virtual void remove(SerializableObject * obj);

		 /*! Returns a first object from container, starting new iteration
			 \return beginning of container
		  */
		 virtual SerializableObject * begin()=0;
		 /*! Returns next iteration
			 \return next object from container , null on end of container 
		  */
		 virtual SerializableObject * next()=0;
		 /*! Destructor
		  */
		 virtual ~SerializableContainer();
};
