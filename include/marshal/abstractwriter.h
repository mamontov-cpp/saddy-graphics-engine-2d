/*! \file abstractwriter.h
	\author HiddenSeeker
	
	This writer is used to write a container of serializable objects
 */
#include "serializablecontainer.h"
#pragma once

namespace serializable
{

/*! An abstract writer data
 */
class AbstractWriter
{
 protected:
			/*! Implement this function to write some entry to an XML
				\param[in] entry entry data
			 */
			virtual void write(SerializationEntry * entry)=0;
			/*! Opens a stream data for writing
			 */ 
			virtual bool openStream()=0;
			/*! Closes a stream data for writing
			 */
			virtual void closeStream()=0;
 public:
			/*! Writes an objects from container to a stream
				\param[in] container container data
				\param[in] context   context of action
			 */
			virtual bool write(SerializableContainer * container);
			/*! Does nothing
			 */
			virtual ~AbstractWriter();
};

}
