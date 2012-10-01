/** \file hashbasedserializablecontainer.h
	\author HiddenSeeker

	Contains hash-based serializable container
 */
#include "serializablecontainer.h"
#include "templates/hhash.hpp"
#include "templates/hstring.h"

/**
 * Hash based serializable container is used  to store data and assign each garanteed unique ID
 */
class HashBasedSerializableContainer: public SerializableContainer
{
 protected:
	hst::hash<hst::string, SerializableObject *> m_container; //!< Container, used to store all of data
	hst::hash<SerializableObject *, hst::string> m_reverse_container; //!< Container, for reverse lookup of data
	
	hst::hash<hst::string, SerializableObject *>::iterator m_iterator; //!< Iterator, used to iterate through container
	/** Creates a human-readable unique id
	 */
	hst::string random_uid(); 
public:
	 /** Constructs new empty container
	  */
	HashBasedSerializableContainer();

	/** Returns uid by object
		\param[in] obj object data
		\return string, empty if not found
	 */
	const hst::string & uid(SerializableObject * obj) const;

	/** Returns object by uid
		\param[in] uid unique id
		\return object (NULL if not found)
	 */
	SerializableObject * object(const hst::string & uid) const;

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
