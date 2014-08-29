/*! \file history/database/propertychanged.h
	\author HiddenSeeker

	Contains definitions of changed property of database.
 */
#pragma once
#include "../history.h"
#include "../gui/table/delegate.h"

namespace history
{
	
namespace database
{

/*! A command, if property value changed in database
 */
template<
	typename T
>
class PropertyChanged: public history::Command
{
public:
	/*! Constructs new object, if property value is changed in database
		\param[in] oldvalue old value of property
		\param[in] newvalue new value of property
		\param[in] d a delegate for objects
	 */
	PropertyChanged(
		const T& oldvalue,
		const T& newvalue,
		gui::table::Delegate* d
	) 
	: 
	m_oldvalue(oldvalue), 
	m_newvalue(newvalue),
	m_delegate(d)
	{
			
	}
	/*! This command can be inherited
	 */
	virtual ~PropertyChanged()
	{
		
	}
	/*! Applies changes, described in command
		\param[in] ob an observer for looking for command
	 */
	virtual void commit(core::Editor * ob = NULL)
	{
		m_delegate->set(sad::db::Variant(m_newvalue));
	}
	/*! Reverts changes, described in command
		\param[in] ob an observer for looking for command
	 */
	virtual void rollback(core::Editor * ob = NULL)
	{
		m_delegate->set(sad::db::Variant(m_oldvalue));		
	}
protected:
	/*! Old value for property 
	 */
	T m_oldvalue;
	/*! New value for property
	 */
	T m_newvalue;
	/*! A delegate a property 
	 */
	gui::table::Delegate* m_delegate;
};

}

}
