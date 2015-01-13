/*! \file abstractcobuilder.h
	\author HiddenSeeker

	Describes an abtract builder for building command of changed property for custom object from QScriptValue
 */
#pragma once
#include "../history/batchcommand.h"

#include <db/dbvariant.h>
#include <db/dbtypename.h>

#include <db/custom/customobject.h>

#include <QScriptEngine>

namespace scripting
{

/*! Describes an abtract builder for building command of changed property for custom object from QScriptValue
 */ 
class AbstractCOBuilder
{
public:
	/*! Polls, whether value for property suffice for builder
		\param[in] o object for getting property
		\param[in] name a name for property
	 */
	virtual bool poll(
		sad::db::custom::Object* o, 
		const QString& name
	) = 0;
	/*! Try to make command in case if everything is nice
		\param[in] o object
		\param[in] name a name
		\param[in] value a value
		\param[in] c command
	 */
	virtual void make(
		sad::db::custom::Object* o, 
		const QString& name, 
		const QScriptValue& value,
		history::BatchCommand* c
	) = 0;
	/*! Could be inherited
	 */
	virtual ~AbstractCOBuilder();
};

}
