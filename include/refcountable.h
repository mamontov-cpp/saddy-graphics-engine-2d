/*! \file   refcountable.h
	\author HiddenSeeker
	Declares a simple reference-countable object.
*/
#pragma once
#include <new>

namespace sad
{

/*!
 * Defines a reference countable object, which behaves like intrusive pointer
 */
class RefCountable
{
public:
	/*! Creates a new reference-countable object, with
		zero references on it
	 */
	RefCountable();
	/*! Adds references to an object, increasing reference count
	 */
	virtual void addRef();
	/*! Removes references to an object, decreasing reference count
	 */
	virtual void delRef();
    /*! Returns count of refs to this object
        \return references
     */
    int refsCount() const;
	/*! A ref countable object could be inherited, so it created a behaviour
		like intrusive shared pointer
	 */
	virtual ~RefCountable();
private:
	/*! Stores amount of references to an object
	 */
	 int m_references;
};

}
