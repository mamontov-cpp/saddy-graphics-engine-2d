/** \file hashbasedserializablecontainer.h
	\author HiddenSeeker

	Contains hash-based serializable container
 */
#include "serializablecontainer.h"
#include "sadhash.h"
#include "sadstring.h"

/**
 * Hash based serializable container is used  to store data and assign each garanteed unique ID
 */
class HashBasedSerializableContainer: public SerializableContainer
{
 protected:
	sad::Hash<sad::String, SerializableObject *> m_container; //!< Container, used to store all of data
	sad::Hash<SerializableObject *, sad::String> m_reverse_container; //!< Container, for reverse lookup of data
	
	sad::Hash<sad::String, SerializableObject *>::iterator m_iterator; //!< Iterator, used to iterate through container
	
public:
	/** Creates a human-readable unique id
	 */
	sad::String random_uid(); 
	 /** Constructs new empty container
	  */
	HashBasedSerializableContainer();

	/** Returns uid by object
		\param[in] obj object data
		\return string, empty if not found
	 */
	const sad::String & uid(SerializableObject * obj) const;

	/** Returns object by uid
		\param[in] uid unique id
		\return object (NULL if not found)
	 */
	SerializableObject * object(const sad::String & uid) const;
	/** Sets uid for object
		\param[in] obj object data
		\param[in] uid uid data
	 */
	void setUid(SerializableObject * obj, const sad::String & uid);
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
	virtual SerializableObject * begin();
	/*! Returns next iteration
	    \return next object from container , null on end of container 
	 */
	virtual SerializableObject * next();
	/** Does nothing
	 */
	~HashBasedSerializableContainer();
};
