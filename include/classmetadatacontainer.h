/*! \file classmetadatacontainer.h
	\author HiddenSeeker

	Defines a container for class meta data
 */
#pragma once
#include "classmetadata.h"
#include "sadhash.h"
#include "sadmutex.h"

namespace sad
{
/*! A container for class meta dat
 */
class ClassMetaDataContainer
{
public:
	/*! Returns a reference for container
		\return reference for  container
	 */
	static sad::ClassMetaDataContainer * ref();
	/*! Returns a metadata for name. If metadata for name does not exists, creates it, with
		initialized class ame and inserts it to container. 
		\param[in] name name of class, which metadata is returned
		\param[out] created when metadata is created by container, this flag set to true 
		\return a  metadata
	 */
	sad::ClassMetaData * get(const sad::String & name, bool & created);
	/*! Returns true if class meta data container contains specified type
		\param[in] name name of class
		\return whether meta data is contained in type
	 */
	bool contains(const sad::String & name) const;
	/*! Erases a container
	 */
	~ClassMetaDataContainer();
private:
	typedef sad::Hash<sad::String, sad::ClassMetaData *> ClassMetaDataHash;
	/*! A hash with all of meta data
	 */
	ClassMetaDataHash m_container;
	/*! A lock for mutex
	 */
	::sad::Mutex m_lock;
	/*! Locked, for singleton usage
	 */
	ClassMetaDataContainer();
	/*! Locked for singleton usage
	 */
	ClassMetaDataContainer(const ClassMetaDataContainer & c);
	/*! An instance for meta data container
	 */
	static sad::ClassMetaDataContainer * m_instance;
	/*! Destroys instance of container
	 */
	static void destroyInstance();
};

}
