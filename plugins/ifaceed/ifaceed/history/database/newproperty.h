/*! \file newproperty.h
	\author HiddenSeeker

	A command of new database property
 */
#pragma once
#include "../command.h"

#include "../../gui/table/delegate.h"

#include <db/dbproperty.h>

#include <refcountable.h>

namespace history
{
	
namespace database
{
	
class NewProperty: public history::Command
{
public:
	 /*! Constructs new property for delegate
		\param[in] d delegate
	  */
	 NewProperty(gui::table::Delegate* d);
	 /*! Erases link to a property
	  */
	 virtual ~NewProperty();
	 /** Applies changes, described in command
		 \param[in] ob an observer for looking for command
	  */
	 virtual void commit(core::Editor * ob = NULL);
	 /** Reverts changes, described in command
		 \param[in] ob an observer for looking for command
	  */
	 virtual void rollback(core::Editor * ob = NULL);
protected:
	/*! A linked property copy for database
	 */
	sad::db::Property * m_property;
	/*! A delegate for removed property
	 */
	gui::table::Delegate * m_delegate;
};

}

}
