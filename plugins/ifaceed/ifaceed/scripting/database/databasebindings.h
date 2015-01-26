/*! \file databasebindings.h
	\author HiddenSeeker

	Describes bindings for editing database properties
 */
#pragma once
#include <QScriptContext>
#include <QScriptEngine>

#include <sadstring.h>

#include <db/dbobject.h>

namespace scripting
{

class Scripting;

namespace database
{

/*! Adds new property
	\param[in] s scripting part
	\param[in] type a type
	\param[in] name name of property
 */
bool addProperty(scripting::Scripting* s, sad::String type, sad::String name);
/*! Removes a property from database by it's name
	\param[in] s scripting part
	\param[in] name a name for a property
 */
bool removeProperty(scripting::Scripting* s, sad::String name);
/*! Lists database properties
	\param[in] ctx context
	\param[in] engine an engine list
	\return list of strings with propeties of database
 */
QScriptValue list(QScriptContext* ctx, QScriptEngine* engine);
/*! Returns type of object
	\param[in] s scripting part
	\param[in] o object
	\return an object type
 */
sad::String type(scripting::Scripting* s, sad::db::Object* o);
/*! Lists readableProperties for a type
	\param[in] ctx context
	\param[in] engine an engine list
	\return list of strings with propeties of database
 */
QScriptValue readableProperties(QScriptContext* ctx, QScriptEngine* engine);
/*! Lists writableProperties for a type
	\param[in] ctx context
	\param[in] engine an engine list
	\return list of strings with propeties of database
 */
QScriptValue writableProperties(QScriptContext* ctx, QScriptEngine* engine);

}

}

