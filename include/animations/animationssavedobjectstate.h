/*! \file animations/animationssavedobjectstate.h
	\author HiddenSeeker

	A saved object state, which could be restored, when animation is finished
 */
#pragma once
#include "../db/dbobject.h"


namespace sad
{
	
namespace animations
{
	
class SavedObjectState
{
public:
	/*! Creates new saved object state
		\param[in] o object
	 */
	SavedObjectState(sad::db::Object* o);
	/*! Can be inherited
	 */
	virtual ~SavedObjectState();
	/*! Restores last object state
	 */
	virtual void restore() = 0;
	/*! Increments references to state
	 */
	inline void increment()
	{
		++m_references;
	}
	/*! Decrements references to state
		\return whether we should delete a state (true if need to)
	 */
	inline bool decrement()
	{
		bool result = false;
		--m_references;
		if (m_references <= 0)
		{
			result = true;
			this->restore();
		}
		return result;
	}
protected:
	/*! Counts references for resetting state. A state should be restored, when references is zero
	 */
	int m_references;
	/*! An object, which state is saved
	 */
	sad::db::Object* m_object;
};

/*! Defines an object state, which could be restored as saving object property
 */
template<
	typename  T
>
class ObjectState: public sad::animations::SavedObjectState
{
public:
	/*! Creates new saved object state
		\param[in] o object
		\param[in] name name of property
		\param[in] value a value for property
	 */
	ObjectState(sad::db::Object* o, const sad::String& name, const T& value)
	: sad::animations::SavedObjectState(o), m_property_name(name), m_property_value(value)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ObjectState()
	{
		
	}
	/*! Restores last object state
	 */
	virtual void restore()
	{
		m_object->setProperty(m_property_name, m_property_value);
	}
protected:
	/*! A property name to be set, when restoring
	 */
	sad::String m_property_name;
	/*! A property value for an object
	 */
	T m_property_value;
};

}

}
