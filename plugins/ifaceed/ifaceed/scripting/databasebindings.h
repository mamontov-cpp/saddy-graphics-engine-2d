/*! \file databasebindings.h
	\author HiddenSeeker

	Describes bindings for editing database properties
 */
#pragma once
#include <sadstring.h>
#include <renderer.h>
#include <equalto.h>

#include <db/dbdatabase.h>

#include "tovalue.h"
#include "fromvalue.h"
#include "callable.h"
#include "scripting.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../history/database/changeproperty.h"

namespace scripting
{

class Scripting;

/*! Defines a setter for database property
 */
template<
	typename T
>
class DatabasePropertySetter: public scripting::Callable
{
public:
	/*! Represents a setter call for a data
		\param[in] e engine
	 */
	DatabasePropertySetter(QScriptEngine* e) : scripting::Callable(e, "set", 2)
	{
		
	}
	/*! Destroys a setter
	 */
	virtual ~DatabasePropertySetter()
	{
		
	}
	/*! Determines, whether it can be called with this context
		\param[in] ctx context
	 */
	virtual sad::Maybe<QString> canBeCalled(QScriptContext* ctx)
	{
		sad::Maybe<QString> result;
		checkArgumentCount(result, ctx);
		checkArgument<sad::String>(result, 0, ctx);
		checkArgument<T>(result, 1, ctx);
		if (result.exists() == false)
		{
			sad::db::TypeName<T>::init();
			sad::db::Database* me = sad::Renderer::ref()->database("");
			sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(0));
			sad::db::Property* prop = me->propertyByName(propname.value());
			if (prop)
			{
				if (prop->baseType() != sad::db::TypeName<T>::baseName() || prop->pointerStarsCount() != 0)
				{
					QString qpropname = propname.value().c_str();
					QString basetype = sad::db::TypeName<T>::baseName().c_str();
					result.setValue(QString("property ") + qpropname + QString(" is not of type ") + basetype);
				}
			}
			else
			{
				result.setValue(QString(propname.value().c_str()) + " is not writeable");
			}
		}
		return result;
	}
	/*! Calls actually a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	virtual QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(0));
		sad::Maybe<T> newvalue = scripting::ToValue<T>::perform(ctx->argument(1));

		sad::db::Database* me = sad::Renderer::ref()->database("");
		sad::Maybe<T> oldvalue = me->getProperty<T>(propname.value());
		std::equal_to<T> comparator;
		if (comparator(newvalue.value(), oldvalue.value()) == false)
		{
			MainPanel* p = static_cast<scripting::Scripting*>(engine->globalObject().property("E").toQObject())->panel();
			gui::table::Delegate* d = p->delegatesByName()[propname.value()];
			history::Command* c = new history::database::ChangeProperty<T>(oldvalue.value(), newvalue.value(), d);
			c->commit(p->editor());
			p->editor()->currentBatchCommand()->add(c);
		}
		return ctx->thisObject();
	}
};

/*! Defines a getter for database property
 */
template<
	typename T
>
class DatabasePropertyGetter: public scripting::Callable
{
public:
	/*! Represents a setter call for a data
		\param[in] e engine
	 */
	DatabasePropertyGetter(QScriptEngine* e) : scripting::Callable(e, "get", 1)
	{
		
	}
	/*! Destroys a setter
	 */
	virtual ~DatabasePropertyGetter()
	{
		
	}
	/*! Determines, whether it can be called with this context
		\param[in] ctx context
	 */
	virtual sad::Maybe<QString> canBeCalled(QScriptContext* ctx)
	{
		sad::Maybe<QString> result;
		checkArgumentCount(result, ctx);
		checkArgument<sad::String>(result, 0, ctx);
		if (result.exists() == false)
		{
			sad::db::TypeName<T>::init();
			sad::db::Database* me = sad::Renderer::ref()->database("");
			sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(0));
			sad::db::Property* prop = me->propertyByName(propname.value());
			if (prop)
			{
				if (prop->baseType() != sad::db::TypeName<T>::baseName() || prop->pointerStarsCount() != 0)
				{
					QString qpropname = propname.value().c_str();
					QString basetype = sad::db::TypeName<T>::baseName().c_str();
					result.setValue(QString("property ") + qpropname + QString(" is not of type ") + basetype);
				}
			}
			else
			{
				result.setValue(QString(propname.value().c_str()) + " is not writeable");
			}
		}
		return result;
	}
	/*! Calls actually a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	virtual QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(0));

		sad::db::Database* me = sad::Renderer::ref()->database("");
		sad::Maybe<T> oldvalue = me->getProperty<T>(propname.value());
		
		return scripting::FromValue<T>::perform(oldvalue.value(), engine);
	}
};

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
/*! Converts a list to a value
	\param[in] list a list of strings
	\param[in] engine an engine
 */
QScriptValue listToValue(const QStringList& list, QScriptEngine* engine);
/*! Lists database properties
	\param[in] ctx context
	\param[in] engine an engine list
	\return list of strings with propeties of database
 */
QScriptValue listProperties(QScriptContext* ctx, QScriptEngine* engine);
/*! Returns type of object
	\param[in] s scripting part
	\param[in] o object
	\return an object type
 */
sad::String objectType(scripting::Scripting* s, sad::db::Object* o);
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

