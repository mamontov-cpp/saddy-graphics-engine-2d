/*! \file abstractreader.h
	\author HiddenSeeker
	
	This reader is used to load a container of objects
 */
#include "serializablecontainer.h"
#pragma once

namespace serializable
{

/*! An abstract reader for files, used to load a bunch of files
 */
class AbstractReader
{
  protected:
			/*! A factory for reader. Used to produce objects
			 */
			SerializableFactory  *  m_factory;
			/*! Finds next point, where document can be read
			 */
			virtual void findReadingPoint()=0;
			/*! Implement this function to read some entry.
				\return entry entry data
			 */
			virtual SerializationEntry * nextEntry()=0;
			/*! Opens a stream data for writing
			 */ 
			virtual bool openStream()=0;
			/*! Closes a stream data for writing
			 */
			virtual void closeStream()=0;
 public:
			/*! Sets a factory for reader
				\param[in] factory factory data
			 */
		    inline AbstractReader (SerializableFactory * factory) { m_factory = factory;}

			/*! Reads an objects from streams
				\param[in] container container data
				\param[in] context   context of action
			 */
			virtual bool read(SerializableContainer * container, ActionContext * context);

			/*! At destruction, reader will delete factory
			 */
			virtual ~AbstractReader();
};


}

