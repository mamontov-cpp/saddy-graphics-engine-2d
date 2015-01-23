/*! \file abstractsetter.h
	\author HiddenSeeker

	Defines a basic setter for property
 */
#pragma once
#include "maybe.h"
#include "callable.h"

#include <equalto.h>

#include <db/dbproperty.h>
#include <db/dbobject.h>

namespace scripting
{
	
template<
	typename _Type,
	typename _PropetyType
>
class AbstractSetter: public scripting::Callable
{
public:
	/*! Represents a constructor call for a function with two arguments
		\param[in] e engine
		\param[in] name name of function call
		\param[in] argumentcount count of arguments
	 */
	AbstractSetter(
		QScriptEngine* e, 
		const QString& name
	) : scripting::Callable(e, name ,3)
	{
		
	}
	/*! Could be inherited
	 */
	virtual ~AbstractSetter()
	{
		
	}
	/*! Pushes new matches property name
		\param[in] name a property name
	 */
	void addMatched(const QString& name)
	{
		m_matched_property_names << name.toStdString();
	}
	/*! Adds excluded property name
		\param[in] name a property name
	 */
	void addExcluded(const QString& name)
	{
		m_excluded_property_names << name.toStdString();
	}
	/*! Determines, whether it can be called with this context
		\param[in] ctx context
		\return whether it could be called, or error
	 */
	virtual sad::Maybe<QString> canBeCalled(QScriptContext* ctx)
	{
		sad::Maybe<QString> result;
		checkArgumentCount(result, ctx);
		checkArgument<_Type>(result, 0, ctx);
		checkArgument<sad::String>(result, 1, ctx);
		checkArgument<_PropetyType>(result, 2, ctx);
		if (result.exists() == false)
		{
			sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(1));
			bool propertymatches = true;
			if (m_matched_property_names.size())
			{
				propertymatches = std::find(
					m_matched_property_names.begin(), 
					m_matched_property_names.end(), 
					propname.value()
				) != m_matched_property_names.end();
			}
			if (m_excluded_property_names.size() && propertymatches)
			{
				propertymatches = std::find(
					m_excluded_property_names.begin(), 
					m_excluded_property_names.end(), 
					propname.value()
				) == m_excluded_property_names.end();
			}

			if (propertymatches)
			{
				sad::Maybe<_Type> basicvalue = scripting::ToValue<_Type>::perform(ctx->argument(0));
				sad::db::Object* object = basicvalue.value();
				sad::db::Property* prop = object->getObjectProperty(propname.value());
				if (prop)
				{
					sad::db::TypeName<_PropetyType>::init();
					if (prop->baseType() != sad::db::TypeName<_PropetyType>::baseName() || prop->pointerStarsCount() != 0)
					{
						QString qpropname = propname.value().c_str();
						QString basetype = sad::db::TypeName<_PropetyType>::baseName().c_str();
						result.setValue(QString("property ") + qpropname + QString(" is not of type ") + basetype);
					}
				}
				else
				{
					propertymatches = false;
				}
			}

			if (propertymatches == false)
			{
				QString qpropname = propname.value().c_str();
				result.setValue(QString("property ") + qpropname + QString(" is not writeable"));
			}
		}
		return result;
	}
	/*! Calls actually a function
		\param[in] ctx context
	 */
	virtual QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_Type>       basicvalue = scripting::ToValue<_Type>::perform(ctx->argument(0)); 
		sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(1));
		sad::Maybe<_PropetyType> newvalue = scripting::ToValue<sad::String>::perform(ctx->argument(2));

		sad::db::Object* object = basicvalue.value();
		sad::Maybe<_PropetyType> oldvalue = object->getProperty<_PropetyType>(propname.value());

		std::equal_to<_PropetyType> comparator;
		if (comparator(newvalue.value(), oldvalue.value()) == false)
		{
			setProperty(basicvalue.value(), propname.value(), oldvalue.value(), newvalue.value());
		}
		return ctx->thisObject();
	}
	/*! Performs actually setting property
		\param[in] obj an object to be set
		\param[in] propertyname a property for object
		\param[in] oldvalue old value 
		\param[in] newvalue new value
	 */
	virtual void setProperty(_Type obj, const sad::String& propertyname, _PropetyType oldvalue,  _PropetyType newvalue) = 0;
protected:
	/*! A matched properties list. If empty - every kind of property is matched
	 */
	sad::Vector<sad::String> m_matched_property_names;
	/*! A property names, that should be excluded from this name
	 */
	sad::Vector<sad::String> m_excluded_property_names;
};

}
