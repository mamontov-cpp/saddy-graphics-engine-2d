/*! \file sadstringdelegate.h
	\author HiddenSeeker

	A delegate for property, which contains sad::String values
 */
#pragma once
#include "delegate.h"

#include "../../history/propertychangecommand.h"

#include <QObject>

#include <sadstring.h>

namespace gui
{
	
namespace table
{
/*! A delegate for editing values for sad::String values
 */
class SadStringDelegate: public gui::table::Delegate
{
Q_OBJECT
public:
	/*! Constructs new delegate
	 */
	SadStringDelegate();
	/*! Destroys delegate
	 */
	virtual ~SadStringDelegate();
	/*! Sets a value for a delegate. Used by commands to reset
		\param[in] v a value
	 */
	virtual void set(const sad::db::Variant& v);
public slots:
	/*! When widget changed
		\param[in] i a widget value
	 */
	void widgetChanged();
protected:
	/*! At this point delegate must create own editor, set it to table and connect delegate slots
		to it.
	 */
	virtual void makeEditor();
	/*! Disconnects slots  for own editor here
	 */ 
	virtual void disconnect();
};

}

}
